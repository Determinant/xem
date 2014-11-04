/*
 * $File: xemfunc.cpp
 * $Date: Sun May 02 11:07:27 2010 CST
 * $Author: Ted Yin <ymf111111@163.com>
 *
 * @package:Xem
 * @license:GNU Lesser General Public License 3 or any later version                   
 * @note:CST in $Date means China Standard Time instead of Central Standard Time
 */

#include <iostream>
#include <cstdio>
#include <cstring>
#include "md5lib.h"
#include "xemfunc.h"
#include "base64.h"

#define MASK 0xFFFFFFFF

using namespace std;

const int shift[7]={0,1,1,2,2,1,1};

bool Xem(char* PASSWD,const char *infile,const char *outfile) {

	char* MD5_HASH;
	unsigned int B[20];
	unsigned int NOW_KEY=0,MYS1=0,MYS2=0;
	unsigned int BUFF = 0;

	MD5_HASH=MDString(PASSWD);
	B[0]=0;
	for (int i=1; i<32; i+=2)
	{
		int BT1=(int)MD5_HASH[i];
		int BT2=(int)MD5_HASH[i+1];
		
		if (BT2<97) B[++B[0]]=BT2-48; else B[++B[0]]=BT2-87;
		if (BT1<97) B[B[0]]+=(BT1-48) << 4; else B[B[0]]+=(BT1-87) << 4;
	}

	MD5_HASH=MDString(MD5_HASH);
	for (int i=1; i<32; i+=2)
	{
		int BT1=(int)MD5_HASH[i];
		int BT2=(int)MD5_HASH[i+1];
		int TMP=0;
		
		if (BT2<97) TMP=BT2-48; else TMP=BT2-87;
		if (BT1<97) TMP+=(BT1-48) << 4; else TMP+=(BT1-87) << 4;

		MYS1 ^= TMP;
	}

	MD5_HASH=MDString(MD5_HASH);
	for (int i=1; i<32; i+=2)
	{
		int BT1=(int)MD5_HASH[i];
		int BT2=(int)MD5_HASH[i+1];
		int TMP=0;
		
		if (BT2<97) TMP=BT2-48; else TMP=BT2-87;
		if (BT1<97) TMP+=(BT1-48) << 4; else TMP+=(BT1-87) << 4;

		MYS2 ^= TMP;
	}
	//convert MD5 String
	//init
	
	FILE *INPUT = fopen(infile,"rb");
	FILE *OUTPUT = fopen(outfile,"wb");
	int insize;

	if (INPUT == NULL || OUTPUT == NULL) return 0;

	while (insize = fread(&BUFF,1,4,INPUT))
	{
		for (int i=1; i<=6; i++)
		{
			int T=B[i]<<=shift[i];
			B[i]=(((((T & 0x300)>>8) | T)+i) & MASK);
			NOW_KEY ^= B[i];			
		}
		for (int i=7; i<=12; i++)
		{
			int T=(B[i] & (1<<(shift[i]-1)))<<8;
			B[i]=(((B[i] | T)+i) & MASK);
			NOW_KEY ^= B[i];
		}
		//shift and change
		B[13] ^= 13; B[15] ^= (~MYS1 & MASK);
 		B[14] ^= 14; B[16] ^= (~MYS2 & MASK);
		NOW_KEY ^= (B[13]^B[14]^B[15]^B[16]) & MASK;

		BUFF ^= NOW_KEY; // of improtance
		if (!fwrite(&BUFF,1,insize,OUTPUT)) return 0;

		MYS1 ^= B[6] ^ B[9];
		MYS2 ^= B[7] ^ B[8];

		BUFF = 0; //important clear!!
	}
	
	fclose(INPUT);
	fclose(OUTPUT);

	return 1;
}

bool xem_encode(char *PASSWD,const char *infile,const char *outfile) {

	bool flag = Xem(PASSWD,infile,".xem.tmp");
	flag &= base64_file_encode(".xem.tmp",outfile);
	remove(".xem.tmp"); //try to remove tmp file
	if (flag) return 1; else return 0;
}


bool xem_decode(char *PASSWD,const char *infile,const char *outfile) {

	bool flag = base64_file_decode(infile,".xem.tmp");
	flag &= Xem(PASSWD,".xem.tmp",outfile);
	remove(".xem.tmp"); //try to remove tmp file
	if (flag) return 1; else return 0;
}



