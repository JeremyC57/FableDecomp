#include <cstdio>
#ifdef _MSC_VER
#pragma optimize("y", off)
#include "engine/GFFloatToLongCeil.h"
#else
#include <math.h>
inline long GFFloatToLongCeil(float value) { return (long)ceil((double)value); }   // the FPU block is ceil() for the magnitudes used here
#endif
#ifdef _MSC_VER
#pragma optimize("y", off)
#include "engine/GFFloatToLongFloor.h"
#else
#include <math.h>
inline long GFFloatToLongFloor(float value) { return (long)floor((double)value); }   // the FPU block is floor() for the magnitudes used here
#endif
#include <math.h>
static float g_applied = -999.0f; static bool g_fromHit = false; static int g_calls = 0;
class CThingPhysical { public:
    void InternalModifyHealth(float amount, bool fromHit) { g_applied = amount; g_fromHit = fromHit; ++g_calls; }
    void ModifyHealth(float amount, bool fromHit)
    {
        if (amount < 0.0f) amount = (float)GFFloatToLongFloor(amount);
        else amount = (float)GFFloatToLongCeil(amount);
        if ((float)fabs(amount) > 1e-4f) InternalModifyHealth(amount, fromHit);
    }
};
int main()
{
    CThingPhysical t;
    t.ModifyHealth(2.3f, true);  bool ok = g_calls == 1 && g_applied == 3.0f && g_fromHit;
    t.ModifyHealth(-2.3f, false); ok = ok && g_calls == 2 && g_applied == -3.0f && !g_fromHit;
    t.ModifyHealth(0.0f, true);  ok = ok && g_calls == 2;
    t.ModifyHealth(-0.4f, true); ok = ok && g_calls == 3 && g_applied == -1.0f;
    t.ModifyHealth(5.0f, true);  ok = ok && g_calls == 4 && g_applied == 5.0f;
    std::printf(ok ? "006a68f0_TEST PASS\n" : "006a68f0_TEST FAIL\n");
    return ok ? 0 : 1;
}