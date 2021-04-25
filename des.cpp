/*DES 구현*/
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define SIZE 64//Block Size

void bitExtraction(char* plainText, int* plainBit);//bit로 분해
void viewArray(int* arr, int h, int w);//배열 확인
void tableSwap(const char* file, int* arr, int h, int w);//table에 따라서 Swap하기
void liner(int* arr, int arrSize);//일렬로 나열하기
void initialPermutation(char* plainText, int* plainBit);//start
void keyGenerator(char* keyText, int key[][48]);//key 생성
void keyChecking(int key[][48]);//round별 이진수와 키 확인
void myXor(int* arrA, int* arrB, int size);//두 배열의 xor연산
void copyArray(int* destination, int* source, int size);//배열 복사
void roundFunction(int* arr_bit32, int* key);//round 함수 정의
void round(int* plainBit, int key[][48], int* cryptoBit);//round
void bitToString(int* bit, int size);//bit를 넣으면 문자열로 바꿔줌
void block(char* plainTextBlock, char* keyTextBlock);//한 프로세스 블럭
void keyOptimization(char* keyText, char* keyTextBlock, int keySize);//key 최적화

void tempBitExtraction(int* keyBit);//이진수 분해 야매 용
void binaryToHex(int *, int , int);//2진수를 16진수로 바꿔주기

void binaryToHex(int* arr, int h, int w)
{
	int i, j, sum;

	printf("\n");
	for (i = 0; i < h; i++) {
		sum = 0;

		for (j = 0; j < w; j++)
			sum += arr[i * 8 + j] * (int)pow(2, 7 - j);

		printf("%.2X", sum);
	}
	printf("\n");
}

void keyOptimization(char* keyText, char* keyTextBlock, int keySize) {

	strncpy(keyTextBlock, keyText, 8);
	keyTextBlock[8] = NULL;
}

void textOptimization() {

}

int main(void) {

	char plainText[1000], keyText[100];
	char plainTextBlock[9], keyTextBlock[9] = { 0 };

	/*printf("평문을 입력하세요 :");
	gets_s(plainText);
	printf("key 를 입력하세요 :");
	gets_s(keyText);*/

	//////key padding or cutoff//////
	//keyOptimization(keyText, keyTextBlock, strlen(keyText));

	//////text padding or cutoff or though/////
	//textOptimization();

	//////send plainTextBlock and keyTextBlock//////
	block(plainTextBlock, keyTextBlock);//Non effect

	return 0;
}

void block(char* plainTextBlock, char* keyTextBlock) {

	char plainText[9] = {0x01, 0x10, 0x10, 0x23, 0x41, 0x10, 0x23}, keyText[9];//char의 max인 127(CF) 초과가 없으므로 unsigned 하지 않음
	//char plainText[9] = "12345678", keyText[9]= "ABCDEFGH";
	int plainBit[SIZE], key[16][48], cryptoBit[64]; 

	//////initialPermutation//////
	initialPermutation(plainText, plainBit);

	//////Key generator//////
	keyGenerator(keyText, key);

	/////round//////
	round(plainBit, key, cryptoBit);

	/////finalPermutation//////
	tableSwap("finalPermutation", cryptoBit, 8, 8);
	viewArray(cryptoBit, 8, 8);
	bitToString(cryptoBit, 64);
	binaryToHex(cryptoBit, 8, 8);

}

void bitExtraction(char* plainText, int* plainBit) {

	int i, j;

	for (i = 0; i < 8; i++)
		for (j = 0; j < 8; j++)
			plainBit[i * 8 + j] = (plainText[i] & (int)pow(2, 7 - j)) / (int)pow(2, 7 - j);
}

void viewArray(int* arr, int h, int w) {

	int i;
	for (i = 0; i < h * w; i++) {
		printf("%2d ", arr[i]);

		if ((i + 1) % w == 0)
			printf("\n");
	}
}

