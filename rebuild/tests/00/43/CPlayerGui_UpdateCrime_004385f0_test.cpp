// Behaviour test for CPlayerGui::UpdateCrime (0x004385F0): an initialised bar animates towards the fine with a step count of
// max(1, (|old - current| - 0.2) * 0.5) truncated; the CurrentValue rounding (GFFloatToLongNear) is computed and discarded; an
// uninitialised bar takes the fine outright.
#include <stdio.h>
#include <math.h>
#include "engine/GFFloatToLongNear.h"
struct Bar { bool ValueInitialised; float CurrentValue, OldValue, LastOldValue; };
static float g_value, g_steps;
static void UpdateStat(float value, float steps) { g_value = value; g_steps = steps; }
static long Update(Bar& bar, long fine)
{
    long current = 0;
    if (bar.ValueInitialised) {
        current = GFFloatToLongNear(bar.CurrentValue);
        long steps = (long)((fabs(bar.OldValue - bar.CurrentValue) - 0.2f) * 0.5f);
        if (steps < 1)
            steps = 1;
        UpdateStat((float)fine, (float)steps);
    } else {
        float value = (float)fine;
        bar.CurrentValue = value;
        bar.OldValue = value;
        bar.LastOldValue = value;
        bar.ValueInitialised = true;
    }
    return current;
}
int main()
{
    Bar bar = { false, 0, 0, 0 };
    Update(bar, 40);
    bool ok = bar.ValueInitialised && bar.CurrentValue == 40.0f && bar.OldValue == 40.0f && bar.LastOldValue == 40.0f;
    bar.CurrentValue = 12.5f; bar.OldValue = 32.5f;
    long r = Update(bar, 100);
    ok = ok && r == 12 && g_value == 100.0f && g_steps == 9.0f;
    bar.CurrentValue = 13.5f; bar.OldValue = 13.6f;
    r = Update(bar, 7);
    ok = ok && r == 14 && g_steps == 1.0f;
    printf(ok ? "004385f0_TEST PASS\n" : "004385f0_TEST FAIL\n");
    return ok ? 0 : 1;
}