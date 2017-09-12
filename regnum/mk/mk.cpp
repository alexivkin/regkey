// mk.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"

#include <bitset>
#include <iostream>

#define SECTIONS			 4
#define CHARSPERSECTION		 4
#define BITSPERCHAR			 5
#define TOTALCHARS CHARSPERSECTION*SECTIONS
#define KEYLENGTH SECTIONS*(CHARSPERSECTION+1) // including delimiter
#define BITSTREAMLENGTH BITSPERCHAR*TOTALCHARS
#define BYTESTREAMLENGTH BITSTREAMLENGTH/8 // should be an integer!

typedef unsigned char byte;
typedef std::bitset<BITSTREAMLENGTH> bitstream;

using namespace std;

int main(int argc, char* argv[])
{
	if (argc != 3 || strlen(argv[1])!=5){
		cout << "usage: " << argv[0] << " custid custip\n  custid - 5 bytes\n";
		return 0;
	}
//	cout << "Current key data - Chars in a key: " << TOTALCHARS << 
//		" --- Bits per char: " <<  BITSPERCHAR << " --- Bytes in a key: " << BYTESTREAMLENGTH << endl;
	//pack the data into a bytestream
	byte bStream[BYTESTREAMLENGTH+1];
	char *sep=".";
	// pack the data
	strcpy((char *)bStream,argv[1]);
    bStream[5]=(char)atoi(strtok(argv[2],sep));
	bStream[6]=(char)atoi(strtok(NULL,sep));
	bStream[7]=(char)atoi(strtok(NULL,sep));
	bStream[8]=(char)atoi(strtok(NULL,sep));
	bStream[9]=0; //to be computed later
	bStream[BYTESTREAMLENGTH]=0;// zero terminate
//	cout << "Packed data: "<< bStream << endl;
	// calculate CRC
	// Encrypt the data
	byte bCRC=0;
	for (int i=BYTESTREAMLENGTH-1;i > 0;i--){
		bStream[i] = bStream[i]^bStream[i-1];
		bCRC = bCRC-bStream[i-1];
	}
	bStream[9]=bCRC;
//	cout << "Encrypted data: "<< bStream << endl;
//	cout << "CRC: " << (int)bCRC <<":"<< (int)bStream[9] << endl;

	// Generate the key
	// push the data into the bitstream
	bitstream bsBits;
	// working from right to left for bits
	int iBitNum=BITSTREAMLENGTH-1;
	for (int i=0; i < BYTESTREAMLENGTH; i++){	 // for every byte
			for(int j=0;j<8;j++){				 // for every bit
				bsBits.set(iBitNum-7+j,bStream[i]&1);// reverse filling
				bStream[i]>>=1;					 // reverse reading
			}
			iBitNum-=8;
	}
//	cout << "Bitstream: ["<< bsBits << "]" << endl;
	// make a key out of a bitstream
	char cKey[KEYLENGTH];
	iBitNum=BITSTREAMLENGTH-1;
	int iByteNum=0;
	for (int k=0; k < SECTIONS; k++){ // for every section
		for (int i=0; i < CHARSPERSECTION; i++){ // for every character
			cKey[iByteNum]=0;
			for (int j=BITSPERCHAR-1; j >= 0; j--){ // for every bit
				cKey[iByteNum]+=bsBits.test(iBitNum) ? 1<<j:0;
				iBitNum--;
			}
			// inflate it to 8 bits
			if (cKey[iByteNum] < 6)
				cKey[iByteNum] += '2'; //lowest values (0-5)
			else
				cKey[iByteNum] += 'A'-6; // top values (6-31)
			iByteNum++;
		}
		if (k < SECTIONS-1){ // if not the last section
			cKey[iByteNum]='-'; // add delimiter
			iByteNum++; //skip the delimiter
		}
	}
	cKey[iByteNum]=0;
	cout << "The key: " << cKey << endl;
	return 0;
}

