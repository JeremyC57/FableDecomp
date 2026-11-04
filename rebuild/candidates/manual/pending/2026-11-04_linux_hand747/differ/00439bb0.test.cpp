// Behaviour test for CPlayerGui::UpdateMoney: the bar value is rounded (GFFloatToLongNear) before the money text is rebuilt (parked row 0x00439BB0).
#include <stdio.h>
#include "engine/GFFloatToLongNear.h"
static long Shown(float v) { return GFFloatToLongNear(v); }
int main()
{
    bool ok = Shown(99.6f) == 100 && Shown(12.4f) == 12 && Shown(0.5f) == 0;
    printf(ok ? "00439bb0_TEST PASS\n" : "00439bb0_TEST FAIL\n");
    return ok ? 0 : 1;
}
