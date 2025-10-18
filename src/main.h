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


const int32_t playerShape[4][2] = {{8, 0}, {-8,5}, {-2,0}, {-8,-5}};
const int32_t playerBulletShape[2][2] = {{5, 0},{-5, 0}};
const int32_t smallAsteroidShape[4][2] = {{5, 0},{0, 5},{-5,0},{0,-5}};
const int32_t mediumAsteroidShape[4][2] = {{8, 0},{0, 8},{-8,0},{0,-8}};
const int32_t largeAsteroidShape[4][2] = {{15, 0},{0, 15},{-15,0},{0,-15}};
const int32_t smallSaucerShape[10][2] = {{6, 0},{4, 2},{-4,2},{-6,0},{-4,-2},{2,-2},{2,-6},{-2,-6},{-2,-2},{4,-2}};
const int32_t largeSaucerShape[10][2] = {{12, 0},{9, 3},{-9,3},{-12,0},{-9,-3},{6,-3},{6,-9},{-6,-9},{-6,-3},{9,-3}};
const int32_t saucerBulletShape[4][2] = {{1,0},{0,1},{-1,0},{0,-1}};

const unsigned char lengths[entityCount] = {4,2,4,4,4,10,10,4};
const unsigned char sizes[entityCount] = {5,3,4,6,12,6,9,1};

const int32_t iconPlayerShape[4][4] = {{-8, 0, 8, -5},{8, -5, 2, 0},{2, 0, 8, 5},{8, 5, -8, 0}};

const int32_t iconNum0[4][4] = {{ssdVT,ssdHL,ssdVT,ssdHR},{ssdVT,ssdHR,ssdVB,ssdHR},{ssdVB,ssdHR,ssdVB,ssdHL},{ssdVB,ssdHL,ssdVT,ssdHL}};
const int32_t iconNum1[2][4] = {{ssdVT,ssdHM,ssdVB,ssdHM},{ssdVT,ssdHM,ssdVTM,ssdHL}};
const int32_t iconNum2[5][4] = {{ssdVT,ssdHL,ssdVT,ssdHR},{ssdVT,ssdHR,ssdVM,ssdHR},{ssdVM,ssdHR,ssdVM,ssdHL},{ssdVM,ssdHL,ssdVB,ssdHL},{ssdVB,ssdHL,ssdVB,ssdHR}};
const int32_t iconNum3[4][4] = {{ssdVT,ssdHL,ssdVT,ssdHR},{ssdVM,ssdHR,ssdVM,ssdHL},{ssdVB,ssdHL,ssdVB,ssdHR},{ssdVT,ssdHR,ssdVB,ssdHR}};
const int32_t iconNum4[3][4] = {{ssdVT,ssdHL,ssdVM,ssdHL},{ssdVM,ssdHR,ssdVM,ssdHL},{ssdVT,ssdHR,ssdVB,ssdHR}};
const int32_t iconNum5[5][4] = {{ssdVT,ssdHL,ssdVT,ssdHR},{ssdVT,ssdHL,ssdVM,ssdHL},{ssdVM,ssdHR,ssdVM,ssdHL},{ssdVM,ssdHR,ssdVB,ssdHR},{ssdVB,ssdHL,ssdVB,ssdHR}};
const int32_t iconNum6[4][4] = {{ssdVB,ssdHL,ssdVT,ssdHL},{ssdVM,ssdHR,ssdVM,ssdHL},{ssdVM,ssdHR,ssdVB,ssdHR},{ssdVB,ssdHL,ssdVB,ssdHR}};
const int32_t iconNum7[2][4] = {{ssdVT,ssdHL,ssdVT,ssdHR},{ssdVT,ssdHR,ssdVB,ssdHL}};
const int32_t iconNum8[5][4] = {{ssdVT,ssdHL,ssdVT,ssdHR},{ssdVT,ssdHR,ssdVB,ssdHR},{ssdVB,ssdHR,ssdVB,ssdHL},{ssdVB,ssdHL,ssdVT,ssdHL},{ssdVM,ssdHL,ssdVM,ssdHR}};
const int32_t iconNum9[4][4] = {{ssdVT,ssdHL,ssdVT,ssdHR},{ssdVT,ssdHR,ssdVB,ssdHR},{ssdVM,ssdHR,ssdVM,ssdHL},{ssdVM,ssdHL,ssdVT,ssdHL}};

