// behaviour test for 0x0045FE2B CSpecialAbilitiesEnflameDef::operator=
#include <cstdio>
static int g_copies = 0;
struct CParentDefClassBase {
    int baseTag;
    CParentDefClassBase& operator=(const CParentDefClassBase& o) { baseTag = o.baseTag; return *this; }
};
struct CFloatVector { float v; CFloatVector& operator=(const CFloatVector& o) { v = o.v; ++g_copies; return *this; } };
struct CSpecialAbilitiesEnflameDef : CParentDefClassBase {
    float CamDist[4], CamHeight[4], CamFOV[4], CamSwitchTime[4], LevelUpTimings[3], Damage[4];
    CFloatVector RadiusOfEffect;
    float EnflameCombatMultiplierFactor;
    CSpecialAbilitiesEnflameDef& operator=(const CSpecialAbilitiesEnflameDef& other)
    {
        CParentDefClassBase::operator=(other);
        long i;
        for (i = 0; i < 4; ++i) CamDist[i] = other.CamDist[i];
        for (i = 0; i < 4; ++i) CamHeight[i] = other.CamHeight[i];
        for (i = 0; i < 4; ++i) CamFOV[i] = other.CamFOV[i];
        for (i = 0; i < 4; ++i) CamSwitchTime[i] = other.CamSwitchTime[i];
        for (i = 0; i < 3; ++i) LevelUpTimings[i] = other.LevelUpTimings[i];
        for (i = 0; i < 4; ++i) Damage[i] = other.Damage[i];
        RadiusOfEffect = other.RadiusOfEffect;
        EnflameCombatMultiplierFactor = other.EnflameCombatMultiplierFactor;
        return *this;
    }
};
int main()
{
    CSpecialAbilitiesEnflameDef a, b;
    for (int i = 0; i < 4; ++i) {
        b.CamDist[i] = 1.0f + i; b.CamHeight[i] = 2.0f + i; b.CamFOV[i] = 3.0f + i; b.CamSwitchTime[i] = 4.0f + i; b.Damage[i] = 5.0f + i;
        a.CamDist[i] = a.CamHeight[i] = a.CamFOV[i] = a.CamSwitchTime[i] = a.Damage[i] = 0;
        if (i < 3) { b.LevelUpTimings[i] = 6.0f + i; a.LevelUpTimings[i] = 0; }
    }
    b.baseTag = 8; b.RadiusOfEffect.v = 2.5f; b.EnflameCombatMultiplierFactor = 1.25f;
    a.baseTag = 0; a.RadiusOfEffect.v = 0; a.EnflameCombatMultiplierFactor = 0;
    CSpecialAbilitiesEnflameDef* r = &(a = b);
    if (r != &a || a.baseTag != 8 || a.CamDist[3] != 4.0f || a.CamHeight[0] != 2.0f || a.CamFOV[2] != 5.0f
        || a.CamSwitchTime[1] != 5.0f || a.LevelUpTimings[2] != 8.0f || a.Damage[3] != 8.0f
        || a.RadiusOfEffect.v != 2.5f || a.EnflameCombatMultiplierFactor != 1.25f || g_copies != 1)
    { std::printf("0045fe2b_TEST FAIL\n"); return 1; }
    std::printf("0045fe2b_TEST PASS\n");
    return 0;
}