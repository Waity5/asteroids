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
#define ssdVTM -4
#define ssdVM 0
#define ssdVBM 4
#define ssdVB 8

#define ssdHL -5
#define ssdHLM -3
#define ssdHM 0
#define ssdHRM 3
#define ssdHR 5

#define gameMenu 0
#define gamePlay 1
#define gameStart 2
#define gameEnd 3

#define gamemodeNormal 0
#define gamemodeAsteroids 1

#define DWIDTH 396
#define LCD_HEIGHT_PX 224 


const float playerShape[4][2] = {{8, 0}, {-8,5}, {-2,0}, {-8,-5}};
const float playerBulletShape[2][2] = {{5, 0},{-5, 0}};
const float smallAsteroidShape[4][2] = {{5, 0},{0, 5},{-5,0},{0,-5}};
const float mediumAsteroidShape[4][2] = {{8, 0},{0, 8},{-8,0},{0,-8}};
const float largeAsteroidShape[4][2] = {{15, 0},{0, 15},{-15,0},{0,-15}};
const float smallSaucerShape[10][2] = {{6, 0},{4, 2},{-4,2},{-6,0},{-4,-2},{2,-2},{2,-6},{-2,-6},{-2,-2},{4,-2}};
const float largeSaucerShape[10][2] = {{12, 0},{9, 3},{-9,3},{-12,0},{-9,-3},{6,-3},{6,-9},{-6,-9},{-6,-3},{9,-3}};
const float saucerBulletShape[4][2] = {{1,0},{0,1},{-1,0},{0,-1}};

const unsigned char lengths[entityCount] = {4,2,4,4,4,10,10,4};
const unsigned char sizes[entityCount] = {6,3,4,6,12,6,9,1};

const float iconPlayerShape[4][4] = {{-8, 0, 8, -5},{8, -5, 2, 0},{2, 0, 8, 5},{8, 5, -8, 0}};

const float iconNum0[4][4] = {{ssdVT,ssdHL,ssdVT,ssdHR},{ssdVT,ssdHR,ssdVB,ssdHR},{ssdVB,ssdHR,ssdVB,ssdHL},{ssdVB,ssdHL,ssdVT,ssdHL}};
const float iconNum1[2][4] = {{ssdVT,ssdHM,ssdVB,ssdHM},{ssdVT,ssdHM,ssdVTM,ssdHL}};
const float iconNum2[5][4] = {{ssdVT,ssdHL,ssdVT,ssdHR},{ssdVT,ssdHR,ssdVM,ssdHR},{ssdVM,ssdHR,ssdVM,ssdHL},{ssdVM,ssdHL,ssdVB,ssdHL},{ssdVB,ssdHL,ssdVB,ssdHR}};
const float iconNum3[4][4] = {{ssdVT,ssdHL,ssdVT,ssdHR},{ssdVM,ssdHR,ssdVM,ssdHL},{ssdVB,ssdHL,ssdVB,ssdHR},{ssdVT,ssdHR,ssdVB,ssdHR}};
const float iconNum4[3][4] = {{ssdVT,ssdHL,ssdVM,ssdHL},{ssdVM,ssdHR,ssdVM,ssdHL},{ssdVT,ssdHR,ssdVB,ssdHR}};
const float iconNum5[5][4] = {{ssdVT,ssdHL,ssdVT,ssdHR},{ssdVT,ssdHL,ssdVM,ssdHL},{ssdVM,ssdHR,ssdVM,ssdHL},{ssdVM,ssdHR,ssdVB,ssdHR},{ssdVB,ssdHL,ssdVB,ssdHR}};
const float iconNum6[4][4] = {{ssdVB,ssdHL,ssdVT,ssdHL},{ssdVM,ssdHR,ssdVM,ssdHL},{ssdVM,ssdHR,ssdVB,ssdHR},{ssdVB,ssdHL,ssdVB,ssdHR}};
const float iconNum7[3][4] = {{ssdVT,ssdHL,ssdVT,ssdHR},{ssdVT,ssdHR,ssdVB,ssdHR}};
const float iconNum8[5][4] = {{ssdVT,ssdHL,ssdVT,ssdHR},{ssdVT,ssdHR,ssdVB,ssdHR},{ssdVB,ssdHR,ssdVB,ssdHL},{ssdVB,ssdHL,ssdVT,ssdHL},{ssdVM,ssdHL,ssdVM,ssdHR}};
const float iconNum9[4][4] = {{ssdVT,ssdHL,ssdVT,ssdHR},{ssdVT,ssdHR,ssdVB,ssdHR},{ssdVM,ssdHR,ssdVM,ssdHL},{ssdVM,ssdHL,ssdVT,ssdHL}};