void tableSwap(const char* file, int* arr, int h, int w) {

	char fileName[50] = "table\\";
	int* table, * tempArr;
	int i;
	FILE* fp;

	//////문자열 조합//////
	strcat(fileName, file);
	strcat(fileName, ".txt");

	//printf("%s\n", fileName);

	///////값 넣기///////
	table = (int*)malloc(sizeof(int) * h * w);//table배열 만들기

	fp = fopen(fileName, "r");

	for (i = 0; !feof(fp); i++)
		fscanf_s(fp, "%d", &table[i]);//malloc이여서 doublePointer

	fclose(fp);//파일 스트림 닫기

	//////bit swap//////
	tempArr = (int*)malloc(sizeof(int) * h * w);//값을 넘겨줄 배열

	for (i = 0; i < h * w; i++)//배열 임시저장
		tempArr[i] = arr[table[i] - 1];

	for (i = 0; i < h * w; i++)//배열 변환
		arr[i] = tempArr[i];

	free(table);//동적할당 제거
}

void liner(int* arr, int arrSize) {

	int i;
	//for (i = 0; i < arrSize / sizeof(int); i++)
	for (i = 0; i < arrSize ; i++)
		arr[i] = i + 1;
}

void initialPermutation(char* plainText, int* plainBit)
{
	bitExtraction(plainText, plainBit);//이진수분해(전, 후)
	//printf("Hex to binary\n");
	//viewArray(plainBit, 8, 8);
	binaryToHex(plainBit, 8, 8);

	tableSwap("initialPermutation", plainBit, 8, 8);//swap(파일이름, 기존, 테이블 행/ 열)
	//printf("\nInitialPermutation\n");
	//viewArray(plainBit, 8, 8);
	binaryToHex(plainBit, 8, 8);
}

void tempBitExtraction(int* keyBit)
{
	int i, j;
	unsigned char plainKey[9] = { 0x01, 0x23, 0xAB, 0xCD, 0x25, 0x62, 0x14, 0x56 };

	for (i = 0; i < 8; i++)
		for (j = 0; j < 8; j++)
			keyBit[i * 8 + j] = (plainKey[i] & (int)pow(2, 7 - j)) / (int)pow(2, 7 - j);
}

void keyGenerator(char* keyText, int key[][48]) {
	int keyBit[SIZE], shift[16];
	int arr_bit56[56], arr_bit48[48], arr_bit28[2][28], arr_tempBit28[28];
	int i, j, round;

	//////Step1) Parity Drop//////
	//bitExtraction(keyText, keyBit);//이진수분해(전, 후)
	tempBitExtraction(keyBit);
	tableSwap("parityDrop", keyBit, 7, 8);//swap(파일이름, 기존, 테이블 행/ 열)

	for (i = 0; i < 56; i++)//8배수 버려서 56bit 만들기
		arr_bit56[i] = keyBit[i];
	
	//////Step2)divide//////
	for (i = 0; i < 2; i++)
		for (j = 0; j < 28; j++)
			arr_bit28[i][j] = arr_bit56[i * 28 + j];

	//////Step3-1) shift Rule//////
	for (i = 0; i < 16; i++)//shift rule적용
	{
		if (i == 0 || i == 1 || i == 8 || i == 15)
			shift[i] = 1;

		else
			shift[i] = 2;
	}

	for (round = 0; round < 16; round++) {

		//////Step3-2)shifting//////
		for (i = 0; i < 2; i++) {
			for (j = 0; j < 28; j++)//순환 이동 임시저장
				arr_tempBit28[j] = arr_bit28[i][(j + round) % 28];//round 규칙에 따라 shift

			for (j = 0; j < 28; j++)
				arr_bit28[i][j] = arr_tempBit28[j];
		}

		//////Step4) Join key//////
		for (i = 0; i < 2; i++)
			for (j = 0; j < 28; j++)
				arr_bit56[i * 28 + j] = arr_bit28[i][j];

		//////Step5) 56bit to 48bits(keyCompression)//////
		tableSwap("keyCompression", arr_bit56, 6, 8);

		for (i = 0; i < 48; i++)//48bit만 검출
			arr_bit48[i] = arr_bit56[i];

		for (i = 0; i < 48; i++)
			key[round][i] = arr_bit48[i];
	}
	//printf("First round key\n");
	//viewArray(key[0], 6, 8);
	binaryToHex(key[0], 6, 8);
}



