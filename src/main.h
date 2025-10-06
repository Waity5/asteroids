#define playerID 0
#define playerBulletID 1
#define smallAsteroidID 2
#define mediumAsteroidID 3
#define largeAsteroidID 4


const float playerShape[4][2] = {{8, 0}, {-8,5}, {-2,0}, {-8,-5}};
const float playerBulletShape[2][2] = {{5, 0},{-5, 0}};
const float smallAsteroidShape[4][2] = {{5, 0},{0, 5},{-5,0},{0,-5}};
const float mediumAsteroidShape[4][2] = {{8, 0},{0, 8},{-8,0},{0,-8}};
const float largeAsteroidShape[4][2] = {{15, 0},{0, 15},{-15,0},{0,-15}};


const unsigned char lengths[] = {4,2,4,4,4};
const unsigned char sizes[] = {6,3,4,6,12};
const int LCD_WIDTH_PY = 216;
const int LCD_PIXELS = LCD_WIDTH_PX*LCD_WIDTH_PY;


typedef struct{
	float x, y, vx, vy, rot, vrot;
	const float (*shape)[][2];
	unsigned char ID;
	float timer;
	unsigned char alive;
} object;