#include <gint/display.h>
#include <gint/keyboard.h>
#include <gint/drivers/r61524.h>
#include <gint/dma.h>

#include <string.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "fp.c"


#define maxEntities 128
#define shortrandMax 65536.0
#define shortrandMean 32768.0

unsigned short* const secondVRAM = (unsigned short*)0xe5007000;
#define blockSize 4

const unsigned short* keyboard_register = (unsigned short*)0xA44B0000;
unsigned short lastkey[8];
unsigned short holdkey[8];
unsigned short* p;
unsigned short* pointer;
unsigned int* p2;
const int32_t PI = (3.14159265358979323846264338327950288419716939937510582097 * FPOne);
const int32_t TAU = PI*2;
int32_t tick = 0;
const float SECS_PER_CLOCK = 1.0 / CLOCKS_PER_SEC;

const int characterSpacing = 16;

unsigned char playerBulletCount = 0;
unsigned char asteroidCount = 0;
unsigned char saucerCount = 0;

//const char maxEntities = 64;
unsigned char numEntities = 0;
object *entities[maxEntities];

clock_t t1 = 0;
clock_t t2 = 0;
float deltaT = 0.02;
float deltaTOld = 0.02;
int32_t deltaTFP;

clock_t t1Dim = 0;
clock_t t2Dim = 0;
float deltaTDim = 0.02;

clock_t t1Display = 0;
clock_t t2Display = 0;
float deltaTDisplay = 0.02;


unsigned int seed = 123456789;

unsigned short shortrand(void)
{
  seed = (1103515245 * seed + 12345);
  return seed>>16;
}

void keyupdate(void) {
    memcpy(holdkey, lastkey, sizeof(unsigned short) * 8);
    memcpy(lastkey, keyboard_register, sizeof(unsigned short) * 8);
}
int keydownlast(int basic_keycode) {
    int row, col, word, bit;
    row = basic_keycode % 10;
    col = basic_keycode / 10 - 1;
    word = row >> 1;
    bit = col + 8 * (row & 1);
    return (0 != (lastkey[word] & 1 << bit));
}
int keydownhold(int basic_keycode) {
    int row, col, word, bit;
    row = basic_keycode % 10;
    col = basic_keycode / 10 - 1;
    word = row >> 1;
    bit = col + 8 * (row & 1);
    return (0 != (holdkey[word] & 1 << bit));
}

unsigned short getColorFromShade(unsigned short shade){
	unsigned short x = shade>>1;
	return (x | (shade<<5) | (x<<11));
}

void drawLine(int32_t x1, int32_t y1, int32_t x2, int32_t y2){
	int32_t m;
	int32_t x;
	int32_t y;
	int offset;
	if (abs(x2-x1) > abs(y2-y1)){
		if (x1>x2){
			m = x2;
			x2 = x1;
			x1 = m;
			m = y2;
			y2 = y1;
			y1 = m;
		}
		m = FPDiv((y2-y1),(x2-x1));
		y = y1;
		for (x=x1; x <= x2; x+=FPOne){
			offset = FPToInt(x) + FPToInt(y)*DWIDTH;
			if (offset >= LCD_PIXELS){
				offset -= LCD_PIXELS;
			} else if (offset < 0){
				offset += LCD_PIXELS;
			}
			*(p + offset) = C_WHITE;
			y += m;
		}
	} else{
		if (y1>y2){
			m = x2;
			x2 = x1;
			x1 = m;
			m = y2;
			y2 = y1;
			y1 = m;
		}
		m = FPDiv((x2-x1),(y2-y1));
		x = x1;
		for (y=y1; y <= y2; y+=FPOne){
			offset = FPToInt(x) + FPToInt(y)*DWIDTH;
			if (offset >= LCD_PIXELS){
				offset -= LCD_PIXELS;
			} else if (offset < 0){
				offset += LCD_PIXELS;
			}
			*(p + offset) = C_WHITE;
			x += m;
		}
	}
}

