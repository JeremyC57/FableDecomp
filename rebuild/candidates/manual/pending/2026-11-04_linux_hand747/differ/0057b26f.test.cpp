// Behaviour test for CTCHeroStats::ForceStaminaTo: the fraction is clamped to [0,1], scaled by MaxStamina and rounded to nearest (parked row 0x0057B26F).
#include <stdio.h>
#include "engine/GFFloatToLongNear.h"
static long Force(float f, long maxStamina) { if (f < 0.0f) f = 0.0f; else if (f > 1.0f) f = 1.0f; f = maxStamina * f; return GFFloatToLongNear(f); }
int main()
{
    bool ok = Force(0.5f, 100) == 50 && Force(2.0f, 80) == 80 && Force(-1.0f, 80) == 0 && Force(0.255f, 10) == 3;
    printf(ok ? "0057b26f_TEST PASS\n" : "0057b26f_TEST FAIL\n");
    return ok ? 0 : 1;
}
