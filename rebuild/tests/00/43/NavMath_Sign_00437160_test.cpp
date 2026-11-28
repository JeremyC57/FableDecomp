// Leaf: ?GFGetSign@@YIJM@Z
// Flags:
#define FABLE_RETAIL_NAV_ROUTE_SIGN_PASS30
#include "rebuild_abi.h"
#include "engine/GFAngles.h"

long FABLE_FASTCALL GFGetSign(float value)
{
    return value < 0.0f ? -1 : 1;
}

#include <stdio.h>
#include <string.h>
int main() {
    const unsigned bits[]={0,0x80000000,0x7f800000,0xff800000,0x7fc00001,0x3f800000,0xbf800000};
    const long expected[]={1,1,1,-1,1,1,-1};
    for(unsigned i=0;i<sizeof(bits)/sizeof(bits[0]);++i) {float value;memcpy(&value,&bits[i],4);if(GFGetSign(value)!=expected[i])return 1;}
    puts("00437160_TEST PASS");return 0;
}
