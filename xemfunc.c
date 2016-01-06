/*
 * $File: xemfunc.cpp
 * $Date: Sun May 02 11:07:27 2010 CST
 * $Author: Ted Yin <ymf111111@163.com>
 *
 * @package:Xem
 * @license:GNU Lesser General Public License 3 or any later version
 * @note:CST in $Date means China Standard Time instead of Central Standard Time
 */

#include <stdio.h>
#include <string.h>
#include "md5lib.h"

#define TO_DIGIT(x) ((x) < 97 ? ((x) - 48) : ((x) - 87))
#define TO_DIGIT2(arr, i) (TO_DIGIT(arr[i + 1]) + (TO_DIGIT(arr[i]) << 4))
#define ROUND \
    do { \
        int i; \
		for (i = 0; i < 6; i++) \
        { \
            int x = b[i], y = b[i + 6]; \
            mys1 ^= b[i] ^= (y + 1) * 131; \
            mys2 ^= b[i + 6] = (x + 1) * 131; \
        } \
		b[12] ^= b[13]; \
 		b[13] ^= b[14]; \
        b[14] ^= mys1; \
        b[15] ^= mys2; \
    } while (0)

int xem(char* passwd, FILE *fin, FILE *fout) {

	char* md5_hash;
	unsigned int b[16];
	unsigned int now_key = 0, mys1 = 0, mys2 = 0;
	unsigned int buff = 0;
    int i, j;
    if (!(fin && fout)) return 0;
	md5_hash = MDString(passwd);
	for (i = 0, j = 0; i < 31; i += 2, j++)
		b[j] = TO_DIGIT2(md5_hash, i);
	md5_hash = MDString(md5_hash);
	for (i = 0; i < 31; i += 2)
		mys1 ^= TO_DIGIT2(md5_hash, i);
	md5_hash = MDString(md5_hash);
	for (i = 0; i < 31; i += 2)
		mys2 ^= TO_DIGIT2(md5_hash, i);
    for (i = 0; i < 16; i++) ROUND;
	int insize;
	while ((insize = fread(&buff, 1, 4, fin)))
	{
        /*
        for (i = 0; i < 16; i++)
            fprintf(stderr, "%x ", b[i]);
        fprintf(stderr, "\n");
        */
        ROUND;
		now_key ^= b[12] ^ b[13] ^ b[14] ^ b[15];

		buff ^= now_key;
		if (!fwrite(&buff, 1, insize, fout)) return 0;
		buff = 0;
	}
	return 1;
}
