
#$File: Makefile
#$Date: Sun May 02 11:37:56 2010 CST
#$Author: Ted Yin <ymf111111@163.com>
#@package:Xem
#@license:GNU Lesser General Public License 3 or any later version                   
#@note:CST in $Date means China Standard Time instead of Central Standard Time

.PHONY: clean
CC = clang
xem: xem.o xemfunc.o
	$(CC) *.o -o xem -Wall

xem.o: xem.c
	$(CC) xem.c -o xem.o -c -O2 -Wall

xemfunc.o:xemfunc.c
	$(CC) xemfunc.c -o xemfunc.o -c -O2 -Wall

clean:
	-rm -r *.o xem
