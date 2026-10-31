// Test for 0x0071BA60 CTCHeroMorph::GetMorphStrength(EHeroMorphType) const -- the switch logic, incl. the case 9 clamp
#include <stdio.h>
#include <math.h>
template <class T> const T& GFLimitReturn(const T& value, const T& low, const T& high)
{
    if (value < low) return low;
    if (value > high) return high;
    return value;
}
enum EHeroMorphType { MT_0 };
class CTCHeroMorph { public:
    float Strength, Berserk, Will, Skill, Age, Morality, Fatness, Tan;
    bool Teenager;
    float GetMorphStrength(EHeroMorphType type) const;
};
float CTCHeroMorph::GetMorphStrength(EHeroMorphType type) const
{
    switch (type) {
    case 0: return 0.0f;
    case 1: if (Tan >= 0.5f) return (Tan - 0.5f) * 2.0f; break;
    case 2: if (Tan < 0.5f) return (0.5f - Tan) * 2.0f; break;
    case 4: { float v = Age - 0.4f; if (v < 0.0f) v = 0.0f; return v * (1.0f / 0.6f); }
    case 5: { float v = Age - 0.2f; if (v < 0.0f) v = 0.0f; return v * (1.0f / 0.8f); }
    case 17: return Berserk;
    case 6: if (Morality < 0.5f) return (0.5f - Morality) * 2.0f; break;
    case 7: if (Morality >= 0.5f) return (Morality - 0.5f) * 2.0f; break;
    case 9: { float difference; return GFLimitReturn(difference = GetMorphStrength((EHeroMorphType)8) - Fatness, 0.0f, 1.0f); }
    case 10: return Will;
    case 11: { float difference = Strength - Skill; float high = 1.0f; float low = 0.0f; return GFLimitReturn(difference, low, high); }
    case 12: if (Strength >= 0.5f) return (Strength - 0.5f) * 2.0f; break;
    case 13: if (Strength < 0.5f) return (0.5f - Strength) * 2.0f; break;
    case 14: return Fatness;
    case 8: case 15: return Skill;
    case 16: return GetMorphStrength((EHeroMorphType)6);
    case 18: return Teenager ? 1.0f : 0.0f;
    }
    return 0.0f;
}
static int near(float a, float b) { return fabs(a - b) < 1e-5f; }
int main()
{
    CTCHeroMorph m;
    m.Strength = 0.8f; m.Berserk = 0.3f; m.Will = 0.6f; m.Skill = 0.9f; m.Age = 0.7f;
    m.Morality = 0.2f; m.Fatness = 0.4f; m.Tan = 0.75f; m.Teenager = true;
    int ok = 1;
    ok &= near(m.GetMorphStrength((EHeroMorphType)0), 0.0f);
    ok &= near(m.GetMorphStrength((EHeroMorphType)1), 0.5f);
    ok &= near(m.GetMorphStrength((EHeroMorphType)2), 0.0f);
    ok &= near(m.GetMorphStrength((EHeroMorphType)3), 0.0f);
    ok &= near(m.GetMorphStrength((EHeroMorphType)4), 0.5f);
    ok &= near(m.GetMorphStrength((EHeroMorphType)5), 0.625f);
    ok &= near(m.GetMorphStrength((EHeroMorphType)6), 0.6f);
    ok &= near(m.GetMorphStrength((EHeroMorphType)7), 0.0f);
    ok &= near(m.GetMorphStrength((EHeroMorphType)9), 0.5f);    // skill 0.9 - fatness 0.4
    m.Fatness = 1.5f;
    ok &= near(m.GetMorphStrength((EHeroMorphType)9), 0.0f);    // clamped low
    m.Fatness = -0.5f;
    ok &= near(m.GetMorphStrength((EHeroMorphType)9), 1.0f);    // clamped high
    ok &= near(m.GetMorphStrength((EHeroMorphType)11), 0.0f);   // 0.8 - 0.9 clamped
    m.Skill = 0.3f;
    ok &= near(m.GetMorphStrength((EHeroMorphType)11), 0.5f);
    ok &= near(m.GetMorphStrength((EHeroMorphType)12), 0.6f);
    ok &= near(m.GetMorphStrength((EHeroMorphType)13), 0.0f);
    ok &= near(m.GetMorphStrength((EHeroMorphType)14), -0.5f);
    ok &= near(m.GetMorphStrength((EHeroMorphType)15), 0.3f);
    ok &= near(m.GetMorphStrength((EHeroMorphType)16), 0.6f);
    ok &= near(m.GetMorphStrength((EHeroMorphType)17), 0.3f);
    ok &= near(m.GetMorphStrength((EHeroMorphType)18), 1.0f);
    ok &= near(m.GetMorphStrength((EHeroMorphType)19), 0.0f);
    printf(ok ? "0071ba60_TEST PASS\n" : "0071ba60_TEST FAIL\n");
    return ok ? 0 : 1;
}