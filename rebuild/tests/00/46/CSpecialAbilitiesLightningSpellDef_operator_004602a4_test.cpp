#include <cstdio>

static int g_baseCopies = 0;
static int g_vectorCopies = 0;
struct CParentDefClassBase {
    virtual ~CParentDefClassBase() {}
    int m_base;
    CParentDefClassBase& operator=(const CParentDefClassBase& o) { m_base = o.m_base; ++g_baseCopies; return *this; }
};
struct CFloatVector { int m_payload; CFloatVector& operator=(const CFloatVector& o) { m_payload = o.m_payload; ++g_vectorCopies; return *this; } };
struct CLongVector { int m_payload; CLongVector& operator=(const CLongVector& o) { m_payload = o.m_payload; ++g_vectorCopies; return *this; } };
struct CDefString { long m_index; };
struct CSpecialAbilitiesLightningSpellDef : CParentDefClassBase {          // vtable 0x0123460C
    CFloatVector m_faAreaEffect;
    CFloatVector m_faDamagePerHit;
    CFloatVector m_faSubBeamDamagePerHit;
    CFloatVector m_faActivationTime;
    CFloatVector m_faDeActivationTime;
    CLongVector m_naMinMainBeams;
    CLongVector m_naMaxMainBeams;
    CLongVector m_naMinSubBeams;
    CLongVector m_naMaxSubBeams;
    CFloatVector m_faTemporaryBeamLife;
    CFloatVector m_faTargettingFOV;
    CFloatVector m_faStaminaPerSecond;
    float m_fRandomBeamMinAngleX;
    float m_fRandomBeamMaxAngleX;
    float m_fRandomBeamMinAngleZ;
    float m_fRandomBeamMaxAngleZ;
    CFloatVector m_faMainBeamDecapitationTime;
    CFloatVector m_faSubBeamDecapitationTime;
    CFloatVector m_faSubBeamDelay;
    CFloatVector OnhitEveryNSecs;
    float CombatMultiplierIncreaseTime;
    CDefString InitSound;
    CDefString LoopingSound;
    CDefString EndHeroSound;
    CDefString EndAttackeeSound;
    float LightningThemeTransitionInSecs;
    float LightningThemeTransitionOutSecs;
    CSpecialAbilitiesLightningSpellDef& operator=(const CSpecialAbilitiesLightningSpellDef& other);
};

CSpecialAbilitiesLightningSpellDef& CSpecialAbilitiesLightningSpellDef::operator=(const CSpecialAbilitiesLightningSpellDef& other)
{
    CParentDefClassBase::operator=(other);
    m_faAreaEffect = other.m_faAreaEffect;
    m_faDamagePerHit = other.m_faDamagePerHit;
    m_faSubBeamDamagePerHit = other.m_faSubBeamDamagePerHit;
    m_faActivationTime = other.m_faActivationTime;
    m_faDeActivationTime = other.m_faDeActivationTime;
    m_naMinMainBeams = other.m_naMinMainBeams;
    m_naMaxMainBeams = other.m_naMaxMainBeams;
    m_naMinSubBeams = other.m_naMinSubBeams;
    m_naMaxSubBeams = other.m_naMaxSubBeams;
    m_faTemporaryBeamLife = other.m_faTemporaryBeamLife;
    m_faTargettingFOV = other.m_faTargettingFOV;
    m_faStaminaPerSecond = other.m_faStaminaPerSecond;
    m_fRandomBeamMinAngleX = other.m_fRandomBeamMinAngleX;
    m_fRandomBeamMaxAngleX = other.m_fRandomBeamMaxAngleX;
    m_fRandomBeamMinAngleZ = other.m_fRandomBeamMinAngleZ;
    m_fRandomBeamMaxAngleZ = other.m_fRandomBeamMaxAngleZ;
    m_faMainBeamDecapitationTime = other.m_faMainBeamDecapitationTime;
    m_faSubBeamDecapitationTime = other.m_faSubBeamDecapitationTime;
    m_faSubBeamDelay = other.m_faSubBeamDelay;
    OnhitEveryNSecs = other.OnhitEveryNSecs;
    CombatMultiplierIncreaseTime = other.CombatMultiplierIncreaseTime;
    InitSound = other.InitSound;
    LoopingSound = other.LoopingSound;
    EndHeroSound = other.EndHeroSound;
    EndAttackeeSound = other.EndAttackeeSound;
    LightningThemeTransitionInSecs = other.LightningThemeTransitionInSecs;
    LightningThemeTransitionOutSecs = other.LightningThemeTransitionOutSecs;
    return *this;
}

