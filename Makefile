
#$File: Makefile
#$Date: Sun May 02 11:37:56 2010 CST
#$Author: Ted Yin <ymf111111@163.com>
#@package:Xem
#@license:GNU Lesser General Public License 3 or any later version                   
#@note:CST in $Date means China Standard Time instead of Central Standard Time

.PHONY: clean
all: base64.o xem.o xemfunc.o
	g++ *.o -o xem -Wall

base64.o: base64.cpp base64.h
	g++ base64.cpp -o base64.o -c -g -Wall

xem.o: xem.cpp
	g++ xem.cpp -o xem.o -c -g -Wall

xemfunc.o:xemfunc.cpp xemfunc.h
	g++ xemfunc.cpp -o xemfunc.o -c -g -Wall

clean:
	-rm -r *.o xem