int32_t fpsin(int32_t x) {
	if(x > PI) { return -fpsin(x - PI); }
	if(x > PI / 2) { return fpsin(PI - x); }
	return x - ((FPMult(FPMult(x,x),x)) / 6) + ((FPMult(FPMult(FPMult(FPMult(x,x),x),x),x)) / 120) - ((FPMult(FPMult(FPMult(FPMult(FPMult(FPMult(x,x),x),x),x),x),x)) / 5040);
}

int32_t fpcos(int32_t x) {
   return fpsin(x + (PI / 2));
}

int32_t fpsqrt(int32_t x){
	int32_t guess = FPOne;
	while (abs(FPMult(guess,guess) - x) > 1<<(FPOffset-4)){
		guess = (FPDiv(x,guess) + guess)/2;
	}
	return guess;
}

int32_t fpwrap(int32_t x){
	if (x<0){
		return x + TAU;
	} else if (x>=TAU){
		return x - TAU;
	}
	return x;
}

void deleteEntity(int index){
	int i;
	unsigned char ID = entities[index]->ID;
	if (ID == smallAsteroidID || ID == mediumAsteroidID || ID == largeAsteroidID){
		asteroidCount--;
	}
	if (ID == playerBulletID){
		playerBulletCount--;
	}
	if (ID == smallSaucerID || ID == largeSaucerID){
		saucerCount--;
	}
	numEntities--;
	free(entities[index]);
	for (i=index; i<numEntities; i++){
		entities[i] = entities[i+1];
	}
	
	
}

void createObject(int32_t x, int32_t y, int32_t vx, int32_t vy, int32_t rot, int32_t vrot, const int32_t (*shape)[][4], unsigned char ID, int32_t timer){
	if (numEntities<maxEntities){
		object* newObject;
		newObject = malloc(sizeof(object));
		
		if (newObject) {
			newObject->x = x;
			newObject->y = y;
			newObject->vx = vx;
			newObject->vy = vy;
			newObject->rot = rot;
			newObject->vrot = vrot;
			newObject->shape = shape;
			newObject->ID = ID;
			newObject->timer = timer;
			newObject->alive = 1;
			
			entities[numEntities] = newObject;
			
			numEntities++;
			
			if (ID == smallAsteroidID || ID == mediumAsteroidID || ID == largeAsteroidID){
				asteroidCount++;
			}
			if (ID == playerBulletID){
				playerBulletCount++;
			}
			if (ID == smallSaucerID || ID == largeSaucerID){
				saucerCount++;
			}
		}
	}
}

void drawStaticShape(int32_t x, int32_t y, const int32_t (*shape)[][4]){
	unsigned char i;
	for (i = 0; (*shape)[i][0] != endVal; i++){
		drawLine(intToFP((*shape)[i][1]) + x, intToFP((*shape)[i][0]) + y, intToFP((*shape)[i][3]) + x, intToFP((*shape)[i][2]) + y);
	}
}

unsigned char asteroidShapeCounter = 0;
int32_t  (*getRandomAsteroidShape(void))[][4]{ // c syntax can be hard to guess at times
	asteroidShapeCounter = (asteroidShapeCounter+1)%3;

	switch (asteroidShapeCounter){
		case 0:
			return &asteroidShape1;
			break;
		case 1:
			return &asteroidShape2;
			break;
		case 2:
			return &asteroidShape3;
			break;
	}

	return &asteroidShape0;
	
}

