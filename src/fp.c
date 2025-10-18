#define FPOffset 14
#define FPOne (1<<FPOffset)

int32_t intToFP(int32_t num1){
	return num1 << FPOffset; // shifts appear to be arithmetic, so this works fine for negative numbers
}

int32_t floatToFP(float num1){
	return num1 * FPOne;
}

int32_t FPToInt(int32_t num1){
	return num1 >> FPOffset;
}


int32_t FPDiv(int64_t num1, int64_t num2){ // most accurate but slower
	return (num1 << FPOffset) / num2;
}

int32_t FPDivLower(int32_t num1, int32_t num2){ // keeps lower bits
	return (num1 << FPOffset) / num2;
}

int32_t FPDivUpper(int32_t num1, int32_t num2){ // keeps upper bits
	return (num1 / num2) << FPOffset;
}


int32_t FPMult(int64_t num1, int64_t num2){ // most accurate but slower
	return (num1 * num2) >> FPOffset;
}

int32_t FPMultLower(int32_t num1, int32_t num2){ // keeps lower bits
	return (num1 * num2) >> FPOffset;
}

int32_t FPMultUpper(int32_t num1, int32_t num2){ // keeps upper bits
	return (num1 >> FPOffset) * num2;
}