int main()
{
    CSpecialAbilitiesLightningSpellDef a, b;
    a.m_base = 1; b.m_base = 2;
    a.m_faAreaEffect.m_payload = 0; b.m_faAreaEffect.m_payload = 10;
    a.m_faDamagePerHit.m_payload = 0; b.m_faDamagePerHit.m_payload = 11;
    a.m_faSubBeamDamagePerHit.m_payload = 0; b.m_faSubBeamDamagePerHit.m_payload = 12;
    a.m_faActivationTime.m_payload = 0; b.m_faActivationTime.m_payload = 13;
    a.m_faDeActivationTime.m_payload = 0; b.m_faDeActivationTime.m_payload = 14;
    a.m_naMinMainBeams.m_payload = 0; b.m_naMinMainBeams.m_payload = 15;
    a.m_naMaxMainBeams.m_payload = 0; b.m_naMaxMainBeams.m_payload = 16;
    a.m_naMinSubBeams.m_payload = 0; b.m_naMinSubBeams.m_payload = 17;
    a.m_naMaxSubBeams.m_payload = 0; b.m_naMaxSubBeams.m_payload = 18;
    a.m_faTemporaryBeamLife.m_payload = 0; b.m_faTemporaryBeamLife.m_payload = 19;
    a.m_faTargettingFOV.m_payload = 0; b.m_faTargettingFOV.m_payload = 20;
    a.m_faStaminaPerSecond.m_payload = 0; b.m_faStaminaPerSecond.m_payload = 21;
    a.m_fRandomBeamMinAngleX = 0; b.m_fRandomBeamMinAngleX = (float)22;
    a.m_fRandomBeamMaxAngleX = 0; b.m_fRandomBeamMaxAngleX = (float)23;
    a.m_fRandomBeamMinAngleZ = 0; b.m_fRandomBeamMinAngleZ = (float)24;
    a.m_fRandomBeamMaxAngleZ = 0; b.m_fRandomBeamMaxAngleZ = (float)25;
    a.m_faMainBeamDecapitationTime.m_payload = 0; b.m_faMainBeamDecapitationTime.m_payload = 26;
    a.m_faSubBeamDecapitationTime.m_payload = 0; b.m_faSubBeamDecapitationTime.m_payload = 27;
    a.m_faSubBeamDelay.m_payload = 0; b.m_faSubBeamDelay.m_payload = 28;
    a.OnhitEveryNSecs.m_payload = 0; b.OnhitEveryNSecs.m_payload = 29;
    a.CombatMultiplierIncreaseTime = 0; b.CombatMultiplierIncreaseTime = (float)30;
    a.InitSound.m_index = 0; b.InitSound.m_index = 31;
    a.LoopingSound.m_index = 0; b.LoopingSound.m_index = 32;
    a.EndHeroSound.m_index = 0; b.EndHeroSound.m_index = 33;
    a.EndAttackeeSound.m_index = 0; b.EndAttackeeSound.m_index = 34;
    a.LightningThemeTransitionInSecs = 0; b.LightningThemeTransitionInSecs = (float)35;
    a.LightningThemeTransitionOutSecs = 0; b.LightningThemeTransitionOutSecs = (float)36;
    CSpecialAbilitiesLightningSpellDef& r = (a = b);
    int bad = (&r != &a) || a.m_base != 2 || g_baseCopies != 1 || g_vectorCopies != 16;
    bad |= a.m_faAreaEffect.m_payload != 10;
    bad |= a.m_faDamagePerHit.m_payload != 11;
    bad |= a.m_faSubBeamDamagePerHit.m_payload != 12;
    bad |= a.m_faActivationTime.m_payload != 13;
    bad |= a.m_faDeActivationTime.m_payload != 14;
    bad |= a.m_naMinMainBeams.m_payload != 15;
    bad |= a.m_naMaxMainBeams.m_payload != 16;
    bad |= a.m_naMinSubBeams.m_payload != 17;
    bad |= a.m_naMaxSubBeams.m_payload != 18;
    bad |= a.m_faTemporaryBeamLife.m_payload != 19;
    bad |= a.m_faTargettingFOV.m_payload != 20;
    bad |= a.m_faStaminaPerSecond.m_payload != 21;
    bad |= a.m_fRandomBeamMinAngleX != (float)22;
    bad |= a.m_fRandomBeamMaxAngleX != (float)23;
    bad |= a.m_fRandomBeamMinAngleZ != (float)24;
    bad |= a.m_fRandomBeamMaxAngleZ != (float)25;
    bad |= a.m_faMainBeamDecapitationTime.m_payload != 26;
    bad |= a.m_faSubBeamDecapitationTime.m_payload != 27;
    bad |= a.m_faSubBeamDelay.m_payload != 28;
    bad |= a.OnhitEveryNSecs.m_payload != 29;
    bad |= a.CombatMultiplierIncreaseTime != (float)30;
    bad |= a.InitSound.m_index != 31;
    bad |= a.LoopingSound.m_index != 32;
    bad |= a.EndHeroSound.m_index != 33;
    bad |= a.EndAttackeeSound.m_index != 34;
    bad |= a.LightningThemeTransitionInSecs != (float)35;
    bad |= a.LightningThemeTransitionOutSecs != (float)36;
    if (bad) { std::printf("004602a4_TEST FAIL\n"); return 1; }
    std::printf("004602a4_TEST PASS\n");
    return 0;
}