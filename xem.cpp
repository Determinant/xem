/*
 * $File: xem.cpp
 * $Date: Sun Apr 11 16:15:33 2010 CST
 * $Author: Ted Yin <ymf111111@163.com>
 *
 * @package:Xem
 * @license:GNU Lesser General Public License 3 or any later version                   
 * @note:CST in $Date means China Standard Time instead of Central Standard Time
 */

#include <iostream>
#include <cstdio>
#include <cstring>
#include <termios.h>
#include <unistd.h>
#include "xemfunc.h"
#include "base64.h"

#define MAXP 2048

using namespace std;

void showhelp() {
	printf("\n>>> Simple Sequence Generation Algorithm v0.3 \n\n");
	printf(">>> Copyright (C) 2010 Ted Yin\n");
	printf(">>> License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>\n");
	printf(">>> There is NO WARRANTY, to the extent permitted by law.\n");
	printf("\n>>> Usage: -e for Encode (-d for Decode) SrcNAME DestNAME\n");
	printf("\n");
}

int main(int argc,char **argv) {

	char pwd[MAXP],rpwd[MAXP],*ptr = pwd,*rptr = rpwd;
	int mode,res,inc;

	if (argc != 4) {showhelp(); return 0 ;}
	if (!strcmp(argv[1],"-e")) mode = 1;
		else if (!strcmp(argv[1],"-d")) mode = 0;
			else {showhelp(); return 0;}
	
	if (!strlen(argv[2]) || !strlen(argv[3])) {showhelp(); return 0;}


	struct termios sg; 
	tcgetattr(STDIN_FILENO, &sg); /* get settings */
	sg.c_lflag &= ~ECHO; /* turn echo off */ 
	tcsetattr(STDIN_FILENO,TCSAFLUSH,&sg); /* set settings */

	printf("Enter password >>> ");
	inc = 0;
	while ((*ptr++ = getc(stdin)) != '\n'); 
		if (inc++ == MAXP) 
		{
			printf("Password is too long!"); 
			return 0;
		}
	*(ptr - 1) = 0;
	printf("\n");

	printf("Repeat>>> ");
	inc = 0;
	while ((*rptr++ = getc(stdin)) != '\n')
		if (inc++ == MAXP) 
		{
			printf("Password repeated is too long!"); 
			return 0;
		}
	*(rptr - 1) = 0;
	printf("\n");
	
	tcgetattr(STDIN_FILENO, &sg); /* get settings */
	sg.c_lflag |= ECHO; /* turn echo on */
	tcsetattr(STDIN_FILENO,TCSAFLUSH,&sg); /* set settings */

	if (strcmp(pwd,rpwd)) {printf("Passwords are not the same!\n"); return 0;}
	if (mode == 1) res = xem_encode(pwd,(const char*)argv[2],(const char*)argv[3]);
		else res = xem_decode(pwd,(const char*)argv[2],(const char *)argv[3]);

	if (!res) printf("Failed!\n");

	return 0;
}
