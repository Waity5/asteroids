#define playerID 0
#define playerBulletID 1
#define smallAsteroidID 2
#define mediumAsteroidID 3
#define largeAsteroidID 4
#define smallSaucerID 5
#define largeSaucerID 6
#define saucerBulletID 7
#define entityCount 8

#define ssdVT -8
#define ssdVM 0
#define ssdVB 8
#define ssdHL -5
#define ssdHM 0
#define ssdHR 5


const float playerShape[4][2] = {{8, 0}, {-8,5}, {-2,0}, {-8,-5}};
const float playerBulletShape[2][2] = {{5, 0},{-5, 0}};
const float smallAsteroidShape[4][2] = {{5, 0},{0, 5},{-5,0},{0,-5}};
const float mediumAsteroidShape[4][2] = {{8, 0},{0, 8},{-8,0},{0,-8}};
const float largeAsteroidShape[4][2] = {{15, 0},{0, 15},{-15,0},{0,-15}};
const float smallSaucerShape[6][2] = {{8, 0},{6, 2},{-6,2},{-8,0},{-6,-2},{6,-2}};
const float largeSaucerShape[6][2] = {{12, 0},{9, 3},{-9,3},{-12,0},{-9,-3},{9,-3}};
const float saucerBulletShape[2][2] = {{3,0},{-3,0}};

const unsigned char lengths[entityCount] = {4,2,4,4,4,6,6,2};
const unsigned char sizes[entityCount] = {6,3,4,6,12,6,9,1};

const float iconPlayerShape[4][2] = {{0, 8},{5, -8},{0,-2},{-5,-8}};






const int LCD_PIXELS = LCD_WIDTH_PX*LCD_HEIGHT_PX;

unsigned char collisionTable[entityCount][entityCount] = {
	{0,0,1,1,1,1,1,1},
	{0,0,1,1,1,1,1,0},
	{1,1,0,0,0,1,1,1},
	{1,1,0,0,0,1,1,1},
	{1,1,0,0,0,1,1,1},
	{1,1,1,1,1,0,0,0},
	{1,1,1,1,1,0,0,0},
	{1,0,1,1,1,0,0,0},
};



typedef struct{
	float x, y, vx, vy, rot, vrot;
	const float (*shape)[][2];
	unsigned char ID;
	float timer;
	unsigned char alive;
	unsigned char killer;
} object;