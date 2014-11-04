/*
 * $File: base64.h
 * $Date: Sun Apr 11 11:49:23 2010 CST
 * $Author: Ted Yin <ymf111111@163.com>
 *
 * @package:Xem
 * @license:GNU Lesser General Public License 3 or any later version                   
 * @note:CST in $Date means China Standard Time instead of Central Standard Time
 */

#ifndef BASE64_H
#define BASE64_H

void base64_init();
int base64_encode(char *DEST,char *TEXT,int len);
int base64_decode(char *DEST,char *TEXT,int len);

bool base64_file_encode(const char *input,const char *output);
bool base64_file_decode(const char *input,const char *output);

#endif // BASE64_H
