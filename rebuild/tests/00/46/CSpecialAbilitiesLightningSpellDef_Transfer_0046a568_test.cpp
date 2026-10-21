#include <cstdio>
// behaviour test: CSpecialAbilitiesLightningSpellDef::Transfer visits sounds, angles, vectors, then the trailing floats
struct CDefString { long m_index; };
template <class T> struct CStlVector { T* _M_start; T* _M_finish; T* _M_end_of_storage; };
struct CPersistContext {
    const void* order[40];
    int kind[40];
    int count;
    void Transfer(CDefString& v) { kind[count] = 1; order[count++] = &v; }
    void Transfer(float& v) { kind[count] = 2; order[count++] = &v; }
    void Transfer(CStlVector<float>& v) { kind[count] = 3; order[count++] = &v; }
    void Transfer(CStlVector<long>& v) { kind[count] = 4; order[count++] = &v; }
};
struct CSpecialAbilitiesLightningSpellDef {
    virtual void Transfer(CPersistContext& context);
    unsigned char _base[0x38];
    CStlVector<float> m_faAreaEffect, m_faDamagePerHit, m_faSubBeamDamagePerHit, m_faActivationTime, m_faDeActivationTime;
    CStlVector<long> m_naMinMainBeams, m_naMaxMainBeams, m_naMinSubBeams, m_naMaxSubBeams;
    CStlVector<float> m_faTemporaryBeamLife, m_faTargettingFOV, m_faStaminaPerSecond;
    float m_fRandomBeamMinAngleX, m_fRandomBeamMaxAngleX, m_fRandomBeamMinAngleZ, m_fRandomBeamMaxAngleZ;
    CStlVector<float> m_faMainBeamDecapitationTime, m_faSubBeamDecapitationTime, m_faSubBeamDelay, OnhitEveryNSecs;
    float CombatMultiplierIncreaseTime;
    CDefString InitSound, LoopingSound, EndHeroSound, EndAttackeeSound;
    float LightningThemeTransitionInSecs, LightningThemeTransitionOutSecs;
};
void CSpecialAbilitiesLightningSpellDef::Transfer(CPersistContext& context)
{
    context.Transfer(InitSound);
    context.Transfer(LoopingSound);
    context.Transfer(EndHeroSound);
    context.Transfer(EndAttackeeSound);
    context.Transfer(m_fRandomBeamMinAngleX);
    context.Transfer(m_fRandomBeamMaxAngleX);
    context.Transfer(m_fRandomBeamMinAngleZ);
    context.Transfer(m_fRandomBeamMaxAngleZ);
    context.Transfer(m_faAreaEffect);
    context.Transfer(m_faDamagePerHit);
    context.Transfer(m_faSubBeamDamagePerHit);
    context.Transfer(m_faActivationTime);
    context.Transfer(m_faDeActivationTime);
    context.Transfer(m_naMinMainBeams);
    context.Transfer(m_naMaxMainBeams);
    context.Transfer(m_naMinSubBeams);
    context.Transfer(m_naMaxSubBeams);
    context.Transfer(m_faTemporaryBeamLife);
    context.Transfer(m_faTargettingFOV);
    context.Transfer(m_faStaminaPerSecond);
    context.Transfer(m_faMainBeamDecapitationTime);
    context.Transfer(m_faSubBeamDecapitationTime);
    context.Transfer(m_faSubBeamDelay);
    context.Transfer(OnhitEveryNSecs);
    context.Transfer(CombatMultiplierIncreaseTime);
    context.Transfer(LightningThemeTransitionInSecs);
    context.Transfer(LightningThemeTransitionOutSecs);
}
int main()
{
    static CSpecialAbilitiesLightningSpellDef def;
    CPersistContext ctx; ctx.count = 0;
    def.Transfer(ctx);
    const void* expect[27] = { &def.InitSound, &def.LoopingSound, &def.EndHeroSound, &def.EndAttackeeSound,
        &def.m_fRandomBeamMinAngleX, &def.m_fRandomBeamMaxAngleX, &def.m_fRandomBeamMinAngleZ, &def.m_fRandomBeamMaxAngleZ,
        &def.m_faAreaEffect, &def.m_faDamagePerHit, &def.m_faSubBeamDamagePerHit, &def.m_faActivationTime, &def.m_faDeActivationTime,
        &def.m_naMinMainBeams, &def.m_naMaxMainBeams, &def.m_naMinSubBeams, &def.m_naMaxSubBeams,
        &def.m_faTemporaryBeamLife, &def.m_faTargettingFOV, &def.m_faStaminaPerSecond,
        &def.m_faMainBeamDecapitationTime, &def.m_faSubBeamDecapitationTime, &def.m_faSubBeamDelay, &def.OnhitEveryNSecs,
        &def.CombatMultiplierIncreaseTime, &def.LightningThemeTransitionInSecs, &def.LightningThemeTransitionOutSecs };
    const int kinds[27] = { 1,1,1,1, 2,2,2,2, 3,3,3,3,3, 4,4,4,4, 3,3,3, 3,3,3,3, 2,2,2 };
    bool ok = ctx.count == 27;
    for (int i = 0; ok && i < 27; ++i)
        ok = ctx.order[i] == expect[i] && ctx.kind[i] == kinds[i];
    if (!ok) { std::printf("0046a568_TEST FAIL\n"); return 1; }
    std::printf("0046a568_TEST PASS\n");
    return 0;
}