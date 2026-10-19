#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <new>
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#define __stdcall
#define __declspec(x)
#define __int64 long long
#endif

// CPackedUIntArray::CountBits -- bits required by biased, strided unsigned values.
// Retail 00a62b70 length 285. PDB CPackedUIntArray size16 fields0/4/8/12;
// no generated header exists (including quarantine). Mask table013979e8.
#include <string.h>
#ifdef _MSC_VER
typedef unsigned long UInt; typedef long SInt;
#define INLINE __forceinline
#else
typedef unsigned int UInt; typedef int SInt;
#define INLINE inline
#endif
extern UInt PackedBitMasks[33]; // 0x013979e8, mask for each bit count
struct CPackedUIntArray {
    UInt* PackedInts; UInt Size; unsigned char Bits; UInt Bias;
    UInt CountBits(const void* data, UInt bits, SInt stride, UInt count) const;
};
static INLINE UInt RequiredBits(UInt value) {
    UInt bit;
    if (value >= 0x80000000U) bit=31;
    else {
        UInt next=value+1;
        bit=((SInt)(0x10000U-next)>>31)&16;
        bit+=((SInt)((1U<<(bit+8))-next)>>31)&8;
        bit+=((SInt)((1U<<(bit+4))-next)>>31)&4;
        bit+=((SInt)((1U<<(bit+2))-next)>>31)&2;
        return bit+(((SInt)((1U<<(bit+1))-next)>>31)&1);
    }
    return bit;
}
UInt CPackedUIntArray::CountBits(const void* data, UInt bits, SInt stride, UInt count) const {
    const unsigned char* p=(const unsigned char*)data;
    UInt value=0;
    UInt bytes=(bits+7)>>3;
    UInt aggregate=0;
    UInt mask=PackedBitMasks[bits];
    for(UInt i=0;i<count;++i) {
        if(bytes==4) value=*(const UInt*)p;
        else if(bytes>0) memcpy(&value,p,bytes);
        value&=mask;
        value-=Bias;
        aggregate|=value;
        p+=stride;
    }
    if (aggregate) return RequiredBits(aggregate)+1;
    return 0;
}



#include <stdio.h>
UInt PackedBitMasks[33];
static UInt reference(unsigned char* p, UInt bits, SInt stride, UInt count, UInt bias) {
    UInt value=0, all=0, mask=bits==32?~UInt(0):((UInt(1)<<bits)-1);
    for(UInt i=0;i<count;++i,p+=stride) {
        unsigned char* q=(unsigned char*)&value;
        for(UInt j=0;j<(bits+7)/8;++j) q[j]=p[j];
        value=(value&mask)-bias; all|=value;
    }
    UInt result=0; while(all) { ++result; all>>=1; } return result;
}
int main() {
    CPackedUIntArray a; a.PackedInts=0; a.Size=0; a.Bits=0;
    UInt words[16], seed=123456789U;
    for(UInt i=0;i<33;++i) PackedBitMasks[i]=i==32?~UInt(0):((UInt(1)<<i)-1);
    for(UInt pass=0;pass<100;++pass) {
        for(UInt j=0;j<16;++j) { seed=seed*1664525U+1013904223U; words[j]=seed; }
        for(UInt bits=0;bits<=32;++bits) for(UInt count=0;count<=16;++count) {
            a.Bias=(pass%3==0)?0:((pass%3==1)?words[0]:~UInt(0));
            unsigned char* p=(unsigned char*)words;
            if(a.CountBits(p,bits,4,count)!=reference(p,bits,4,count,a.Bias)) return 1;
            p=(unsigned char*)(words+15);
            if(a.CountBits(p,bits,-4,count)!=reference(p,bits,-4,count,a.Bias)) return 2;
        }
    }
    a.Bias=0;
    for(UInt b=0;b<32;++b) {
        words[0]=UInt(1)<<b;
        if(a.CountBits(words,32,4,1)!=b+1) return 3;
        --words[0];
        if(a.CountBits(words,32,4,1)!=b) return 4;
    }
    if(a.CountBits(0,32,4,0)) return 5;
    puts("00a62b70_TEST PASS"); return 0;
}