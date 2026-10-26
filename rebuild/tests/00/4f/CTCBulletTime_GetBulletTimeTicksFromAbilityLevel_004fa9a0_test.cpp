#include <cstdio>
#pragma optimize("y", off)
class CTCBulletTime { public: long GetBulletTimeTicksFromAbilityLevel(float value); };
long CTCBulletTime::GetBulletTimeTicksFromAbilityLevel(float value)
{
    long  result; float one; float half; float back;
    half = 0.5f; one = 1.0f;
    __asm fld value
    __asm fadd half
    __asm fistp result
    __asm fild result
    __asm fsub one
    __asm fstp back
    if (*(long*)&value == *(long*)&back)
        --result;
    __asm mov eax, result
}
int main()
{
    CTCBulletTime t;
    bool ok = t.GetBulletTimeTicksFromAbilityLevel(2.0f) == 2 && t.GetBulletTimeTicksFromAbilityLevel(2.25f) == 3
           && t.GetBulletTimeTicksFromAbilityLevel(2.75f) == 3 && t.GetBulletTimeTicksFromAbilityLevel(0.0f) == 0
           && t.GetBulletTimeTicksFromAbilityLevel(0.1f) == 1 && t.GetBulletTimeTicksFromAbilityLevel(-1.5f) == -1
           && t.GetBulletTimeTicksFromAbilityLevel(-2.0f) == -2 && t.GetBulletTimeTicksFromAbilityLevel(3.5f) == 4;   // 4.0 rounds even, (4-1)!=3.5 -> 4
    std::printf(ok ? "004fa9a0_TEST PASS\n" : "004fa9a0_TEST FAIL\n");
    return ok ? 0 : 1;
}