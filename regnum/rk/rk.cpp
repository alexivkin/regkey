// rk.cpp : Defines the entry point for the console application.
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

byte getbyte(bitstream& bs,int i); // 1 if 1, 0 if 0
inline byte getbit(bitstream& bs,int i); // 1 if 1, 0 if 0

int main(int argc, char* argv[])
{
	using namespace std;
	if ((argc != 2) || (strlen(argv[1])!=KEYLENGTH-1)){
		printf("usage: %s key\n   key must be %d bytes long (including delimiters)\n", argv[0],KEYLENGTH);
        return 0;
	}
	bitstream bsStream;
//	cout << "Current key data - Chars in a key: " << TOTALCHARS << 
//		" --- Bits per char: " <<  BITSPERCHAR << " --- Bytes in a key: " << BYTESTREAMLENGTH << endl;
    // push the key in 
	// working from right to left for bits
	int iBitNum=BITSTREAMLENGTH-1,iByteNum=0;
	//bool bBit;
	char cByte;
	for (int k=0; k < SECTIONS; k++){ // for every section
		for (int i=0; i < CHARSPERSECTION; i++){ // for every character
			cByte = argv[1][iByteNum++];
			// deflate it to 5 bits
			if (cByte > '1' && cByte < '8')
				cByte -= '2'; //lowest values (0-5)
			else if (cByte >= 'A' && cByte <= 'Z')
				cByte -= 'A' - 6; // top values (6-31)
			else {
				cout << "Typo in the key: '"<< cByte << "' at " << iByteNum << endl;
				return 0;
			}
			for (int j=BITSPERCHAR-1; j >= 0; j--){ // for every bit
				if ((cByte >> j) & 1)
						bsStream.set(iBitNum);
				iBitNum--;
			}
		}
		iByteNum++; //skip the delimiter
	}
//	cout << "Bitstream: ["<< bsStream << "]" << endl;
	// Unpack
	byte pbData[BYTESTREAMLENGTH+1];
	for (int i=0;i < BYTESTREAMLENGTH;i++){
		pbData[i] = getbyte(bsStream,BITSTREAMLENGTH-(i+1)*8);//reverse filling
	}
	pbData[BYTESTREAMLENGTH]=0;
//	cout << "Unpacked: " << pbData << endl;
	// Unencrypt
	byte bCRC=0;
	byte bCRCReal=pbData[BYTESTREAMLENGTH-1]; // do not unencrypt the crc
	for (int i=1;i < BYTESTREAMLENGTH;i++){
		bCRC = bCRC-pbData[i-1]; // crc for the unencrypted data
		pbData[i] = pbData[i]^pbData[i-1];
	}
//	cout << "Unencrypted: " << pbData << endl;
//	cout << "CRC:" << (int)bCRC << ":" << (int)bCRCX << endl;
	// unpack the data
	char pcCustomer[6];
	char pcIP[17];
	strncpy(pcCustomer,(char *)pbData,5);
	sprintf(pcIP,"%d.%d.%d.%d",pbData[5],pbData[6],pbData[7],pbData[8]);
    // zero string termination
	pcCustomer[5]=0;
	if (bCRC != bCRCReal)
		cout << "Invalid key!" << endl;
	else
		cout << "Customer: "<< pcCustomer << " IP: "<< pcIP  << endl;
	return 0;
}

byte getbyte(bitstream& bs,int i){
	/* from i forward
	byte bRet=0;
	for(int j=0;j<8;j++)
		bRet+=getbit(bs,i+j)<<j;
	return bRet;
	}*/
	return (getbit(bs,i+7)<<7)+(getbit(bs,i+6)<<6)+(getbit(bs,i+5)<<5)+(getbit(bs,i+4)<<4)+
		   (getbit(bs,i+3)<<3)+(getbit(bs,i+2)<<2)+(getbit(bs,i+1)<<1)+(getbit(bs,i));
}
byte getbit(bitstream& bs,int i){
	return bs.test(i) ? 1:0;
}