void keyChecking(int key[][48]) {

	int round, i, j;
	int keyValue[16][6] = { 0 };//16rounds 6byte

	for (round = 0; round < 16; round++) {//before
		printf("round%2d : ", round + 1);

		for (i = 0; i < 48; i++)
			printf("%d", key[round][i]);

		printf("\n");
	}

	for (round = 0; round < 16; round++) {
		printf("\nround%2d : ", round + 1);
		for (i = 0; i < 6; i++) {
			for (j = 0; j < 8; j++)
				keyValue[round][i] += key[round][i * 8 + j] * (int)pow(2, 7 - j);

			printf("%02X", keyValue[round][i]);
		}
	}
}

void myXor(int* arrA, int* arrB, int size) {

	int i;
	for (i = 0; i < size; i++)
		arrA[i] ^= arrB[i];
}

void copyArray(int* destination, int* source, int size)
{
	int i;
	for (i = 0; i < size; i++)
		destination[i] = source[i];
}

void roundFunction(int* arr_bit32, int* key) {

	int arr_bit48[48], sboxNum, table[64];
	int i, h, w;
	char fileName[50], numToChar[2];
	FILE* fp;

	//////Step1) 32bits to 48bits(Expansion)//////
	copyArray(arr_bit48, arr_bit32, 32);//확장을 위한 배열 복사
	tableSwap("expansionFunction", arr_bit48, 8, 6);

	//////Step2) xor with key//////
	myXor(arr_bit48, key, sizeof(arr_bit48) / sizeof(int));

	//////Step3) sbox //////
	for (sboxNum = 0; sboxNum < 8; sboxNum++) {

		//////open file stream//////
		sprintf(numToChar, "%d", sboxNum + 1);
		strcpy(fileName, "table\\sbox");
		strcat(fileName, numToChar);//번호를 아스키코드로 인해 문자로바꾸기
		strcat(fileName, ".txt");

		//strcpy(fileName, "table\\sbox1.txt");//임시로 sbox1만 하게끔 함
		fp = fopen(fileName, "r");

		//////Insert table//////
		for (i = 0; !feof(fp); i++)
			fscanf_s(fp, "%d", &table[i]);

		fclose(fp);//파일 스트림 닫기

		///////find and insert//////
		h = arr_bit48[sboxNum * 6] * 2 + arr_bit48[sboxNum * 6 + 5];
		for (w = i = 0; i < 4; i++)//w넣기
			w += arr_bit48[sboxNum * 6 + (i + 1)] * (int)pow(2, 3 - i);

		for (i = 2, w = 0; i < 6; i++, w++)//arr_bit32에다가 값 넣기, 앞에 2bit는 손실되기 때문에 초기값 2
			arr_bit32[sboxNum * 4 + i - 2] = (table[h * 16 + w] & (int)pow(2, 3 - i + 2)) / (int)pow(2, 3 - i + 2);
	}

	//////Step4) change bit for output//////
	tableSwap("straight", arr_bit32, 4, 8);
}

void round(int* plainBit, int key[][48], int* cryptoBit) {

	int arr_bit32[2][32], arr_tempBit32[32], arr_bit64[64];
	int i, j, round;

	//////Step1) divide//////
	for (i = 0; i < 2; i++)
		for (j = 0; j < 32; j++)
			arr_bit32[i][j] = plainBit[i * 32 + j];

	for (round = 0; round < 16; round++) {
		//////Step2) Mixer//////
		//////function//////
		copyArray(arr_tempBit32, arr_bit32[1], sizeof(arr_tempBit32) / sizeof(int));//right는 xor하기위해 변형됨.
		roundFunction(arr_bit32[1], key[round]);

		//////Left XOR with right//////
		myXor(arr_bit32[0], arr_bit32[1], 32);

		//////Step3) swap//////
		copyArray(arr_bit32[1], arr_bit32[0], sizeof(arr_bit32[1]) / sizeof(int));
		copyArray(arr_bit32[0], arr_tempBit32, sizeof(arr_bit32[0]) / sizeof(int));
	}

	//////Step4) output//////
	for (i = 0; i < 2; i++)
		for (j = 0; j < 32; j++)
			arr_bit64[i * 32 + j] = arr_bit32[i][j];

	copyArray(cryptoBit, arr_bit64, 64);
}

void bitToString(int* bit, int size) {

	int string[9] = { 0 };
	int i, j;

	for (i = 0; i < size / 8; i++) {
		for (j = 0; j < 8; j++)
			string[i] += bit[i * 8 + j] * (int)pow(2, 7 - j);
		printf("%c", string[i]);
	}
}