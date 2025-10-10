#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include "main.h"


#define maxEntities 128
#define shortrandMax 65536.0
#define shortrandMean 32768.0

const unsigned short* keyboard_register = (unsigned short*)0xA44B0000;
unsigned short lastkey[8];
unsigned short holdkey[8];
unsigned short* p;
unsigned int* p2;
const float PI = 3.14159265358979323846264338327950288419716939937510582097;
const float TAU = PI*2.0;
int tick;
const float SECS_PER_CLOCK_FLOAT = 1.0 / (float)CLOCKS_PER_SEC;


unsigned char playerBulletCount = 0;
unsigned char asteroidCount = 0;
unsigned char saucerCount = 0;

//const char maxEntities = 64;
unsigned char numEntities = 0;
object *entities[maxEntities];

clock_t t1 = 0;
clock_t t2 = 0;
float deltaT = 0.02;
float deltaTOld;

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

void drawLine(float x1, float y1, float x2, float y2){
	float m;
	float x;
	float y;
	int offset;
	if (fabs(x2-x1) > fabs(y2-y1)){
		if (x1>x2){
			m = x2;
			x2 = x1;
			x1 = m;
			m = y2;
			y2 = y1;
			y1 = m;
		}
		m = (y2-y1)/(x2-x1);
		y = y1;
		for (x=x1; x <= x2; x++){
			offset = ((int)x) + ((int)y)*LCD_WIDTH_PX;
			if (offset >= LCD_PIXELS){
				offset -= LCD_PIXELS;
			} else if (offset < 0){
				offset += LCD_PIXELS;
			}
			*(p + offset) = COLOR_WHITE;
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
		m = (x2-x1)/(y2-y1);
		x = x1;
		for (y=y1; y <= y2; y++){
			offset = ((int)x) + ((int)y)*LCD_WIDTH_PX;
			if (offset >= LCD_PIXELS){
				offset -= LCD_PIXELS;
			} else if (offset < 0){
				offset += LCD_PIXELS;
			}
			*(p + offset) = COLOR_WHITE;
			x += m;
		}
	}
}

float fsin(float x) {
   if(x > PI) { return -fsin(x - PI); }
   if(x > PI / 2) { return fsin(PI - x); }
   return x - ((x*x*x) / 6.0) + ((x*x*x*x*x) / 120.0) - ((x*x*x*x*x*x*x) / 5040.0);
}

float fcos(float x) {
   return fsin(x + (PI / 2));
}

float fsqrt(float x){
	float guess = 1;
	while (fabs(guess*guess - x) > 0.1){
		guess = ((x/guess) + guess)/2;
	}
	return guess;
}

float fwrap(float x){
	if (x<0){
		return x + TAU;
	} else if (x>=PI){
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

void createObject(float x, float y, float vx, float vy, float rot, float vrot, const float (*shape)[][2], unsigned char ID, float timer){
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

void drawStaticShape(float x, float y, const float (*shape)[][4], unsigned char length){
	unsigned char i;
	for (i = 0; i<length; i++){
		drawLine((*shape)[i][1] + x, (*shape)[i][0] + y, (*shape)[i][3] + x, (*shape)[i][2] + y);
	}
}

void summonAsteroids(char count, float meanSpeed){
	float x,y,vx,vy,rot,vrot;
	char i;
	for (i = 0; i < count; i++){
		if (shortrand()&1){
			x = shortrand()%LCD_WIDTH_PX;
			y = 0;
		} else {
			x = 0;
			y = shortrand()%LCD_HEIGHT_PX;
		}
		vx = (((float)shortrand() + (float)shortrand()) / shortrandMax)*meanSpeed;
		if (shortrand()+shortrand()>shortrandMax){
			vx = -vx;
		}
		vy = (((float)shortrand() + (float)shortrand()) / shortrandMax)*meanSpeed;
		if (shortrand()+shortrand()>shortrandMax){
			vy = -vy;
		}
		rot = ((float)shortrand() / shortrandMean)*3.14;
		vrot = (((float)shortrand() / shortrandMean)-1)*0.7;
		createObject(x,y,vx,vy,rot,vrot,&largeAsteroidShape,largeAsteroidID,0);	
	}
}

int main(void) {
	int key;
	p = GetVRAMAddress();
	

	int i;
	int j;
	float sinCr;
	float cosCr;
	float x1, y1, x2, y2;
	unsigned char length;
	unsigned char trails = 1;
	float playerDeathTimer = 0;
	object *entity;
	object *entity2;
	
	unsigned int playerScore = 0;
	const unsigned char playerSpawnRadius = 25;
	unsigned char playerLives = 3;
	unsigned char playerAlive = 1;
	const float playerRotSpeed = 6;
	const float	playerAccl = 150;
	const float playerDrag = 5;
	const float	playerMaxSpeed = 300;
	const float playerBulletSpeed = 200;
	const float playerBulletLifetime = 1;
	const float playerDeathTimerMax = 3;

	char asteroidSpawnCount = 4;
	const float asteroidSpawnDelay = 3;
	float asteroidSpanTimer = 0;
	const unsigned char smallAsteroidScore = 100;
	const unsigned char mediumAsteroidScore = 50;
	const unsigned char largeAsteroidScore = 20;
	float asteroidSpawnSpeed = 20;
	const float asteroidSpawnSpeedIncrease = 2;
	const float asteroidSlipForwardMult = 1.2/shortrandMax;
	const float asteroidSlipSidewaysMult = 1.5/shortrandMax;
	const float asteroidSplitVRot = 1.2;

	const float saucerCheckDelay = 4;
	float saucerCheckTimer = 4;
	unsigned short saucerSpawnChance = shortrandMax/5;
	unsigned short saucerSmallSpawnChance = shortrandMax/2;
	const float saucerSpeed = 60;
	const short smallSaucerScore = 990;
	const short largeSaucerScore = 300;
	const float saucerBulletInterval = 1;
	float saucerBasePrecision = 0.5;
	float saucerSmallPrecisionMult = 0.5;
	const float saucerBulletSpeed = 200;
	const float saucerBulletLifetime = 0.75;
	
	
	
	
	createObject(LCD_WIDTH_PX/2,LCD_HEIGHT_PX/2,0,0,0,0,&playerShape,playerID,0);
	//createObject(20,100,-30,0,0,0.8,&smallAsteroidShape,smallAsteroidID,0);
	//createObject(20,150,-20,0,0,-0.5,&mediumAsteroidShape,mediumAsteroidID,0);
	
	
	
	
	
	//for (i=0;i<30;i++){
	//	createObject(20+5*i,20+5*i,0,0,0,0.4,&largeAsteroidShape,largeAsteroidID,0);	
	//}
	
    
	
	DrawFrame(0);
	p2 = GetVRAMAddress();
	for (i = 0; i < LCD_PIXELS>>1; i++){
		*p2 = 0;
		p2++;
	}
    
	
    while (1) {
		Bdisp_EnableColor(1);
		
		keyupdate();
		
		shortrand(); // makes the results more random
		
		if (keydownlast(KEY_PRGM_MENU)) {
			GetKey(&key);
			DrawFrame(0);
		}
		p2 = GetVRAMAddress();
		if (trails){
			for (i = 0; i < LCD_PIXELS>>1; i++){
				if (*p2){
					*p2 = ((*p2)&0xF7DEF7DE)>>1;
				}
				p2++;
			}
		} else{
			for (i = 0; i < LCD_PIXELS>>1; i++){
				*p2 = 0;
				p2++;
			}
		}
		
		if (keydownlast(72) && !keydownhold(72)){
			trails = (trails+1)&0x1;
		}
		
		
		deltaTOld = deltaT;
		t2 = clock();
		deltaT = (float)(t2-t1) * SECS_PER_CLOCK_FLOAT;
		t1 = t2;
		if (deltaT > 0.2){
			deltaT = 0.0;
		}
		deltaT = deltaT*0.2 + deltaTOld*0.8;
		
		if (!playerAlive && playerLives){
			if (playerDeathTimer>0){
				playerDeathTimer -= deltaT;
			} else {
				x1 = 1;
				
				for (i = 0; i<numEntities; i++){
					entity2 = entities[i];
					x2 = playerSpawnRadius+sizes[entity2->ID];
					if (fabs(entity2->x - LCD_WIDTH_PX/2)<x2 && fabs(entity2->y - LCD_HEIGHT_PX/2)<x2){
						x1 = 0;
					}
				}
				if (x1){
					createObject(LCD_WIDTH_PX/2,LCD_HEIGHT_PX/2,0,0,0,0,&playerShape,playerID,0);
					playerAlive = 1;
				}
			}
		}

		if (!asteroidCount){
			if (asteroidSpanTimer > 0){
				asteroidSpanTimer -= deltaT;
			} else {
				summonAsteroids(asteroidSpawnCount,asteroidSpawnSpeed);
				asteroidSpawnSpeed += asteroidSpawnSpeedIncrease;
				asteroidSpawnCount = 6;
				asteroidSpanTimer += asteroidSpawnDelay;
			}
		}

		if (!saucerCount){
			if (saucerCheckTimer > 0){
				saucerCheckTimer -= deltaT;
			} else {
				saucerCheckTimer += saucerCheckDelay;
				if (shortrand() < saucerSpawnChance){
					x1 = 0;
					y1 = shortrand()%LCD_HEIGHT_PX;
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
						createObject(x1,y1,x2,y2,0,0,&smallSaucerShape,smallSaucerID,3);
					} else {
						createObject(x1,y1,x2,y2,0,0,&largeSaucerShape,largeSaucerID,3);
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
			
			length = lengths[entity->ID];
			if (entity->ID == playerID){
				if (keydownlast(76)){
					entity->rot += deltaT*playerRotSpeed;
				}
				if (keydownlast(66)){
					entity->rot -= deltaT*playerRotSpeed;
				}
			}

			
			
			
			entity->rot += entity->vrot * deltaT;
			entity->rot = fwrap(entity->rot);
			
			sinCr = fsin(entity->rot);
			cosCr = fcos(entity->rot);
			if (entity->ID == playerID){
				if (keydownlast(36)){
					entity->vx += cosCr*deltaT*playerAccl;
					entity->vy -= sinCr*deltaT*playerAccl;
				}
				x1 = fsqrt(entity->vx*entity->vx + entity->vy*entity->vy);
				if (x1>0.0) {
					if (x1>playerMaxSpeed){
						x2 = playerMaxSpeed; //cap speed
					} else {
						x2 = x1 - playerDrag*deltaT; //reduce speed
						if (x2 < 0.0){
							x2 = 0;
						}
					}
					x2 = x2/x1;
					entity->vx = entity->vx * x2;
					entity->vy = entity->vy * x2;
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
				entity->timer -= deltaT;
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
							x2 = saucerBulletSpeed/fsqrt(x1*x1 + y1*y1);
							x1 *= x2;
							y1 *= x2;
							x2 = (((float)shortrand() / shortrandMean)-1) * saucerBasePrecision;
							if (entity->ID == smallSaucerID){
								x2 *= saucerSmallPrecisionMult;
							}
							cosCr = fcos(x2);
							sinCr = fsin(x2);
							createObject(entity->x, entity->y, x1*cosCr+y1*sinCr, y1*cosCr+x1*sinCr, 0, 0, &saucerBulletShape, saucerBulletID, saucerBulletLifetime);
						}
						break;
					case saucerBulletID:
						entity->alive = 0;
						entity->killer = smallSaucerID;
						break;
				}
			}
			
			entity->x += entity->vx*deltaT;
			entity->y += entity->vy*deltaT;
			if (entity->x >= LCD_WIDTH_PX){
				entity->x -= LCD_WIDTH_PX;
				entity->y += 1.0;
			} else if (entity->x < 0){
				entity->x += LCD_WIDTH_PX;
				entity->y -= 1.0;
			}
			if (entity->y >= LCD_HEIGHT_PX){
				entity->y -= LCD_HEIGHT_PX;
			} else if (entity->y < 0){
				entity->y += LCD_HEIGHT_PX;
			}
			
			if (entity -> alive){
				for (j=0; j<numEntities; j++){
					entity2 = entities[j];
					x1 = sizes[entity->ID]+sizes[entity2->ID];
					if (collisionTable[entity->ID][entity2->ID] && fabs(entity2->x - entity->x)<x1 && fabs(entity2->y - entity->y)<x1 && entity2->alive){
						entity->alive = 0;
						entity->killer = entity2->ID;
						entity2->alive = 0;
						entity2->killer = entity->ID;
						break;
					}
				}
			}
			
			
			x1 = (*entity->shape)[length-1][0]*cosCr + (*entity->shape)[length-1][1]*sinCr + entity->x;
			y1 = (*entity->shape)[length-1][1]*cosCr - (*entity->shape)[length-1][0]*sinCr + entity->y;
			for (j = 0; j<length; j++){
				x2 = (*entity->shape)[j][0]*cosCr + (*entity->shape)[j][1]*sinCr + entity->x;
				y2 = (*entity->shape)[j][1]*cosCr - (*entity->shape)[j][0]*sinCr + entity->y;
				drawLine(x1,y1,x2,y2);
				x1 = x2;
				y1 = y2;
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
					cosCr = ((float)shortrand() + (float)shortrand()) * asteroidSlipForwardMult;
					sinCr = ((float)shortrand() - (float)shortrand()) * asteroidSlipSidewaysMult;
					createObject(
						entity->x,
						entity->y,
						entity->vx * cosCr + entity->vy * sinCr,
						entity->vy * cosCr - entity->vx * sinCr,
						entity->rot + 0.5,
						entity->vrot * -asteroidSplitVRot,
						&smallAsteroidShape,
						smallAsteroidID,
						0.0
					);
					cosCr = ((float)shortrand() + (float)shortrand()) * asteroidSlipForwardMult;
					sinCr = ((float)shortrand() - (float)shortrand()) * asteroidSlipSidewaysMult;
					createObject(
						entity->x,
						entity->y,
						entity->vx * cosCr - entity->vy * sinCr,
						entity->vy * cosCr + entity->vx * sinCr,
						-entity->rot - 1.7,
						entity->vrot * asteroidSplitVRot,
						&smallAsteroidShape,
						smallAsteroidID,
						0.0
					);
					break;
				case largeAsteroidID:
					if (entity->killer == playerID || entity->killer == playerBulletID){
						playerScore += largeAsteroidScore;
					}
					cosCr = ((float)shortrand() + (float)shortrand()) * asteroidSlipForwardMult;
					sinCr = ((float)shortrand() - (float)shortrand()) * asteroidSlipSidewaysMult;
					createObject(
						entity->x,
						entity->y,
						entity->vx * cosCr + entity->vy * sinCr,
						entity->vy * cosCr - entity->vx * sinCr,
						entity->rot + 1.5,
						entity->vrot * -asteroidSplitVRot,
						&mediumAsteroidShape,
						mediumAsteroidID,
						0.0
					);
					cosCr = ((float)shortrand() + (float)shortrand()) * asteroidSlipForwardMult;
					sinCr = ((float)shortrand() - (float)shortrand()) * asteroidSlipSidewaysMult;
					createObject(
						entity->x,
						entity->y,
						entity->vx * cosCr - entity->vy * sinCr,
						entity->vy * cosCr + entity->vx * sinCr,
						-entity->rot - 0.4,
						entity->vrot * asteroidSplitVRot,
						&mediumAsteroidShape,
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
			drawStaticShape(i*10+5.5,32.2,&iconPlayerShape,4);
		}
		
		i = 1;
		while (playerScore/i){
			i *= 10;
		}
		i /= 10;
		if (i==0){
			i = 1;
		}
		x1 = 6;
		y1 = 10;
		while (i){
			j = (playerScore/i)%10;
			drawStaticShape(x1,y1,iconNums[j],iconNumLengths[j]);
			x1 += 16;
			i /= 10;
		}
		
		
		//for (i = 0; i<(int)(deltaT*1000.0); i++){
		//	drawLine((i/5)*15,(i%5)*5+2,10+(i/5)*15,(i%5)*5+2);	
		//}
		//drawLine(0,10,deltaT*15.0*((float)LCD_WIDTH_PX),10);
		
        
		
		
        //Print_OS("Press EXE to exit", 0, 0);

        
        Bdisp_PutDisp_DD();
		
		
		
		
    }
 
    return 0;
}
