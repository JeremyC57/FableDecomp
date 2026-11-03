#include <cstdio>
#include "engine/GFFloatToLongFloor.h"
int main()
{
    bool ok = GFFloatToLongFloor(2.0f) == 2 && GFFloatToLongFloor(2.7f) == 2 && GFFloatToLongFloor(3.0f) == 3
           && GFFloatToLongFloor(0.0f) == 0 && GFFloatToLongFloor(-0.5f) == -1 && GFFloatToLongFloor(-2.0f) == -2
           && GFFloatToLongFloor(2.5f) == 2 && GFFloatToLongFloor(1000.25f) == 1000;
    std::printf(ok ? "0048d940_TEST PASS\n" : "0048d940_TEST FAIL\n");
    return ok ? 0 : 1;
}