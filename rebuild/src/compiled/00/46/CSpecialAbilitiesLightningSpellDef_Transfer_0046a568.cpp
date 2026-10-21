// CSpecialAbilitiesLightningSpellDef::Transfer(CPersistContext&) -- persist the lightning spell def's fields
// Leaf: ?Transfer@CSpecialAbilitiesLightningSpellDef@@UAEXAAVCPersistContext@@@Z
// Retail 0x0046A568, 373 bytes, size-optimised def TU (#pragma optimize("s")): unaligned address, no padding.
// One CPersistContext::Transfer<T> instantiation per member type:
//   0x0045228F Transfer(CDefString&), 0x00431061 Transfer(float&),
//   0x00464CBF Transfer(vector<float>&), 0x00466A47 Transfer(vector<long>&).
// Layout: retail STLport vectors are 12 bytes (the PDB's are 16), so every member sits below the PDB offset:
//   vectors from +0x3C (5 float, 4 long, 3 float), four angle floats +0xCC, four more float vectors +0xDC,
//   CombatMultiplierIncreaseTime +0x10C, four CDefString sounds +0x110, two theme floats +0x120.
// Shape: the sounds are transferred first, then the angles, then the vectors in declaration order.
#pragma optimize("s", on)

struct CDefString { long m_index; };
template <class T> struct CStlVector { T* _M_start; T* _M_finish; T* _M_end_of_storage; };

struct CPersistContext {
    void Transfer(CDefString& v);           // 0x0045228F
    void Transfer(float& v);                // 0x00431061
    void Transfer(CStlVector<float>& v);    // 0x00464CBF
    void Transfer(CStlVector<long>& v);     // 0x00466A47
};

struct CSpecialAbilitiesLightningSpellDef {
    virtual void Transfer(CPersistContext& context);
    unsigned char _base[0x38];                       // CSpecialAbilitiesDef base fields (+0x04)
    CStlVector<float> m_faAreaEffect;                // +0x3C
    CStlVector<float> m_faDamagePerHit;              // +0x48
    CStlVector<float> m_faSubBeamDamagePerHit;       // +0x54
    CStlVector<float> m_faActivationTime;            // +0x60
    CStlVector<float> m_faDeActivationTime;          // +0x6C
    CStlVector<long>  m_naMinMainBeams;              // +0x78
    CStlVector<long>  m_naMaxMainBeams;              // +0x84
    CStlVector<long>  m_naMinSubBeams;               // +0x90
    CStlVector<long>  m_naMaxSubBeams;               // +0x9C
    CStlVector<float> m_faTemporaryBeamLife;         // +0xA8
    CStlVector<float> m_faTargettingFOV;             // +0xB4
    CStlVector<float> m_faStaminaPerSecond;          // +0xC0
    float m_fRandomBeamMinAngleX;                    // +0xCC
    float m_fRandomBeamMaxAngleX;                    // +0xD0
    float m_fRandomBeamMinAngleZ;                    // +0xD4
    float m_fRandomBeamMaxAngleZ;                    // +0xD8
    CStlVector<float> m_faMainBeamDecapitationTime;  // +0xDC
    CStlVector<float> m_faSubBeamDecapitationTime;   // +0xE8
    CStlVector<float> m_faSubBeamDelay;              // +0xF4
    CStlVector<float> OnhitEveryNSecs;               // +0x100
    float CombatMultiplierIncreaseTime;              // +0x10C
    CDefString InitSound;                            // +0x110
    CDefString LoopingSound;                         // +0x114
    CDefString EndHeroSound;                         // +0x118
    CDefString EndAttackeeSound;                     // +0x11C
    float LightningThemeTransitionInSecs;            // +0x120
    float LightningThemeTransitionOutSecs;           // +0x124
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