#include <cstdio>
#ifdef _MSC_VER
#pragma optimize("y", off)
__forceinline long CeilToLong(float value)
{
    long result; float one; float half; float back;
    half = 0.5f; one = 1.0f;
    __asm {
        fld value
        fadd half
        fistp result
        fild result
        fsub one
        fstp back
        mov eax, value
        cmp eax, back
        jne done
        dec result
    done:
    }
    return result;
}
#else
#include <math.h>
inline long CeilToLong(float value) { return (long)ceil((double)value); }   // the FPU block is ceil() for the magnitudes used here
#endif
class CSpecialAbilitiesBerserkDef { public:
    unsigned char _pad00[0x7c]; float* Duration; unsigned char _pad80[0x88 - 0x80]; float* DamageToEnemy; unsigned char _pad8c[0x98 - 0x8c]; float* DamageToMe;
    unsigned char _pad9c[0xa4 - 0x9c]; float* MaxIncreaseInStrength; unsigned char _pada8[0xb0 - 0xa8]; float* Scale; unsigned char _padb4[0xf0 - 0xb4]; float* RumbleLevel;
    unsigned char _padf4[0xfc - 0xf4]; float* RumbleSmoothness; unsigned char _pad100[0x108 - 0x100]; float* RumbleAttack; unsigned char _pad10c[0x114 - 0x10c]; float* RumbleDecay;
};
long g_FramesPerSecond = 15;
class CTCBerserk { public:
    void* __vftable; unsigned char _base04[8]; long Timer; long DarkenEnvTimer; float StrengthPercentage; float MaxIncreaseInStrength; float ReleasedAnimationTimer; long AnimTimer;
    float BerserkDamageToMe; float BerserkDamageToEnemy; float BerserkScale; float RumbleLevel; float RumbleSmoothness; float RumbleAttack; float RumbleDecay; unsigned long BerserkingLoopingSound;
    long SpellLevel; unsigned char _pad48[0x68 - 0x48]; const CSpecialAbilitiesBerserkDef* PDef_Object;
    const CSpecialAbilitiesBerserkDef* GetDef() const { return PDef_Object; }
    void SetLevel()
    {
        Timer = CeilToLong(g_FramesPerSecond * GetDef()->Duration[SpellLevel]);
        BerserkDamageToMe = GetDef()->DamageToMe[SpellLevel];
        BerserkDamageToEnemy = GetDef()->DamageToEnemy[SpellLevel];
        MaxIncreaseInStrength = GetDef()->MaxIncreaseInStrength[SpellLevel];
        BerserkScale = GetDef()->Scale[SpellLevel];
        RumbleLevel = GetDef()->RumbleLevel[SpellLevel];
        RumbleSmoothness = GetDef()->RumbleSmoothness[SpellLevel];
        RumbleAttack = GetDef()->RumbleAttack[SpellLevel];
        RumbleDecay = GetDef()->RumbleDecay[SpellLevel];
    }
};
int main()
{
    static CSpecialAbilitiesBerserkDef def; static CTCBerserk b;
    float dur[3] = { 1.0f, 2.1f, 3.0f }, dte[3] = { 1, 2, 3 }, dtm[3] = { 4, 5, 6 }, mis[3] = { 7, 8, 9 }, sc[3] = { 10, 11, 12 }, rl[3] = { 13, 14, 15 }, rs[3] = { 16, 17, 18 }, ra[3] = { 19, 20, 21 }, rd[3] = { 22, 23, 24 };
    def.Duration = dur; def.DamageToEnemy = dte; def.DamageToMe = dtm; def.MaxIncreaseInStrength = mis; def.Scale = sc; def.RumbleLevel = rl; def.RumbleSmoothness = rs; def.RumbleAttack = ra; def.RumbleDecay = rd;
    b.PDef_Object = &def; b.SpellLevel = 1; b.SetLevel();
    bool ok = b.Timer == 32 && b.BerserkDamageToMe == 5.0f && b.BerserkDamageToEnemy == 2.0f && b.MaxIncreaseInStrength == 8.0f && b.BerserkScale == 11.0f
           && b.RumbleLevel == 14.0f && b.RumbleSmoothness == 17.0f && b.RumbleAttack == 20.0f && b.RumbleDecay == 23.0f;
    b.SpellLevel = 0; b.SetLevel(); ok = ok && b.Timer == 15 && b.RumbleDecay == 22.0f;
    std::printf(ok ? "0078bf00_TEST PASS\n" : "0078bf00_TEST FAIL\n");
    return ok ? 0 : 1;
}