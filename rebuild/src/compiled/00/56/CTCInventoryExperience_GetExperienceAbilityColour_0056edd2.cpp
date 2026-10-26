// CTCInventoryExperience::GetExperienceAbilityColour(EHeroAbility ability) -- the spend-screen colour of an ability's category
// Retail 0x0056EDD2, 248 bytes, ret 8 (hidden CRGBFloatColour result + ability). Three local ability tables
// are filled in declaration order (strength 3,16,12,9,11,18,19; skill 1,5,2,6,10; will 4,8,13,14,15,17) and
// scanned linearly: the strength scan stops after FIVE entries (retail shares `push 5 ; pop ecx` between the
// strength and skill bounds, so the last two strength ids 18/19 never colour), then skill, then will (6).
// Colours: strength (0.5, 0, 1, 1), skill (0.1, 0.1, 0.7, 1), will (0.5, 0.5, 1, 1), none (0, 0, 1, 1);
// each return is an inline four-float constructor into the hidden result (fldz/fld1/0.5f/0.1f pool
// 0x0122F59C/0x0122EE58), which VC7.1 tail-merges across the arms. Frame-pointer body: `optimize("y", off)`.
#pragma optimize("y", off)
#pragma optimize("s", on)

class CRGBFloatColour { public:
    float R;                                       // +0x00
    float G;                                       // +0x04
    float B;                                       // +0x08
    float A;                                       // +0x0c
    CRGBFloatColour(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) {}
};

enum EHeroAbility { EHeroAbility_Force32 = 0x7fffffff };

class CTCInventoryExperience { public:
    CRGBFloatColour GetExperienceAbilityColour(EHeroAbility ability);
};

CRGBFloatColour CTCInventoryExperience::GetExperienceAbilityColour(EHeroAbility ability)
{
    long strength[7] = { 3, 16, 12, 9, 11, 18, 19 };
    long skill[5] = { 1, 5, 2, 6, 10 };
    long will[6] = { 4, 8, 13, 14, 15, 17 };
    long i;
    for (i = 0; i != 5; ++i)
        if (strength[i] == ability)
            return CRGBFloatColour(0.5f, 0.0f, 1.0f, 1.0f);
    for (i = 0; i != 5; ++i)
        if (skill[i] == ability)
            return CRGBFloatColour(0.1f, 0.1f, 0.7f, 1.0f);
    for (i = 0; i != 6; ++i)
        if (will[i] == ability)
            return CRGBFloatColour(0.5f, 0.5f, 1.0f, 1.0f);
    return CRGBFloatColour(0.0f, 0.0f, 1.0f, 1.0f);
}