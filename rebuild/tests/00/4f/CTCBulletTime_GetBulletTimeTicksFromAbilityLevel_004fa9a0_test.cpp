#include <cstdio>
#include "engine/GFFloatToLongCeil.h"
int main()
{
    bool ok = GFFloatToLongCeil(2.0f) == 2 && GFFloatToLongCeil(2.25f) == 3
           && GFFloatToLongCeil(2.75f) == 3 && GFFloatToLongCeil(0.0f) == 0
           && GFFloatToLongCeil(0.1f) == 1 && GFFloatToLongCeil(-1.5f) == -1
           && GFFloatToLongCeil(-2.0f) == -2 && GFFloatToLongCeil(3.5f) == 4;   // 4.0 rounds even, (4-1)!=3.5 -> 4
    std::printf(ok ? "004fa9a0_TEST PASS\n" : "004fa9a0_TEST FAIL\n");
    return ok ? 0 : 1;
}
