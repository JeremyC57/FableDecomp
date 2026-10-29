#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// 00c03cb0 z_adler32
// Leaf: _z_adler32
// Flags:
/* adler32.c -- compute the Adler-32 checksum of a data stream
 * Copyright (C) 1995-2002 Mark Adler
 * For conditions of distribution and use, see copyright notice in zlib.h 
 */

/* @(#) $Id$ */

#ifndef _MSC_VER
#define __cdecl
#endif
typedef unsigned char Byte;
typedef Byte Bytef;
typedef unsigned int uInt;
typedef unsigned long uLong;
#define Z_NULL 0

#define BASE 65521L /* largest prime smaller than 65536 */
#define NMAX 5552
/* NMAX is the largest n such that 255n(n+1)/2 + (n+1)(BASE-1) <= 2^32-1 */

#define DO1(buf,i)  {s1 += buf[i]; s2 += s1;}
#define DO2(buf,i)  DO1(buf,i); DO1(buf,i+1);
#define DO4(buf,i)  DO2(buf,i); DO2(buf,i+2);
#define DO8(buf,i)  DO4(buf,i); DO4(buf,i+4);
#define DO16(buf)   DO8(buf,0); DO8(buf,8);

/* ========================================================================= */
extern "C" uLong __cdecl z_adler32(uLong adler, const Bytef *buf, uInt len)
{
    unsigned long s1 = adler & 0xffff;
    unsigned long s2 = (adler >> 16) & 0xffff;
    int k;

    if (buf == Z_NULL) return 1L;

    while (len > 0) {
        k = len < NMAX ? len : NMAX;
        len -= k;
        while (k >= 16) {
            DO16(buf);
	    buf += 16;
            k -= 16;
        }
        if (k != 0) do {
            s1 += *buf++;
	    s2 += s1;
        } while (--k);
        s1 %= BASE;
        s2 %= BASE;
    }
    return (s2 << 16) | s1;
}

static void check(bool p){if(!p)abort();}
static uLong serial(uLong state,const Bytef*p,uInt n){uLong a=state&65535UL,b=state>>16;while(n--){a=(a+*p++)%65521UL;b=(b+a)%65521UL;}return (b<<16)|a;}
static Bytef bytes[70016];
int main(){check(sizeof(uLong)==4);check(z_adler32(0,0,0)==1&&z_adler32(123,0,100)==1);check(z_adler32(1,(const Bytef*)"",0)==1);check(z_adler32(1,(const Bytef*)"Wikipedia",9)==0x11e60398UL);check(z_adler32(1,(const Bytef*)"123456789",9)==0x091e01deUL);check(z_adler32(1,(const Bytef*)"abc",3)==0x024d0127UL);check(z_adler32(0x12345678UL,(const Bytef*)"",0)==0x12345678UL);for(unsigned c=0;c<256;++c){bytes[0]=Bytef(c);check(z_adler32(1,bytes,1)==serial(1,bytes,1));}const unsigned lengths[]={0,1,2,15,16,17,31,32,33,255,256,257,5551,5552,5553,5567,5568,11103,11104,11105,32768,65535,70000};for(unsigned pattern=0;pattern<4;++pattern){for(unsigned i=0;i<70016;++i)bytes[i]=pattern==0?Bytef(0):(pattern==1?Bytef(255):(pattern==2?Bytef(i):Bytef((i*73+(i>>3)*19)^0xa5)));for(unsigned n=0;n<sizeof(lengths)/sizeof(lengths[0]);++n){unsigned count=lengths[n];uLong expected=serial(1,bytes,count);check(z_adler32(1,bytes,count)==expected);check(z_adler32(1,bytes+1,count)==serial(1,bytes+1,count));for(unsigned step=1;step<=5;++step){unsigned stride=step==1?1:(step==2?17:(step==3?5552:(step==4?8191:65536)));uLong state=1;for(unsigned offset=0;offset<count;){unsigned block=count-offset;if(block>stride)block=stride;state=z_adler32(state,bytes+offset,block);offset+=block;}check(state==expected);}}for(unsigned count=0;count<300;++count)check(z_adler32(1,bytes,count)==serial(1,bytes,count));}puts("00c03cb0_TEST PASS");return 0;}