// CDKeyDecode.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>


bool verifyCDKey(char *cdkey);
void shuffleCDKey(char *cdkey);
void hashWC2(char *cdkey, int *buffer);
bool verifyWC2(char *cdkey, int checksum);
void war2shuffle(char *cdkey);
void getKeyValues(char *cdkey);
int war2map[256] = 
{
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0x01, 0xFF, 
	0x02, 0x03, 0x04, 0x05, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 
	0x0C, 0xFF, 0x0D, 0x0E, 0xFF, 0x0F, 0x10, 0xFF, 0x11, 
	0xFF, 0x12, 0xFF, 0x13, 0xFF, 0x14, 0x15, 0x16, 0xFF, 
	0x17, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x06, 
	0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0xFF, 0x0D, 0x0E, 
	0xFF, 0x0F, 0x10, 0xFF, 0x11, 0xFF, 0x12, 0xFF, 0x13, 
	0xFF, 0x14, 0x15, 0x16, 0xFF, 0x17, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF 
};


int main(int argc, char *argv[])
{
	char CDKey[] = "0000000000000000";

	int shifted = 0;

	printf("Original key: %s\n", CDKey);	

	hashWC2(CDKey, &shifted);

    printf("Hashed key:   %s\n", CDKey);
    printf("Shifted: %02x\n", shifted);
    
	if(verifyWC2(CDKey, shifted))
		printf("Key is valid!\n");
	else
		printf("Key is invalid!\n");

	war2shuffle(CDKey);

	printf("Shuffled:  %s\n", CDKey);

	getKeyValues(CDKey);
	printf("After final hash: %s\n", CDKey);
	printf("Value should be:  0000000000000000\n");
	system("PAUSE");	
	return 0;
}

bool verifyWC2(char *cdkey, int checksum)
{
	int accumulator = 3;

	for(int i = 0; i < 0x10; i++)
	{
		cdkey[i] = tolower(cdkey[i]);// & 0xFF;
		accumulator += ((accumulator * 2) ^ (cdkey[i] < 'a' ? cdkey[i] - 0x30 : cdkey[i] - 0x57));
	}

	return ((accumulator & 0xFF) == checksum);
}


void swap(char *a, char *b)
{
	char temp = *a;
	*a = *b;
	*b = temp;
}


void hashWC2(char *cdkey, int *shifted)
{
	*shifted = 0;
	for(int i = 0; i < 16; i += 2)
	{
		int hashedChar = war2map[cdkey[i + 1]] + war2map[cdkey[i]] * 24;

		if(hashedChar >= 0x100)
		{
			hashedChar -= 0x100;
			*shifted = *shifted | (int) pow(2, (i / 2));
		}

		cdkey[i] =		(hashedChar >> 4) >= 0x0A ?		(hashedChar >> 4) + 0x37 :		(hashedChar >> 4) + 0x30;
		cdkey[i + 1] =	(hashedChar & 0x0F) >= 0x0A ?	(hashedChar & 0x0F) + 0x37 :	(hashedChar & 0x0F) + 0x30;
	}
}

void war2shuffle(char *cdkey)
{
	for(int i = 0; i < 0x10; i++)
		swap(cdkey + 0x0F - i, &cdkey[(22 - i) & 0xF]);
}



void getKeyValues(char *cdkey)
{
	int hashKey = 0x13ac9741;

	for(char i = strlen(cdkey) - 1; i >= 0; i--)
	{
		cdkey[i] = toupper(cdkey[i]);
		if(cdkey[i] <= '7')
		{
			cdkey[i] = ((hashKey &  7) ^ cdkey[i]);
			hashKey = (hashKey >> 3);
		}
		else if(cdkey[i] < 'A')
		{
			cdkey[i] = (i & 1) ^ (cdkey[i]);	
		}
	}
}
