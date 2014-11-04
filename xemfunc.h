/*
 * $File: xemfunc.h
 * $Date: Sun Apr 11 11:47:56 2010 CST
 * $Author: Ted Yin <ymf111111@163.com>
 *
 * @package:Xem
 * @license:GNU Lesser General Public License 3 or any later version                   
 * @note:CST in $Date means China Standard Time instead of Central Standard Time
 */


#ifndef XEMFUNC_H
#define XEMFUNC_H

bool xem_encode(char *PASSWD,const char *INPUT,const char *OUTPUT);
bool xem_decode(char *PASSWD,const char *INPUT,const char *OUTPUT);

#endif //XEMFUNC_H
