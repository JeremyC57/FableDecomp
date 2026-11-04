// Behaviour test for CMiniMapDisplay::GetDrawPosition: the map position is scaled into the box and rounded to whole pixels (parked row 0x0082A020).
#include <stdio.h>
#include "engine/GFFloatToLongNear.h"
static long Pix(float x, float scale, float origin) { return GFFloatToLongNear((x - origin) * scale); }
int main()
{
    bool ok = Pix(10.0f, 2.0f, 0.0f) == 20 && Pix(1.25f, 2.0f, 0.0f) == 2 && Pix(5.0f, 0.5f, 2.0f) == 2;
    printf(ok ? "0082a020_TEST PASS\n" : "0082a020_TEST FAIL\n");
    return ok ? 0 : 1;
}