void summonAsteroids(char count, int32_t meanSpeed){
	int32_t x,y,vx,vy,rot,vrot;
	char i;
	for (i = 0; i < count; i++){
		if (shortrand()&1){
			x = intToFP(shortrand()%DWIDTH);
			y = 0;
		} else {
			x = 0;
			y = intToFP(shortrand()%DHEIGHT);
		}
		vx = (FPMult(intToFP(shortrand()) + intToFP(shortrand()), meanSpeed/shortrandMax));
		if ((int) shortrand() + (int) shortrand()>shortrandMax){
			vx = -vx;
		}
		vy = (FPMult(intToFP(shortrand()) + intToFP(shortrand()), meanSpeed/shortrandMax));
		if ((int) shortrand() + (int) shortrand()>shortrandMax){
			vy = -vy;
		}
		rot = (intToFP(shortrand()) / shortrandMean)*3.14;
		vrot = ((intToFP(shortrand()) / shortrandMean)-intToFP(1))*0.7;
		createObject(x,y,vx,vy,rot,vrot,getRandomAsteroidShape(),largeAsteroidID,0);	
	}
}

void drawText(int x, int y, char text[]){
	unsigned int i;
	for (i=0; i < strlen(text); i++){
		if (text[i] >= 0x41 && text[i] <= 0x5A){
			drawStaticShape(intToFP(x),intToFP(y),iconLets[text[i]-0x41]);
		} else if (text[i] >= 0x30 && text[i] <= 0x39){
			drawStaticShape(intToFP(x),intToFP(y),iconNums[text[i]-0x30]);
		}
		x += characterSpacing;
	}
}

void drawNumber(int x, int y, int number){
	int i,j;
	if (number < 0){
		number = -number;
		x += characterSpacing;
	}
	i = 1;
	while (number/i){
		i *= 10;
	}
	i /= 10;
	if (i==0){
		i = 1;
	}
	while (i){
		j = (number/i)%10;
		drawStaticShape(intToFP(x),intToFP(y),iconNums[j]);
		x += characterSpacing;
		i /= 10;
	}
}



