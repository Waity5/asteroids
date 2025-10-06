#include <fxcg/display.h>
#include <fxcg/keyboard.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include "main.h"


#define maxEntities 64

const unsigned short* keyboard_register = (unsigned short*)0xA44B0000;
unsigned short lastkey[8];
unsigned short holdkey[8];
unsigned short* p;
unsigned int* p2;
const float PI = 3.14159265358979323846264338327950288419716939937510582097;
const float TAU = PI*2.0;
int tick;
const unsigned short colour_red = 0xF800;
const unsigned short colour_green = 0x07E0;
const unsigned short colour_blue = 0x001F;
const unsigned short colour_black = 0x0000;
const unsigned short colour_white = 0xFFFF;
const unsigned int colour_black_double = 0;
const float SECS_PER_CLOCK_FLOAT = 1.0 / (float)CLOCKS_PER_SEC;

//const char maxEntities = 64;
unsigned char numEntities = 0;
object *entities[maxEntities];

clock_t t1 = 0;
clock_t t2 = 0;
float deltaT = 0.02;
float deltaTOld;



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

unsigned short getColourFromShade(unsigned short shade){
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
		for (x=x1; x < x2; x++){
			offset = ((int)x) + ((int)y)*LCD_WIDTH_PX;
			if (offset >= LCD_PIXELS){
				offset -= LCD_PIXELS;
			} else if (offset < 0){
				offset += LCD_PIXELS;
			}
			*(p + offset) = getColourFromShade(63);
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
		for (y=y1; y < y2; y++){
			offset = ((int)x) + ((int)y)*LCD_WIDTH_PX;
			if (offset >= LCD_PIXELS){
				offset -= LCD_PIXELS;
			} else if (offset < 0){
				offset += LCD_PIXELS;
			}
			*(p + offset) = getColourFromShade(63);
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
		}
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
	object *entity;
	object *entity2;
	
	const float playerRotSpeed = 6;
	const float	playerAccl = 200;
	const float playerBulletSpeed = 200;
	const float asteroidSplitCos = 1;
	const float asteroidSplitSin = 0.2;
	const float asteroidSplitVRot = 1.2;
	
	
	
	createObject(150,100,0,0,0,0,&playerShape,playerID,0);
	createObject(20,100,-30,0,0,0.8,&smallAsteroidShape,smallAsteroidID,0);
	createObject(20,150,-20,0,0,-0.5,&mediumAsteroidShape,mediumAsteroidID,0);
	createObject(20,200,-10,0,0,0.4,&largeAsteroidShape,largeAsteroidID,0);	
    
	
    

    while (1) {
		Bdisp_EnableColor(1);
		
		keyupdate();
		
		if (keydownlast(KEY_PRGM_MENU)) {
			GetKey(&key);
		}
		DrawFrame(0);
		p2 = GetVRAMAddress();
		for (i = 0; i < LCD_WIDTH_PX*LCD_WIDTH_PY/2; i++){
			*p2 = 0;
			p2++;
		}
		
		deltaTOld = deltaT;
		t2 = clock();
		deltaT = (float)(t2-t1) * SECS_PER_CLOCK_FLOAT;
		t1 = t2;
		if (deltaT > 0.2){
			deltaT = 0.0;
		}
		deltaT = deltaT*0.2 + deltaTOld*0.8;
		
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
				if (keydownlast(26) && !keydownhold(26)){
					createObject(
						entity->x,
						entity->y,
						entity->vx + cosCr*playerBulletSpeed,
						entity->vy - sinCr*playerBulletSpeed,
						entity->rot,
						0,
						&playerBulletShape,
						playerBulletID,
						1.0
					);
				}
			}
			
			if (entity->timer > 0){
				entity->timer -= deltaT;
			} else{
				switch (entity->ID){
					case playerBulletID:
						entity->alive = 0;
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
			if (entity->y >= LCD_WIDTH_PY){
				entity->y -= LCD_WIDTH_PY;
			} else if (entity->y < 0){
				entity->y += LCD_WIDTH_PY;
			}
			
			switch (entity->ID){
				case playerBulletID:
					for (j=1; j<numEntities; j++){
						entity2 = entities[j];
						x1 = sizes[entity->ID]+sizes[entity2->ID];
						if (entity2->ID >= smallAsteroidID && fabs(entity2->x-entity->x)<x1 && fabs(entity2->y-entity->y)<x1){
							entity->alive = 0;
							entity2->alive = 0;
							break;
						}
					}
					break;
				
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
				case mediumAsteroidID:
					createObject(
						entity->x,
						entity->y,
						entity->vx * asteroidSplitCos + entity->vy * asteroidSplitSin,
						entity->vy * asteroidSplitCos - entity->vx * asteroidSplitSin,
						entity->rot + 0.5,
						entity->vrot * -asteroidSplitVRot,
						&smallAsteroidShape,
						smallAsteroidID,
						0.0
					);
					createObject(
						entity->x,
						entity->y,
						entity->vx * asteroidSplitCos - entity->vy * asteroidSplitSin,
						entity->vy * asteroidSplitCos + entity->vx * asteroidSplitSin,
						-entity->rot - 1.7,
						entity->vrot * asteroidSplitVRot,
						&smallAsteroidShape,
						smallAsteroidID,
						0.0
					);
					break;
				case largeAsteroidID:
					createObject(
						entity->x,
						entity->y,
						entity->vx * asteroidSplitCos + entity->vy * asteroidSplitSin,
						entity->vy * asteroidSplitCos - entity->vx * asteroidSplitSin,
						entity->rot + 1.5,
						entity->vrot * -asteroidSplitVRot,
						&mediumAsteroidShape,
						mediumAsteroidID,
						0.0
					);
					createObject(
						entity->x,
						entity->y,
						entity->vx * asteroidSplitCos - entity->vy * asteroidSplitSin,
						entity->vy * asteroidSplitCos + entity->vx * asteroidSplitSin,
						-entity->rot - 0.4,
						entity->vrot * asteroidSplitVRot,
						&mediumAsteroidShape,
						mediumAsteroidID,
						0.0
					);
					break;
				}
				deleteEntity(i);
			}
		}
		
		for (i = 0; i<(int)(deltaT*1000.0); i++){
			drawLine((i/5)*15,(i%5)*5+2,10+(i/5)*15,(i%5)*5+2);	
		}
		//drawLine(0,10,deltaT*15.0*((float)LCD_WIDTH_PX),10);
		
        
		
		
        //Print_OS("Press EXE to exit", 0, 0);

        
        Bdisp_PutDisp_DD();
		
		
		
		
    }
 
    return 0;
}
