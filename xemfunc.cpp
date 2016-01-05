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

const int shift[7] = {0, 1, 1, 2, 2, 1, 1};

bool xem(char* passwd, FILE *fin, FILE *fout) {

	char* md5_hash;
	unsigned int b[20];
	unsigned int now_key = 0, mys1 = 0, mys2 = 0;
	unsigned int buff = 0;

	md5_hash = MDString(passwd);
	b[0] = 0;
	for (int i = 1; i < 32; i += 2)
	{
		int bt1 = (int)md5_hash[i];
		int bt2 = (int)md5_hash[i + 1];
		
		if (bt2 < 97) b[++b[0]] = bt2 - 48; else b[++b[0]] = bt2 - 87;
		if (bt1 < 97) b[b[0]] += (bt1 - 48) << 4; else b[b[0]] += (bt1 - 87) << 4;
	}

	md5_hash = MDString(md5_hash);
	for (int i = 1; i < 32; i += 2)
	{
		int bt1 = (int)md5_hash[i];
		int bt2 = (int)md5_hash[i + 1];
		int tmp = 0;
		
		if (bt2 < 97) tmp = bt2 - 48; else tmp = bt2 - 87;
		if (bt1 < 97) tmp += (bt1 - 48) << 4; else tmp += (bt1 - 87) << 4;

		mys1 ^= tmp;
	}

	md5_hash = MDString(md5_hash);
	for (int i = 1; i < 32; i += 2 )
	{
		int bt1 = (int)md5_hash[i];
		int bt2 = (int)md5_hash[i + 1];
		int tmp = 0;
		
		if (bt2 < 97) tmp = bt2 - 48; else tmp = bt2 - 87;
		if (bt1 < 97) tmp += (bt1 - 48) << 4; else tmp += (bt1 - 87) << 4;

		mys2 ^= tmp;
	}
	//convert MD5 String
	//init
	int insize;

	if (fin == NULL || fout == NULL) return 0;

	while (insize = fread(&buff, 1, 4, fin))
	{
		for (int i =1; i <=6; i++)
		{
			int T =b[i] <<= shift[i];
			b[i] = (((((T & 0x300) >> 8) | T) + i) & MASK);
			now_key ^= b[i];
		}
		for (int i = 7; i <= 12; i++)
		{
			int T = (b[i] & (1 << (shift[i] - 1))) << 8;
			b[i] = (((b[i] | T) + i) & MASK);
			now_key ^= b[i];
		}
		//shift and change
		b[13] ^= 13; b[15] ^= (~mys1 & MASK);
 		b[14] ^= 14; b[16] ^= (~mys2 & MASK);
		now_key ^= (b[13] ^ b[14] ^ b[15] ^ b[16]) & MASK;

		buff ^= now_key; // of improtance
		if (!fwrite(&buff, 1, insize, fout)) return 0;

		mys1 ^= b[6] ^ b[9];
		mys2 ^= b[7] ^ b[8];

		buff = 0; //important clear!!
	}

	return 1;
}