int main(void) {
	p = gint_vram;
	
	int i;
	int j;
	int k;
	int32_t sinCr;
	int32_t cosCr;
	int32_t x1, y1, x2, y2;
	unsigned char trailType = 3;
	int32_t playerDeathTimer = 0;
	object *entity;
	object *entity2;
	
	int playerScore = 0;
	int playerScoreOld = 0;
	const int extraLifeInterval = 10000;
	const unsigned char playerSpawnRadius = 40;
	unsigned char playerLives = 0;
	unsigned char playerAlive = 0;
	const int32_t playerRotSpeed = intToFP(6);
	const int playerAccl = 150;
	const int32_t playerDrag = intToFP(5);
	const int32_t playerMaxSpeed = intToFP(300);
	const int playerBulletSpeed = 200;
	const int32_t playerBulletLifetime = intToFP(1);
	const int32_t playerDeathTimerMax = intToFP(3);

	char asteroidSpawnCount = 4;
	const int32_t asteroidSpawnDelay = intToFP(3);
	int32_t asteroidSpawnTimer = 0;
	const unsigned char smallAsteroidScore = 100;
	const unsigned char mediumAsteroidScore = 50;
	const unsigned char largeAsteroidScore = 20;
	int32_t asteroidSpawnSpeed = intToFP(30);
	const int32_t asteroidSpawnSpeedIncrease = intToFP(2);
	const int32_t asteroidSlipForwardMult = floatToFP(1.1);
	const int32_t asteroidSlipSidewaysMult = floatToFP(1.3);
	const int32_t asteroidSplitVRot = floatToFP(1.2);

	const int32_t saucerCheckDelay = intToFP(4);
	int32_t saucerCheckTimer = intToFP(4);
	unsigned short saucerSpawnChance = shortrandMax/5;
	unsigned short saucerSmallSpawnChance = shortrandMax/2;
	const int32_t saucerSpeed = intToFP(60);
	const int smallSaucerScore = 990;
	const int largeSaucerScore = 300;
	const int32_t saucerBulletInterval = intToFP(1);
	int32_t saucerBasePrecision = intToFP(2);
	int32_t saucerSmallPrecisionMult = floatToFP(0.5);
	const int32_t saucerBulletSpeed = intToFP(200);
	const int32_t saucerBulletLifetime = floatToFP(0.75);
	
	unsigned char gameState = gameEnd;
	unsigned char gameType = gamemodeNormal;
	
	
	
	
	
	
		
	//createObject(20,100,-30,0,0,0.8,&smallAsteroidShape,smallAsteroidID,0);
	//createObject(20,150,-20,0,0,-0.5,&mediumAsteroidShape,mediumAsteroidID,0);
	
	
	
	
	
	//for (i=0;i<30;i++){
	//	createObject(20+5*i,20+5*i,0,0,0,0.4,&largeAsteroidShape,largeAsteroidID,0);	
	//}
	
    
	
	dclear(C_BLACK);
	//p2 = (unsigned int*) gint_vram();
	//for (i = 0; i < LCD_PIXELS>>1; i++){
	//	*p2 = 0;
	//	p2++;
	//}
    
	
    while (1) {
		keyupdate();
		
		shortrand(); // makes the results more random
		
		if (keydownlast(48)) {
			getkey();
			//dupdate();
		}
		
		
		t1Dim = clock();
		//for (i = 0; i < DHEIGHT; i+=4){
		//	r61524_display(p,i,4,R61524_DMA);
		//}
		
		if (trailType){
			// this loop draws the vram to the screen, and dims the vram
			// it does this quickly by
			// 1 copying a block of vram to a place in on-chip memory
			// 2 copying that to the screen
			// 3 dimming the pixels
			// 4 copying it back to vram
			// steps 1 and 3 are done at the same time (same with 2 and 4), so it is (ideally) always copying to or from main ram
			// 
			// note that the on-chip memory isn't large enough for the entire framebuffer, and the loop abuses the fact that the memory loops
			p2 = (unsigned int*) secondVRAM;
			for (i = 0; i <= DHEIGHT; i+=blockSize){
				if (i<DHEIGHT){
					// start copy of new block from vram to on-chip memory
					dma_transfer_async(1,DMA_32B,DWIDTH*2*blockSize/32, p + (i*DWIDTH), DMA_INC, secondVRAM + (i*DWIDTH), DMA_INC, GINT_CALL_NULL);
				}
				if (i){
					// do the dimming on the block copied in the previous loop, while the new block is coppied in the background
					if (trailType == 3){
						for (j = i-blockSize; j < i; j++){
							pointer = secondVRAM + j*DWIDTH + ((tick+j)%3);
							for (k = 0; k < DWIDTH/3; k++){
								*pointer = ((*pointer)&0xF7DE)>>1;
								pointer += 3;
							}
						}
					} else if (trailType == 2){
						for (j = i-blockSize; j < i; j++){
							pointer = secondVRAM + j*DWIDTH + ((tick+j)&1);
							for (k = 0; k < DWIDTH>>1; k++){
								*pointer = ((*pointer)&0xF7DE)>>1;
								pointer += 2;
							}
						}
					} else {
						for (j = 0; j < DWIDTH*blockSize/2; j++){
							if (*p2){
								*p2 = ((*p2)&0xF7DEF7DE)>>1;
							}
							p2++;
						}
					}
				}
				if (i<DHEIGHT){
					// make sure copy is done
					dma_transfer_wait(1);
				}
				
				if (i){
					// start copy of previous block from on-chip memory to vram
					dma_transfer_async(2,DMA_32B,DWIDTH*blockSize/16, secondVRAM + ((i-blockSize)*DWIDTH), DMA_INC, p + ((i-blockSize)*DWIDTH), DMA_INC, GINT_CALL_NULL);
				}
				if (i<DHEIGHT){
					// copy new block to display
					r61524_display(secondVRAM,i,blockSize,R61524_DMA_WAIT);
				}
				if (i){
					// make sure copy is done
					dma_transfer_wait(2);
				}
			}
			
			
			p = gint_vram;
		} else{
			dupdate();
			dclear(C_BLACK);
			//for (i = 0; i < LCD_PIXELS>>1; i++){
			//	*p2 = 0;
			//	p2++;
			//}
		}
		t2Dim = clock();
		deltaTDim = (t2Dim-t1Dim) * SECS_PER_CLOCK * 0.01 + deltaTDim * 0.99;
		
		
		if (gameState == gameEnd){
			gameState = gameMenu;
		}
		if (gameState == gameMenu){
			if (playerScore > 0){
				drawText(50,10,"SCORE");
				drawNumber(50+7*characterSpacing,10,playerScore);
			}
			drawText(50,50,"F1  NORMAL PLAY");
			drawText(50,70,"F2  ASTEROIDS ONLY");
			drawText(50,110,"X0T LOG  TURN");
			drawText(50,130,"COS      THRUSTER");
			drawText(50,150,"TAN      FIRE");
			drawText(50,190,"1 ADJUST TRAILS");
			
			if (keydownlast(79)){
				gameState = gameStart;
				gameType = gamemodeNormal;
			} else if (keydownlast(69)){
				gameState = gameStart;
				gameType = gamemodeAsteroids;
			}
		}
		if (gameState == gameStart){
			for (i=numEntities-1; i>=0; i--){
				deleteEntity(i);
			}
			
			playerScore = 0;
			playerScoreOld = 0;
			playerLives = 3;
			playerAlive = 1;
			
			asteroidSpawnCount = 4;
			asteroidSpawnTimer = 0;
			asteroidSpawnSpeed = intToFP(20);
			
			saucerCheckTimer = intToFP(4);
			
			createObject(intToFP(DWIDTH/2),intToFP(DHEIGHT/2),0,0,0,0,&playerShape,playerID,0);
			
			gameState = gamePlay;
		}
		
		if (keydownlast(72)){
			if (!keydownhold(72)){
				trailType = (trailType+1)%4;
			}
			drawNumber(DWIDTH-6-1,10,trailType);
		}
		
		if (playerScore/extraLifeInterval > playerScoreOld/extraLifeInterval){
			playerLives ++;
		}
		playerScoreOld = playerScore;
		
		
		deltaTOld = deltaT;
		t2 = clock();
		deltaT = (t2-t1) * SECS_PER_CLOCK;
		t1 = t2;
		if (deltaT > 0.2){
			deltaT = 0;
		}
		deltaT = deltaT*0.2 + deltaTOld*0.8;
		
		deltaTFP = floatToFP(deltaT);
		
		if (!playerAlive){
			if (playerDeathTimer>0){
				playerDeathTimer -= deltaTFP;
			} else {
				if (playerLives){
					x1 = 1;
					
					for (i = 0; i<numEntities; i++){
						entity2 = entities[i];
						x2 = playerSpawnRadius+sizes[entity2->ID];
						if (abs(FPToInt(entity2->x) - DWIDTH/2)<x2 && abs(FPToInt(entity2->y) - DHEIGHT/2)<x2){
							x1 = 0;
						}
					}
					if (x1){
						createObject(intToFP(DWIDTH/2),intToFP(DHEIGHT/2),0,0,0,0,&playerShape,playerID,0);
						playerAlive = 1;
					}
				} else {
					gameState = gameEnd;
				}
			}
		}

		if (!asteroidCount){
			if (asteroidSpawnTimer > 0){
				asteroidSpawnTimer -= deltaTFP;
			} else {
				summonAsteroids(asteroidSpawnCount,asteroidSpawnSpeed);
				asteroidSpawnSpeed += asteroidSpawnSpeedIncrease;
				asteroidSpawnCount = 6;
				asteroidSpawnTimer += asteroidSpawnDelay;
			}
		}

		if (!saucerCount && playerLives && gameType == gamemodeNormal){
			if (saucerCheckTimer > 0){
				saucerCheckTimer -= deltaTFP;
			} else {
				saucerCheckTimer += saucerCheckDelay;
				if (shortrand() < saucerSpawnChance){
					x1 = 0;
					y1 = intToFP((shortrand()%(DHEIGHT>>1)) + (shortrand()%(DHEIGHT>>1)));
					if (shortrand()&1){
						x2 = saucerSpeed;
					} else {
						x2 = -saucerSpeed;
					}
					y2 = 0;
					if (playerScore > 30000){
						j = 1;
					} else {
						if (shortrand() < saucerSmallSpawnChance){
							j = 1;
						} else {
							j = 0;
						}
					}
					if (j){
						createObject(x1,y1,x2,y2,0,0,&smallSaucerShape,smallSaucerID,intToFP(3));
					} else {
						createObject(x1,y1,x2,y2,0,0,&largeSaucerShape,largeSaucerID,intToFP(3));
					}
				}
			}
		}
		
		
		//drawLine(25.0, 25.0, 350.0, 180.0);
		//drawLine(350.0, 25.0, 25.0, 180.0);
		//drawLine(100.0, 25.0, 150.0, 180.0);
		//drawLine(150.0, 25.0, 120.0, 180.0);

		
		for (i=numEntities-1; i>=0; i--){
			entity = entities[i];

			
			if (entity->ID == playerID){
				if (keydownlast(76)){
					entity->rot += FPMult(deltaTFP,playerRotSpeed);
				}
				if (keydownlast(66)){
					entity->rot -= FPMult(deltaTFP,playerRotSpeed);
				}
			}

			
			
			
			entity->rot += FPMult(entity->vrot, deltaTFP);
			entity->rot = fpwrap(entity->rot);
			
			sinCr = fpsin(entity->rot);
			cosCr = fpcos(entity->rot);
			if (entity->ID == playerID){
				if (keydownlast(36)){
					entity->vx += FPMult(cosCr,deltaTFP)*playerAccl;
					entity->vy -= FPMult(sinCr,deltaTFP)*playerAccl;
					entity->shape = &playerShapeBurning;
				} else {
					entity->shape = &playerShape;
				}
				x1 = fpsqrt(FPMult(entity->vx,entity->vx) + FPMult(entity->vy,entity->vy));
				if (x1>0) {
					if (x1>playerMaxSpeed){
						x2 = playerMaxSpeed; //cap speed
					} else {
						x2 = x1 - FPMult(playerDrag,deltaTFP); //reduce speed
						if (x2 < 0){
							x2 = 0;
						}
					}
					x2 = FPDiv(x2,x1);
					entity->vx = FPMult(entity->vx, x2);
					entity->vy = FPMult(entity->vy, x2);
				}
				if (keydownlast(26) && !keydownhold(26) && playerBulletCount<4){
					createObject(
						entity->x,
						entity->y,
						entity->vx + cosCr*playerBulletSpeed,
						entity->vy - sinCr*playerBulletSpeed,
						entity->rot,
						0,
						&playerBulletShape,
						playerBulletID,
						playerBulletLifetime
					);
				}
			}
			
			if (entity->timer > 0){
				entity->timer -= deltaTFP;
			} else{
				switch (entity->ID){
					case playerBulletID:
						entity->alive = 0;
						entity->killer = playerID;
						break;
					case smallSaucerID:
					case largeSaucerID:
						entity->timer += saucerBulletInterval;
						if (playerAlive){
							x1 = 0;
							y1 = 0;
							for (j=0; j<numEntities; j++){
								entity2 = entities[j];
								if (entity2->ID == playerID){
									x1 = entity2->x - entity->x;
									y1 = entity2->y - entity->y;
								}
							}
							x2 = FPDiv(saucerBulletSpeed,fpsqrt(FPMult(x1,x1) + FPMult(y1,y1)));
							x1 = FPMult(x1, x2);
							y1 = FPMult(y1, x2);
							x2 = FPMult(((intToFP(shortrand()) / shortrandMean)-FPOne), saucerBasePrecision);
							if (entity->ID == smallSaucerID){
								x2 = FPMult(x2, saucerSmallPrecisionMult);
							}
							x2 = fpwrap(x2);
							cosCr = fpcos(x2);
							sinCr = fpsin(x2);
							createObject(entity->x, entity->y, FPMult(x1,cosCr)+FPMult(y1,sinCr), FPMult(y1,cosCr)+FPMult(x1,sinCr), 0, 0, &saucerBulletShape, saucerBulletID, saucerBulletLifetime);
						}
						break;
					case saucerBulletID:
						entity->alive = 0;
						entity->killer = smallSaucerID;
						break;
				}
			}
			
			entity->x += FPMult(entity->vx,deltaTFP);
			entity->y += FPMult(entity->vy,deltaTFP);
			if (entity->x >= intToFP(DWIDTH)){
				entity->x -= intToFP(DWIDTH);
				entity->y += intToFP(1);
			} else if (entity->x < 0){
				entity->x += intToFP(DWIDTH);
				entity->y -= intToFP(1);
			}
			if (entity->y >= intToFP(DHEIGHT)){
				entity->y -= intToFP(DHEIGHT);
			} else if (entity->y < 0){
				entity->y += intToFP(DHEIGHT);
			}
			
			if (entity -> alive){
				for (j=0; j<numEntities; j++){
					entity2 = entities[j];
					x1 = intToFP(sizes[entity->ID]+sizes[entity2->ID]);
					if (collisionTable[entity->ID][entity2->ID] && abs(entity2->x - entity->x)<x1 && abs(entity2->y - entity->y)<x1 && entity2->alive){
						entity->alive = 0;
						entity->killer = entity2->ID;
						entity2->alive = 0;
						entity2->killer = entity->ID;
						break;
					}
				}
			}
			
			if (entity->ID == mediumAsteroidID){
				cosCr >>= 1;
				sinCr >>= 1;
			}
			if (entity->ID == smallAsteroidID){
				cosCr >>= 2;
				sinCr >>= 2;
			}
			for (j = 0; (*entity->shape)[j][0] != endVal; j++){
				x1 = (*entity->shape)[j][0]*cosCr + (*entity->shape)[j][1]*sinCr + entity->x;
				y1 = (*entity->shape)[j][1]*cosCr - (*entity->shape)[j][0]*sinCr + entity->y;
				x2 = (*entity->shape)[j][2]*cosCr + (*entity->shape)[j][3]*sinCr + entity->x;
				y2 = (*entity->shape)[j][3]*cosCr - (*entity->shape)[j][2]*sinCr + entity->y;
				drawLine(x1,y1,x2,y2);
			}
			
			if (!entity->alive){
				switch (entity->ID){
				case playerID:
					playerAlive = 0;
					if (playerLives>0){
						playerDeathTimer = playerDeathTimerMax;
						playerLives -= 1;
					}
					break;
				case smallAsteroidID:
					if (entity->killer == playerID || entity->killer == playerBulletID){
						playerScore += smallAsteroidScore;
					}
					break;
				case mediumAsteroidID:
					if (entity->killer == playerID || entity->killer == playerBulletID){
						playerScore += mediumAsteroidScore;
					}
					cosCr = FPMult(intToFP((int)shortrand() + (int)shortrand()) / shortrandMax, asteroidSlipForwardMult);
					sinCr = FPMult(intToFP((int)shortrand() - (int)shortrand()) / shortrandMax, asteroidSlipSidewaysMult);
					createObject(
						entity->x,
						entity->y,
						FPMult(entity->vx, cosCr) + FPMult(entity->vy, sinCr),
						FPMult(entity->vy, cosCr) - FPMult(entity->vx, sinCr),
						entity->rot + floatToFP(0.5),
						FPMult(entity->vrot, -asteroidSplitVRot),
						getRandomAsteroidShape(),
						smallAsteroidID,
						0.0
					);
					cosCr = FPMult(intToFP((int)shortrand() + (int)shortrand()) / shortrandMax, asteroidSlipForwardMult);
					sinCr = FPMult(intToFP((int)shortrand() - (int)shortrand()) / shortrandMax, asteroidSlipSidewaysMult);
					createObject(
						entity->x,
						entity->y,
						FPMult(entity->vx, cosCr) + FPMult(entity->vy, sinCr),
						FPMult(entity->vy, cosCr) - FPMult(entity->vx, sinCr),
						-entity->rot - floatToFP(1.7),
						FPMult(entity->vrot, asteroidSplitVRot),
						getRandomAsteroidShape(),
						smallAsteroidID,
						0.0
					);
					break;
				case largeAsteroidID:
					if (entity->killer == playerID || entity->killer == playerBulletID){
						playerScore += largeAsteroidScore;
					}
					cosCr = FPMult(intToFP((int)shortrand() + (int)shortrand()) / shortrandMax, asteroidSlipForwardMult);
					sinCr = FPMult(intToFP((int)shortrand() - (int)shortrand()) / shortrandMax, asteroidSlipSidewaysMult);
					createObject(
						entity->x,
						entity->y,
						FPMult(entity->vx, cosCr) + FPMult(entity->vy, sinCr),
						FPMult(entity->vy, cosCr) - FPMult(entity->vx, sinCr),
						entity->rot + floatToFP(1.5),
						FPMult(entity->vrot, -asteroidSplitVRot),
						getRandomAsteroidShape(),
						mediumAsteroidID,
						0.0
					);
					cosCr = FPMult(intToFP((int)shortrand() + (int)shortrand()) / shortrandMax, asteroidSlipForwardMult);
					sinCr = FPMult(intToFP((int)shortrand() - (int)shortrand()) / shortrandMax, asteroidSlipSidewaysMult);
					createObject(
						entity->x,
						entity->y,
						FPMult(entity->vx, cosCr) + FPMult(entity->vy, sinCr),
						FPMult(entity->vy, cosCr) - FPMult(entity->vx, sinCr),
						-entity->rot - floatToFP(0.4),
						FPMult(entity->vrot, asteroidSplitVRot),
						getRandomAsteroidShape(),
						mediumAsteroidID,
						0.0
					);
					break;
				case smallSaucerID:
					if (entity->killer == playerID || entity->killer == playerBulletID){
						playerScore += smallSaucerScore;
					}
					break;
				case largeSaucerID:
					if (entity->killer == playerID || entity->killer == playerBulletID){
						playerScore += largeSaucerScore;
					}
					break;
				}
				deleteEntity(i);
			}
		}
		for (i=0; i<playerLives; i++){
			drawStaticShape(floatToFP(i*10+5.5),floatToFP(32.2),&iconPlayerShape);
		}
		
		if (gameState == gamePlay){
			drawNumber(6,10,playerScore);
		}
		
		//drawNumber(6,60,deltaT*10000);
		//drawNumber(6,80,deltaTDim*10000);
		//drawNumber(6,100,deltaTDisplay*10000);
		
		//drawNumber(6,80,CLOCKS_PER_SEC);
		//drawNumber(6,100,((int) -1) >> 5);
		
		//drawText(7,50,"ABCDEFGHIJKLMNOPQRSTUV");
		//drawText(7,70,"WXYZ");
		
		
		//for (i = 0; i<(int)(deltaTFP*1000.0); i++){
		//	drawLine((i/5)*15,(i%5)*5+2,10+(i/5)*15,(i%5)*5+2);	
		//}
		//drawLine(0,10,deltaTFP*15.0*((float)DWIDTH),10);
		
        
		
		
        //Print_OS("Press EXE to exit", 0, 0);
		tick += 1;
        
		t1Display = clock();
        //dupdate();
		t2Display = clock();
		deltaTDisplay = (t2Display-t1Display) * SECS_PER_CLOCK * 0.01 + deltaTDisplay * 0.99;
		
		
		
		
    }
 
    return 0;
}
