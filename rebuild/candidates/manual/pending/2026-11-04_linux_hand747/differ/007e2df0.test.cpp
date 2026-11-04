// Behaviour test for CTCWobble::OnInitialActivate: the start angle is the wobble range scaled and rounded up (GFFloatToLongCeil) (parked row 0x007E2DF0).
#include <stdio.h>
#include "engine/GFFloatToLongCeil.h"
static long Angle(float range, float scale) { return GFFloatToLongCeil(range * scale); }
int main()
{
    bool ok = Angle(1.5f, 1.0f) == 2 && Angle(2.0f, 1.0f) == 2 && Angle(0.25f, 4.0f) == 1;
    printf(ok ? "007e2df0_TEST PASS\n" : "007e2df0_TEST FAIL\n");
    return ok ? 0 : 1;
}
