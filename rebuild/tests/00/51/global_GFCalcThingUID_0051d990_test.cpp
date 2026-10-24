// Behaviour test for 0x0051D990 GFCalcThingUID: mapUID * 2^40 + thingID, wrapping in 64 bits.
#include <cstdio>
#ifdef _MSC_VER
typedef unsigned __int64 u64;
#define K64(x) x##ui64
#else
#define __fastcall
typedef unsigned long long u64;
#define K64(x) x##ULL
#endif

u64 __fastcall GFCalcThingUID(u64 mapUID, u64 thingID)
{
    return mapUID * K64(0x10000000000) + thingID;
}

int main()
{
    int ok = 1;
    ok &= GFCalcThingUID(0, 0) == 0;
    ok &= GFCalcThingUID(1, 5) == K64(0x10000000005);
    ok &= GFCalcThingUID(0xfffffe, K64(0x123456789A)) == K64(0xfffffe123456789A);
    ok &= GFCalcThingUID(0x1000001, 7) == K64(0x0000010000000007);   // bits above 63 drop
    ok &= GFCalcThingUID(2, K64(0x20000000000)) == K64(0x40000000000);  // id overflow carries into map bits
    if (!ok) { std::printf("0051d990_TEST FAIL\n"); return 1; }
    std::printf("0051d990_TEST PASS\n");
    return 0;
}