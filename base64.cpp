/*
 * $File: base64.cpp
 * $Date: Sun Apr 11 16:15:06 2010 CST
 * $Author: Ted Yin <ymf111111@163.com>
 *
 * @package:Xem
 * @license:GNU Lesser General Public License 3 or any later version                   
 * @note:CST in $Date means China Standard Time instead of Central Standard Time
 */

#include <iostream>
#include <cstdio>
#include <cstring>
#include "base64.h"

#define OMAXBuff 59049
#define BMAXBuff 65536

using namespace std;

const unsigned char table[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
unsigned char table2[257];

void base64_init() {

	for (int i = 'A'; i <= 'Z'; i++) table2[i] = i - 'A';
	for (int i = 'a'; i <= 'z'; i++) table2[i] = i - 'a' + 26;
	for (int i = '0'; i <= '9'; i++) table2[i] = i - '0' + 52;
	table2['+'] = 62;
	table2['/'] = 63;
	table2['='] = 0;
}

int base64_encode(unsigned char *dest,unsigned char *text,int len) {

	unsigned int buffer;
	unsigned char *ptr = text,*ptr2 = dest;
	int blocks = len / 3;
	int remain = len % 3;
	int inc = 0,final = blocks << 2;
	if (remain) final += 4;
	
	while (++inc <= blocks)
	{
		buffer = ((*ptr) << 16) + (*(ptr + 1) << 8) + *(ptr + 2);
		*ptr2++ = table[(buffer & 0xFC0000) >> 18];
		*ptr2++ = table[(buffer & 0x03F000) >> 12];
		*ptr2++ = table[(buffer & 0x000FC0) >> 6];
		*ptr2++ = table[buffer & 0x00003F];
		ptr += 3;
	}
	if (remain == 2)
	{
		buffer = ((*ptr) << 16) + (*(ptr + 1) << 8);
		*ptr2++ = table[(buffer & 0xFC0000) >> 18];
		*ptr2++ = table[(buffer & 0x03F000) >> 12];
		*ptr2++ = table[(buffer & 0x000FC0) >> 6];
		*ptr2++ = '=';
	}
	else if (remain)
	{
		buffer = (*ptr) << 16;
		*ptr2++ = table[(buffer & 0xFC0000) >> 18];
		*ptr2++ = table[(buffer & 0x03F000) >> 12];
		*ptr2++ = '='; *ptr2 = '=';
	}
	return final;
}

int base64_decode(unsigned char *dest,unsigned char *text,int len) {
	
	unsigned int buffer;
	unsigned char *ptr = text,*ptr2 = dest;
	int blocks = len >> 2,inc = 0,final = 0;
	while (++inc <= blocks)
	{
		buffer = (table2[*ptr] << 18) + (table2[*(ptr + 1)] << 12) + (table2[*(ptr + 2)] << 6) + table2[*(ptr + 3)];
		*ptr2++ = (buffer & 0xFF0000) >> 16;
		*ptr2++ = (buffer & 0x00FF00) >> 8;
		*ptr2++ = (buffer & 0x0000FF);
		ptr += 4; final += 3;
	}
	if (text[len - 1] == '=') final--; // '=' is not a real byte !!!
	if (text[len - 2] == '=') final--;

	return final;
}

bool base64_file_encode(const char *infile,const char *outfile) {

	FILE *INPUT = fopen(infile,"rb");
	FILE *OUTPUT = fopen(outfile,"wb");
	
	if (INPUT == NULL || OUTPUT == NULL) return 0; //failed

	unsigned char buff1[OMAXBuff],buff2[OMAXBuff];
	int insize = 0,outsize = 0;
	while (insize = fread(buff1,1,OMAXBuff,INPUT))
	{
		outsize = base64_encode(buff2,buff1,insize);
		if (!fwrite(buff2,1,outsize,OUTPUT)) return 0;
	}

	fclose(INPUT);
	fclose(OUTPUT);
	return 1;
}


bool base64_file_decode(const char *infile,const char *outfile) {

	FILE *INPUT = fopen(infile,"rb");
	FILE *OUTPUT = fopen(outfile,"wb");
	
	if (INPUT == NULL || OUTPUT == NULL) return 0; //failed

	base64_init(); //

	unsigned char buff1[BMAXBuff],buff2[BMAXBuff];
	int insize = 0,outsize = 0;
	while (insize = fread(buff1,1,BMAXBuff,INPUT))
	{
		outsize = base64_decode(buff2,buff1,insize);
		if (!fwrite(buff2,1,outsize,OUTPUT)) return 0;
	}

	fclose(INPUT);
	fclose(OUTPUT);

	return 1;
}

/*
int main() {

	char str[1000],str2[1000],str3[1000],tmp[500];
	int mode;
	char *ptr = str;
	while (fgets(tmp,sizeof(tmp),stdin))
	{
		memmove(ptr,tmp,strlen(tmp));
		ptr += strlen(tmp);
	}
	*(ptr - 1) = 0;
	base64_encode(str2,str,strlen(str));
	base64_decode(str3,str2,strlen(str2));
	printf("%s\n",str2);
	printf("%s\n",str3);
	scanf("%s %s\n",str,str2);
	scanf("%d",&mode);
	if (mode) base64_file_encode(str,str2);
		else base64_file_decode(str,str2);

	return 0;
}*/
