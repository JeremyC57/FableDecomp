// Behaviour test for 0x0078C210 CTCBerserk::CTCBerserk (level-0 def copies, GFFloatToLongCeil timer and its clamp, strength share)
#include <stdio.h>

static long CeilModel(float value)          // GFFloatToLongCeil on values off the rounding boundary
{
    long r = (long)value;
    if ((float)r < value) ++r;
    return r;
}
struct Def { long FramesToFadeOut; float Duration, ToMe, ToEnemy, MaxStrength, Scale, R0, R1, R2, R3; };
struct Berserk {
    long Timer; float StrengthPercentage, MaxIncreaseInStrength, BerserkDamageToMe, BerserkDamageToEnemy, BerserkScale;
    float RumbleLevel, RumbleSmoothness, RumbleAttack, RumbleDecay; long SpellLevel; bool JustReleased, Pausable;
    long Flourish;
    Berserk(const Def& def, long fps, bool hasExp, long level, long maxLevel, bool hasCombat, bool hasAbilities, long abilityLevel)
        : SpellLevel(0), JustReleased(true), Pausable(true), Flourish(-1)
    {
        Timer = CeilModel(fps * def.Duration);
        BerserkDamageToMe = def.ToMe; BerserkDamageToEnemy = def.ToEnemy; MaxIncreaseInStrength = def.MaxStrength;
        BerserkScale = def.Scale; RumbleLevel = def.R0; RumbleSmoothness = def.R1; RumbleAttack = def.R2; RumbleDecay = def.R3;
        if (Timer <= def.FramesToFadeOut) Timer = def.FramesToFadeOut + 1;
        if (hasExp) { long m = maxLevel; StrengthPercentage = (float)level / m; } else StrengthPercentage = 0.0f;
        if (hasCombat) Flourish = 77;
        if (hasAbilities) SpellLevel = abilityLevel;
    }
};
int main()
{
    int fails = 0;
    Def d = { 10, 2.3f, 1.5f, 2.0f, 0.25f, 1.1f, 0.1f, 0.2f, 0.3f, 0.4f };
    Berserk a(d, 30, true, 3, 12, true, true, 2);
    if (a.Timer != 69 || a.StrengthPercentage != 0.25f || a.SpellLevel != 2 || a.Flourish != 77) ++fails;
    if (a.BerserkDamageToMe != 1.5f || a.RumbleDecay != 0.4f || !a.JustReleased || !a.Pausable) ++fails;
    Def e = d; e.FramesToFadeOut = 100;
    Berserk b(e, 30, false, 0, 1, false, false, 5);
    if (b.Timer != 101 || b.StrengthPercentage != 0.0f || b.SpellLevel != 0 || b.Flourish != -1) ++fails;
    if (fails) { printf("0078c210_TEST FAIL %d\n", fails); return 1; }
    printf("0078c210_TEST PASS\n");
    return 0;
}