const float (*iconNums[10])[][4] = {&iconNum0,&iconNum1,&iconNum2,&iconNum3,&iconNum4,&iconNum5,&iconNum6,&iconNum7,&iconNum8,&iconNum9};
const unsigned char iconNumLengths[10] = {4,2,5,4,3,5,4,3,5,4};

const float iconLetA[4][4] = {{ssdVT,ssdHL,ssdVT,ssdHR},{ssdVT,ssdHR,ssdVB,ssdHR},{ssdVM,ssdHR,ssdVM,ssdHL},{ssdVB,ssdHL,ssdVT,ssdHL}};
const float iconLetB[8][4] = {{ssdVT,ssdHL,ssdVT,ssdHM},{ssdVM,ssdHL,ssdVM,ssdHM},{ssdVB,ssdHL,ssdVB,ssdHM},{ssdVT,ssdHL,ssdVB,ssdHL},{ssdVT,ssdHM,ssdVTM,ssdHR},{ssdVM,ssdHM,ssdVTM,ssdHR},{ssdVM,ssdHM,ssdVBM,ssdHR},{ssdVB,ssdHM,ssdVBM,ssdHR}};
const float iconLetC[3][4] = {{ssdVT,ssdHL,ssdVT,ssdHR},{ssdVB,ssdHL,ssdVB,ssdHR},{ssdVT,ssdHL,ssdVB,ssdHL}};
const float iconLetD[6][4] = {{ssdVT,ssdHL,ssdVT,ssdHM},{ssdVB,ssdHL,ssdVB,ssdHM},{ssdVT,ssdHL,ssdVB,ssdHL},{ssdVT,ssdHM,ssdVTM,ssdHR},{ssdVB,ssdHM,ssdVBM,ssdHR},{ssdVTM,ssdHR,ssdVBM,ssdHR}};
const float iconLetE[4][4] = {{ssdVT,ssdHL,ssdVT,ssdHR},{ssdVM,ssdHL,ssdVM,ssdHR},{ssdVB,ssdHL,ssdVB,ssdHR},{ssdVT,ssdHL,ssdVB,ssdHL}};
const float iconLetF[3][4] = {{ssdVT,ssdHL,ssdVT,ssdHR},{ssdVM,ssdHL,ssdVM,ssdHR},{ssdVT,ssdHL,ssdVB,ssdHL}};
const float iconLetG[5][4] = {{ssdVT,ssdHL,ssdVT,ssdHR},{ssdVB,ssdHL,ssdVB,ssdHR},{ssdVM,ssdHM,ssdVM,ssdHR},{ssdVT,ssdHL,ssdVB,ssdHL},{ssdVM,ssdHR,ssdVB,ssdHR}};
const float iconLetH[3][4] = {{ssdVT,ssdHR,ssdVB,ssdHR},{ssdVM,ssdHR,ssdVM,ssdHL},{ssdVB,ssdHL,ssdVT,ssdHL}};
const float iconLetI[3][4] = {{ssdVT,ssdHL,ssdVT,ssdHR},{ssdVB,ssdHL,ssdVB,ssdHR},{ssdVT,ssdHM,ssdVB,ssdHM}};
const float iconLetJ[4][4] = {{ssdVT,ssdHL,ssdVT,ssdHR},{ssdVM,ssdHL,ssdVB,ssdHM},{ssdVB,ssdHM,ssdVB,ssdHR},{ssdVT,ssdHR,ssdVB,ssdHR}};
const float iconLetK[3][4] = {{ssdVM,ssdHL,ssdVT,ssdHR},{ssdVM,ssdHL,ssdVB,ssdHR},{ssdVT,ssdHL,ssdVB,ssdHL}};
const float iconLetL[2][4] = {{ssdVB,ssdHL,ssdVB,ssdHR},{ssdVT,ssdHL,ssdVB,ssdHL}};
const float iconLetM[4][4] = {{ssdVT,ssdHR,ssdVB,ssdHR},{ssdVT,ssdHL,ssdVM,ssdHM},{ssdVM,ssdHM,ssdVT,ssdHR},{ssdVB,ssdHL,ssdVT,ssdHL}};
const float iconLetN[3][4] = {{ssdVT,ssdHR,ssdVB,ssdHR},{ssdVB,ssdHR,ssdVT,ssdHL},{ssdVB,ssdHL,ssdVT,ssdHL}};
const float iconLetO[4][4] = {{ssdVT,ssdHL,ssdVT,ssdHR},{ssdVT,ssdHR,ssdVB,ssdHR},{ssdVB,ssdHR,ssdVB,ssdHL},{ssdVB,ssdHL,ssdVT,ssdHL}};
const float iconLetP[4][4] = {{ssdVT,ssdHL,ssdVT,ssdHR},{ssdVT,ssdHR,ssdVM,ssdHR},{ssdVM,ssdHR,ssdVM,ssdHL},{ssdVB,ssdHL,ssdVT,ssdHL}};
const float iconLetQ[6][4] = {{ssdVT,ssdHL,ssdVT,ssdHR},{ssdVT,ssdHR,ssdVM,ssdHR},{ssdVM,ssdHR,ssdVB,ssdHM},{ssdVB,ssdHM,ssdVB,ssdHL},{ssdVB,ssdHL,ssdVT,ssdHL},{ssdVM,ssdHM,ssdVB,ssdHR}};
const float iconLetR[5][4] = {{ssdVT,ssdHL,ssdVT,ssdHR},{ssdVT,ssdHR,ssdVM,ssdHR},{ssdVM,ssdHR,ssdVM,ssdHL},{ssdVB,ssdHL,ssdVT,ssdHL},{ssdVB,ssdHR,ssdVM,ssdHL}};
const float iconLetS[5][4] = {{ssdVT,ssdHL,ssdVT,ssdHR},{ssdVT,ssdHL,ssdVM,ssdHL},{ssdVM,ssdHR,ssdVM,ssdHL},{ssdVM,ssdHR,ssdVB,ssdHR},{ssdVB,ssdHL,ssdVB,ssdHR}};
const float iconLetT[2][4] = {{ssdVT,ssdHL,ssdVT,ssdHR},{ssdVT,ssdVM,ssdVB,ssdHM}};
const float iconLetU[3][4] = {{ssdVT,ssdHR,ssdVB,ssdHR},{ssdVB,ssdHR,ssdVB,ssdHL},{ssdVB,ssdHL,ssdVT,ssdHL}};
const float iconLetV[2][4] = {{ssdVT,ssdHL,ssdVB,ssdHM},{ssdVB,ssdHM,ssdVT,ssdHR}};
const float iconLetW[4][4] = {{ssdVT,ssdHL,ssdVB,ssdHLM},{ssdVB,ssdHLM,ssdVT,ssdHM},{ssdVT,ssdHM,ssdVB,ssdHRM},{ssdVB,ssdHRM,ssdVT,ssdHR}};
const float iconLetX[2][4] = {{ssdVT,ssdHL,ssdVB,ssdHR},{ssdVB,ssdHL,ssdVT,ssdHR}};
const float iconLetY[3][4] = {{ssdVT,ssdHL,ssdVTM,ssdHM},{ssdVTM,ssdHM,ssdVT,ssdHR},{ssdVTM,ssdHM,ssdVB,ssdHM}};
const float iconLetZ[3][4] = {{ssdVB,ssdHL,ssdVT,ssdHR},{ssdVT,ssdHL,ssdVT,ssdHR},{ssdVB,ssdHL,ssdVB,ssdHR}};

const float (*iconLets[26])[][4] = {&iconLetA,&iconLetB,&iconLetC,&iconLetD,&iconLetE,&iconLetF,&iconLetG,&iconLetH,&iconLetI,&iconLetJ,&iconLetK,&iconLetL,&iconLetM,
	&iconLetN,&iconLetO,&iconLetP,&iconLetQ,&iconLetR,&iconLetS,&iconLetT,&iconLetU,&iconLetV,&iconLetW,&iconLetX,&iconLetY,&iconLetZ};
const unsigned char iconLetLengths[26] = {4,8,3,6,4,3,5,3,3,4,3,2,4,3,4,4,6,5,5,2,3,2,4,2,3,3};

const int LCD_PIXELS = DWIDTH*LCD_HEIGHT_PX;

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