const int32_t (*iconNums[10])[][4] = {&iconNum0,&iconNum1,&iconNum2,&iconNum3,&iconNum4,&iconNum5,&iconNum6,&iconNum7,&iconNum8,&iconNum9};
const unsigned char iconNumLengths[10] = {4,2,5,4,3,5,4,2,5,4};

const int32_t iconLetA[4][4] = {{ssdVT,ssdHL,ssdVT,ssdHR},{ssdVT,ssdHR,ssdVB,ssdHR},{ssdVM,ssdHR,ssdVM,ssdHL},{ssdVB,ssdHL,ssdVT,ssdHL}};
const int32_t iconLetB[8][4] = {{ssdVT,ssdHL,ssdVT,ssdHM},{ssdVM,ssdHL,ssdVM,ssdHM},{ssdVB,ssdHL,ssdVB,ssdHM},{ssdVT,ssdHL,ssdVB,ssdHL},{ssdVT,ssdHM,ssdVTM,ssdHR},{ssdVM,ssdHM,ssdVTM,ssdHR},{ssdVM,ssdHM,ssdVBM,ssdHR},{ssdVB,ssdHM,ssdVBM,ssdHR}};
const int32_t iconLetC[3][4] = {{ssdVT,ssdHL,ssdVT,ssdHR},{ssdVB,ssdHL,ssdVB,ssdHR},{ssdVT,ssdHL,ssdVB,ssdHL}};
const int32_t iconLetD[6][4] = {{ssdVT,ssdHL,ssdVT,ssdHM},{ssdVB,ssdHL,ssdVB,ssdHM},{ssdVT,ssdHL,ssdVB,ssdHL},{ssdVT,ssdHM,ssdVTM,ssdHR},{ssdVB,ssdHM,ssdVBM,ssdHR},{ssdVTM,ssdHR,ssdVBM,ssdHR}};
const int32_t iconLetE[4][4] = {{ssdVT,ssdHL,ssdVT,ssdHR},{ssdVM,ssdHL,ssdVM,ssdHR},{ssdVB,ssdHL,ssdVB,ssdHR},{ssdVT,ssdHL,ssdVB,ssdHL}};
const int32_t iconLetF[3][4] = {{ssdVT,ssdHL,ssdVT,ssdHR},{ssdVM,ssdHL,ssdVM,ssdHR},{ssdVT,ssdHL,ssdVB,ssdHL}};
const int32_t iconLetG[5][4] = {{ssdVT,ssdHL,ssdVT,ssdHR},{ssdVB,ssdHL,ssdVB,ssdHR},{ssdVM,ssdHM,ssdVM,ssdHR},{ssdVT,ssdHL,ssdVB,ssdHL},{ssdVM,ssdHR,ssdVB,ssdHR}};
const int32_t iconLetH[3][4] = {{ssdVT,ssdHR,ssdVB,ssdHR},{ssdVM,ssdHR,ssdVM,ssdHL},{ssdVB,ssdHL,ssdVT,ssdHL}};
const int32_t iconLetI[3][4] = {{ssdVT,ssdHL,ssdVT,ssdHR},{ssdVB,ssdHL,ssdVB,ssdHR},{ssdVT,ssdHM,ssdVB,ssdHM}};
const int32_t iconLetJ[4][4] = {{ssdVT,ssdHL,ssdVT,ssdHR},{ssdVM,ssdHL,ssdVB,ssdHM},{ssdVB,ssdHM,ssdVB,ssdHR},{ssdVT,ssdHR,ssdVB,ssdHR}};
const int32_t iconLetK[3][4] = {{ssdVM,ssdHL,ssdVT,ssdHR},{ssdVM,ssdHL,ssdVB,ssdHR},{ssdVT,ssdHL,ssdVB,ssdHL}};
const int32_t iconLetL[2][4] = {{ssdVB,ssdHL,ssdVB,ssdHR},{ssdVT,ssdHL,ssdVB,ssdHL}};
const int32_t iconLetM[4][4] = {{ssdVT,ssdHR,ssdVB,ssdHR},{ssdVT,ssdHL,ssdVM,ssdHM},{ssdVM,ssdHM,ssdVT,ssdHR},{ssdVB,ssdHL,ssdVT,ssdHL}};
const int32_t iconLetN[3][4] = {{ssdVT,ssdHR,ssdVB,ssdHR},{ssdVB,ssdHR,ssdVT,ssdHL},{ssdVB,ssdHL,ssdVT,ssdHL}};
const int32_t iconLetO[4][4] = {{ssdVT,ssdHL,ssdVT,ssdHR},{ssdVT,ssdHR,ssdVB,ssdHR},{ssdVB,ssdHR,ssdVB,ssdHL},{ssdVB,ssdHL,ssdVT,ssdHL}};
const int32_t iconLetP[4][4] = {{ssdVT,ssdHL,ssdVT,ssdHR},{ssdVT,ssdHR,ssdVM,ssdHR},{ssdVM,ssdHR,ssdVM,ssdHL},{ssdVB,ssdHL,ssdVT,ssdHL}};
const int32_t iconLetQ[6][4] = {{ssdVT,ssdHL,ssdVT,ssdHR},{ssdVT,ssdHR,ssdVM,ssdHR},{ssdVM,ssdHR,ssdVB,ssdHM},{ssdVB,ssdHM,ssdVB,ssdHL},{ssdVB,ssdHL,ssdVT,ssdHL},{ssdVM,ssdHM,ssdVB,ssdHR}};
const int32_t iconLetR[5][4] = {{ssdVT,ssdHL,ssdVT,ssdHR},{ssdVT,ssdHR,ssdVM,ssdHR},{ssdVM,ssdHR,ssdVM,ssdHL},{ssdVB,ssdHL,ssdVT,ssdHL},{ssdVB,ssdHR,ssdVM,ssdHL}};
const int32_t iconLetS[5][4] = {{ssdVT,ssdHL,ssdVT,ssdHR},{ssdVT,ssdHL,ssdVM,ssdHL},{ssdVM,ssdHR,ssdVM,ssdHL},{ssdVM,ssdHR,ssdVB,ssdHR},{ssdVB,ssdHL,ssdVB,ssdHR}};
const int32_t iconLetT[2][4] = {{ssdVT,ssdHL,ssdVT,ssdHR},{ssdVT,ssdVM,ssdVB,ssdHM}};
const int32_t iconLetU[3][4] = {{ssdVT,ssdHR,ssdVB,ssdHR},{ssdVB,ssdHR,ssdVB,ssdHL},{ssdVB,ssdHL,ssdVT,ssdHL}};
const int32_t iconLetV[2][4] = {{ssdVT,ssdHL,ssdVB,ssdHM},{ssdVB,ssdHM,ssdVT,ssdHR}};
const int32_t iconLetW[4][4] = {{ssdVT,ssdHL,ssdVB,ssdHLM},{ssdVB,ssdHLM,ssdVT,ssdHM},{ssdVT,ssdHM,ssdVB,ssdHRM},{ssdVB,ssdHRM,ssdVT,ssdHR}};
const int32_t iconLetX[2][4] = {{ssdVT,ssdHL,ssdVB,ssdHR},{ssdVB,ssdHL,ssdVT,ssdHR}};
const int32_t iconLetY[3][4] = {{ssdVT,ssdHL,ssdVTM,ssdHM},{ssdVTM,ssdHM,ssdVT,ssdHR},{ssdVTM,ssdHM,ssdVB,ssdHM}};
const int32_t iconLetZ[3][4] = {{ssdVB,ssdHL,ssdVT,ssdHR},{ssdVT,ssdHL,ssdVT,ssdHR},{ssdVB,ssdHL,ssdVB,ssdHR}};

const int32_t (*iconLets[26])[][4] = {&iconLetA,&iconLetB,&iconLetC,&iconLetD,&iconLetE,&iconLetF,&iconLetG,&iconLetH,&iconLetI,&iconLetJ,&iconLetK,&iconLetL,&iconLetM,
	&iconLetN,&iconLetO,&iconLetP,&iconLetQ,&iconLetR,&iconLetS,&iconLetT,&iconLetU,&iconLetV,&iconLetW,&iconLetX,&iconLetY,&iconLetZ};
const unsigned char iconLetLengths[26] = {4,8,3,6,4,3,5,3,3,4,3,2,4,3,4,4,6,5,5,2,3,2,4,2,3,3};

const int LCD_PIXELS = DWIDTH*DHEIGHT;

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
	int32_t x, y, vx, vy, rot, vrot;
	const int32_t (*shape)[][2];
	unsigned char ID;
	int32_t timer;
	unsigned char alive;
	unsigned char killer;
} object;