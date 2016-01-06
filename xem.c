/*
 * $File: xem.cpp
 * $Date: Sun Apr 11 16:15:33 2010 CST
 * $Author: Ted Yin <ymf111111@163.com>
 *
 * @package:Xem
 * @license:GNU Lesser General Public License 3 or any later version
 * @note:CST in $Date means China Standard Time instead of Central Standard Time
 */

#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define MAXP 2048

void showhelp() {
    fprintf(stderr, "\n>>> Simple Sequence Generation Algorithm v0.3 \n\n");
    fprintf(stderr, ">>> Copyright (C) 2010 Ted Yin\n");
    fprintf(stderr, ">>> License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>\n");
    fprintf(stderr, ">>> There is NO WARRANTY, to the extent permitted by law.\n");
    fprintf(stderr, "\n>>> Usage: `xem [<src_file>] [<dest_file>]`\n");
    fprintf(stderr, "\n");
}

extern int xem(char *, FILE *, FILE *);

int main(int argc, char **argv) {

    char pwd[MAXP], rpwd[MAXP], *ptr = pwd, *rptr = rpwd;
    FILE *fin, *fout;
    int res, inc;

    fin = stdin;
    fout = stdout;

    if (argc > 1) fin = fopen(argv[1], "rb");
    if (argc > 2) fout = fopen(argv[2], "wb");
    if (argc > 3)
    {
        showhelp();
        return 1;
    }

    struct termios sg;
    tcgetattr(STDIN_FILENO, &sg); /* get settings */
    sg.c_lflag &= ~ECHO; /* turn echo off */
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &sg); /* set settings */

    fprintf(stderr, "Enter password >>> ");
    inc = 0;
    while ((*ptr++ = getc(stdin)) != '\n')
        if (inc++ == MAXP)
        {
            fprintf(stderr, "Password is too long!");
            return 0;
        }
    *(ptr - 1) = 0;
    fprintf(stderr, "\n");

    fprintf(stderr, "Repeat>>> ");
    inc = 0;
    while ((*rptr++ = getc(stdin)) != '\n')
        if (inc++ == MAXP)
        {
            fprintf(stderr, "Password repeated is too long!");
            return 0;
        }
    *(rptr - 1) = 0;
    fprintf(stderr, "\n");

    tcgetattr(STDIN_FILENO, &sg); /* get settings */
    sg.c_lflag |= ECHO; /* turn echo on */
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &sg); /* set settings */

    if (strcmp(pwd, rpwd))
    {
        fprintf(stderr, "Passwords are not the same!\n");
        return 0;
    }
    res = xem(pwd, fin, fout);
    return !res;
}
