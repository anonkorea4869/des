/*DES ����*/
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define SIZE 64//Block Size

void bitExtraction(char* plainText, int* plainBit);//bit�� ����
void viewArray(int* arr, int h, int w);//�迭 Ȯ��
void tableSwap(const char* file, int* arr, int h, int w);//table�� ���� Swap�ϱ�
void liner(int* arr, int arrSize);//�Ϸķ� �����ϱ�
void initialPermutation(char* plainText, int* plainBit);//start
void keyGenerator(char* keyText, int key[][48]);//key ����
void keyChecking(int key[][48]);//round�� �������� Ű Ȯ��
void myXor(int* arrA, int* arrB, int size);//�� �迭�� xor����
void copyArray(int* destination, int* source, int size);//�迭 ����
void roundFunction(int* arr_bit32, int* key);//round �Լ� ����
void round(int* plainBit, int key[][48], int* cryptoBit);//round
void bitToString(int* bit, int size);//bit�� ������ ���ڿ��� �ٲ���
void block(char* plainTextBlock, char* keyTextBlock);//�� ���μ��� ��
void keyOptimization(char* keyText, char* keyTextBlock, int keySize);//key ����ȭ

void tempBitExtraction(int* keyBit);//������ ���� �߸� ��
void binaryToHex(int *, int , int);//2������ 16������ �ٲ��ֱ�

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

	/*printf("���� �Է��ϼ��� :");
	gets_s(plainText);
	printf("key �� �Է��ϼ��� :");
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

	char plainText[9] = {0x01, 0x10, 0x10, 0x23, 0x41, 0x10, 0x23}, keyText[9];//char�� max�� 127(CF) �ʰ��� �����Ƿ� unsigned ���� ����
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

	//////���ڿ� ����//////
	strcat(fileName, file);
	strcat(fileName, ".txt");

	//printf("%s\n", fileName);

	///////�� �ֱ�///////
	table = (int*)malloc(sizeof(int) * h * w);//table�迭 �����

	fp = fopen(fileName, "r");

	for (i = 0; !feof(fp); i++)
		fscanf_s(fp, "%d", &table[i]);//malloc�̿��� doublePointer

	fclose(fp);//���� ��Ʈ�� �ݱ�

	//////bit swap//////
	tempArr = (int*)malloc(sizeof(int) * h * w);//���� �Ѱ��� �迭

	for (i = 0; i < h * w; i++)//�迭 �ӽ�����
		tempArr[i] = arr[table[i] - 1];

	for (i = 0; i < h * w; i++)//�迭 ��ȯ
		arr[i] = tempArr[i];

	free(table);//�����Ҵ� ����
}

void liner(int* arr, int arrSize) {

	int i;
	//for (i = 0; i < arrSize / sizeof(int); i++)
	for (i = 0; i < arrSize ; i++)
		arr[i] = i + 1;
}

void initialPermutation(char* plainText, int* plainBit)
{
	bitExtraction(plainText, plainBit);//����������(��, ��)
	//printf("Hex to binary\n");
	//viewArray(plainBit, 8, 8);
	binaryToHex(plainBit, 8, 8);

	tableSwap("initialPermutation", plainBit, 8, 8);//swap(�����̸�, ����, ���̺� ��/ ��)
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
	//bitExtraction(keyText, keyBit);//����������(��, ��)
	tempBitExtraction(keyBit);
	tableSwap("parityDrop", keyBit, 7, 8);//swap(�����̸�, ����, ���̺� ��/ ��)

	for (i = 0; i < 56; i++)//8��� ������ 56bit �����
		arr_bit56[i] = keyBit[i];
	
	//////Step2)divide//////
	for (i = 0; i < 2; i++)
		for (j = 0; j < 28; j++)
			arr_bit28[i][j] = arr_bit56[i * 28 + j];

	//////Step3-1) shift Rule//////
	for (i = 0; i < 16; i++)//shift rule����
	{
		if (i == 0 || i == 1 || i == 8 || i == 15)
			shift[i] = 1;

		else
			shift[i] = 2;
	}

	for (round = 0; round < 16; round++) {

		//////Step3-2)shifting//////
		for (i = 0; i < 2; i++) {
			for (j = 0; j < 28; j++)//��ȯ �̵� �ӽ�����
				arr_tempBit28[j] = arr_bit28[i][(j + round) % 28];//round ��Ģ�� ���� shift

			for (j = 0; j < 28; j++)
				arr_bit28[i][j] = arr_tempBit28[j];
		}

		//////Step4) Join key//////
		for (i = 0; i < 2; i++)
			for (j = 0; j < 28; j++)
				arr_bit56[i * 28 + j] = arr_bit28[i][j];

		//////Step5) 56bit to 48bits(keyCompression)//////
		tableSwap("keyCompression", arr_bit56, 6, 8);

		for (i = 0; i < 48; i++)//48bit�� ����
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
	copyArray(arr_bit48, arr_bit32, 32);//Ȯ���� ���� �迭 ����
	tableSwap("expansionFunction", arr_bit48, 8, 6);

	//////Step2) xor with key//////
	myXor(arr_bit48, key, sizeof(arr_bit48) / sizeof(int));

	//////Step3) sbox //////
	for (sboxNum = 0; sboxNum < 8; sboxNum++) {

		//////open file stream//////
		sprintf(numToChar, "%d", sboxNum + 1);
		strcpy(fileName, "table\\sbox");
		strcat(fileName, numToChar);//��ȣ�� �ƽ�Ű�ڵ�� ���� ���ڷιٲٱ�
		strcat(fileName, ".txt");

		//strcpy(fileName, "table\\sbox1.txt");//�ӽ÷� sbox1�� �ϰԲ� ��
		fp = fopen(fileName, "r");

		//////Insert table//////
		for (i = 0; !feof(fp); i++)
			fscanf_s(fp, "%d", &table[i]);

		fclose(fp);//���� ��Ʈ�� �ݱ�

		///////find and insert//////
		h = arr_bit48[sboxNum * 6] * 2 + arr_bit48[sboxNum * 6 + 5];
		for (w = i = 0; i < 4; i++)//w�ֱ�
			w += arr_bit48[sboxNum * 6 + (i + 1)] * (int)pow(2, 3 - i);

		for (i = 2, w = 0; i < 6; i++, w++)//arr_bit32���ٰ� �� �ֱ�, �տ� 2bit�� �սǵǱ� ������ �ʱⰪ 2
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
		copyArray(arr_tempBit32, arr_bit32[1], sizeof(arr_tempBit32) / sizeof(int));//right�� xor�ϱ����� ������.
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