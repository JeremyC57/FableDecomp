// Behaviour test for CSpecialAbilitiesPhysicalShieldDef::operator= (0x0045FEE3): the base part and the eight
// float vectors go through their own operator=, every other member is copied, and *this is returned.
#include <cstdio>
static int g_baseCopies = 0;
static int g_vectorCopies = 0;
class CParentDefClassBase { public:
    long Base;
    CParentDefClassBase& operator=(const CParentDefClassBase& other) { Base = other.Base; ++g_baseCopies; return *this; }
};
class CDefString { public: long TablePos; };
struct CFloatVector {
    float Value;
    CFloatVector& operator=(const CFloatVector& other) { Value = other.Value; ++g_vectorCopies; return *this; }
};
class CSpecialAbilitiesPhysicalShieldDef : public CParentDefClassBase { public:
    CDefString InitSound; CDefString PowerUpSound[3]; CDefString CastSound; CDefString LoopingSound[4]; CDefString EndSound;
    float Health[4]; float LevelUpTimings[3]; float CamDist[4]; float CamHeight[4]; float CamFOV[4]; float CamSwitchTime[4];
    float CastCamDist, CastCamHeight, CastCamFOV, CastCamSwitchTime, MinWidth; CFloatVector MaxWidth;
    float BeenHitExtraWidth, BeenHitFadeOffTimeSecs, WobbleRate;
    CFloatVector ColorStartLevel1, ColorStartLevel2, ColorStartLevel3, ColorStartLevel4, ColorEnd, ColorHit;
    float FadeOutPhysicalShieldTimeSecs; CFloatVector ManaDamage; bool ShieldCoversCarriedItems;
    CSpecialAbilitiesPhysicalShieldDef& operator=(const CSpecialAbilitiesPhysicalShieldDef& other);
};
CSpecialAbilitiesPhysicalShieldDef& CSpecialAbilitiesPhysicalShieldDef::operator=(const CSpecialAbilitiesPhysicalShieldDef& other)
{
    CParentDefClassBase::operator=(other);
    InitSound = other.InitSound;
    for (int i = 0; i < 3; ++i) PowerUpSound[i] = other.PowerUpSound[i];
    CastSound = other.CastSound;
    for (int i = 0; i < 4; ++i) LoopingSound[i] = other.LoopingSound[i];
    EndSound = other.EndSound;
    for (int i = 0; i < 4; ++i) Health[i] = other.Health[i];
    for (int i = 0; i < 3; ++i) LevelUpTimings[i] = other.LevelUpTimings[i];
    for (int i = 0; i < 4; ++i) CamDist[i] = other.CamDist[i];
    for (int i = 0; i < 4; ++i) CamHeight[i] = other.CamHeight[i];
    for (int i = 0; i < 4; ++i) CamFOV[i] = other.CamFOV[i];
    for (int i = 0; i < 4; ++i) CamSwitchTime[i] = other.CamSwitchTime[i];
    CastCamDist = other.CastCamDist; CastCamHeight = other.CastCamHeight; CastCamFOV = other.CastCamFOV;
    CastCamSwitchTime = other.CastCamSwitchTime; MinWidth = other.MinWidth; MaxWidth = other.MaxWidth;
    BeenHitExtraWidth = other.BeenHitExtraWidth; BeenHitFadeOffTimeSecs = other.BeenHitFadeOffTimeSecs;
    WobbleRate = other.WobbleRate; ColorStartLevel1 = other.ColorStartLevel1; ColorStartLevel2 = other.ColorStartLevel2;
    ColorStartLevel3 = other.ColorStartLevel3; ColorStartLevel4 = other.ColorStartLevel4; ColorEnd = other.ColorEnd;
    ColorHit = other.ColorHit; FadeOutPhysicalShieldTimeSecs = other.FadeOutPhysicalShieldTimeSecs;
    ManaDamage = other.ManaDamage; ShieldCoversCarriedItems = other.ShieldCoversCarriedItems;
    return *this;
}
int main()
{
    CSpecialAbilitiesPhysicalShieldDef a, b;
    b.Base = 7; b.InitSound.TablePos = 11; b.CastSound.TablePos = 12; b.EndSound.TablePos = 13;
    for (int i = 0; i < 3; ++i) { b.PowerUpSound[i].TablePos = 20 + i; b.LevelUpTimings[i] = 1.5f + i; }
    for (int i = 0; i < 4; ++i) {
        b.LoopingSound[i].TablePos = 30 + i; b.Health[i] = 100.0f + i; b.CamDist[i] = 2.0f + i;
        b.CamHeight[i] = 3.0f + i; b.CamFOV[i] = 4.0f + i; b.CamSwitchTime[i] = 5.0f + i;
    }
    b.CastCamDist = 6; b.CastCamHeight = 7; b.CastCamFOV = 8; b.CastCamSwitchTime = 9; b.MinWidth = 10;
    b.MaxWidth.Value = 11; b.BeenHitExtraWidth = 12; b.BeenHitFadeOffTimeSecs = 13; b.WobbleRate = 14;
    b.ColorStartLevel1.Value = 15; b.ColorStartLevel2.Value = 16; b.ColorStartLevel3.Value = 17;
    b.ColorStartLevel4.Value = 18; b.ColorEnd.Value = 19; b.ColorHit.Value = 20;
    b.FadeOutPhysicalShieldTimeSecs = 21; b.ManaDamage.Value = 22; b.ShieldCoversCarriedItems = true;
    CSpecialAbilitiesPhysicalShieldDef& r = (a = b);
    bool ok = &r == &a && g_baseCopies == 1 && g_vectorCopies == 8 && a.Base == 7 && a.InitSound.TablePos == 11
        && a.CastSound.TablePos == 12 && a.EndSound.TablePos == 13 && a.CastCamSwitchTime == 9 && a.MinWidth == 10
        && a.MaxWidth.Value == 11 && a.WobbleRate == 14 && a.ColorHit.Value == 20 && a.ManaDamage.Value == 22
        && a.FadeOutPhysicalShieldTimeSecs == 21 && a.ShieldCoversCarriedItems;
    for (int i = 0; i < 3; ++i) ok = ok && a.PowerUpSound[i].TablePos == 20 + i && a.LevelUpTimings[i] == 1.5f + i;
    for (int i = 0; i < 4; ++i)
        ok = ok && a.LoopingSound[i].TablePos == 30 + i && a.Health[i] == 100.0f + i && a.CamDist[i] == 2.0f + i
            && a.CamHeight[i] == 3.0f + i && a.CamFOV[i] == 4.0f + i && a.CamSwitchTime[i] == 5.0f + i;
    if (!ok) { std::printf("0045fee3_TEST FAIL\n"); return 1; }
    std::printf("0045fee3_TEST PASS\n");
    return 0;
}