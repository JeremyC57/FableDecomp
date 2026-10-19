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