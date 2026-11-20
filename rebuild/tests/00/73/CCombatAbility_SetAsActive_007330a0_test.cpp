// Behaviour test for CCombatAbility::SetAsActive (0x007330A0): links the row object; ActiveTimer = (long)(ConstantFPS * SecondsDuration), the float product
// truncated toward zero (__ftol2); SecondsDuration is CCombatAbilityData +0x8; the other timer and the data are left alone.
#include <stdio.h>
#include <string.h>
extern "C" long g_TestConstantFPS = 30;
#pragma comment(linker, "/alternatename:?ConstantFPS@CMainGameComponent@@1JA=_g_TestConstantFPS")
extern "C" void __fastcall RowSetAs(void* self, int unused, const void* combat_ability);
#pragma comment(linker, "/alternatename:@RowSetAs@12=?SetAsActive@CCombatAbility@@QAEXABVCCombatAbilityData@@@Z")
static bool Run(long fps, float seconds, float otherField)
{
    g_TestConstantFPS = fps;
    float data[6] = { 0, 77.0f, 77.0f, 77.0f, 0, 0 };            // vptr, PercentageChance (as float bits, unused), SecondsDuration, SecondsToWaitBeforeRepeat, Anim, InterfaceType
    data[8 / 4] = seconds; data[8 == 8 ? 3 : 2] = otherField;
    float data0[6]; memcpy(data0, data, sizeof data);
    long ab[4] = { 0x0BADF00D, 0x0BADF00D, 0x33333333, 0x44444444 }, ab0[4]; memcpy(ab0, ab, sizeof ab);
    RowSetAs(ab, 0, data);
    long want = (long)((float)fps * seconds);
    ab0[1] = want;
    bool ok = !memcmp(ab, ab0, sizeof ab) && !memcmp(data, data0, sizeof data);
    printf("fps %ld seconds %g: timer %ld want %ld %s\n", fps, seconds, ab[1], want, ok ? "ok" : "FAIL");
    return ok;
}
int main()
{
    bool ok = true;
    ok &= Run(30, 1.5f, 9.0f);
    ok &= Run(30, 0.0f, 9.0f);
    ok &= Run(30, 2.99f, -4.0f);
    ok &= Run(30, -1.5f, 2.0f);
    ok &= Run(25, 0.333f, 100.0f);
    ok &= Run(60, 12.25f, 0.5f);
    printf(ok ? "007330a0_TEST PASS\n" : "007330a0_TEST FAIL\n");
    return ok ? 0 : 1;
}