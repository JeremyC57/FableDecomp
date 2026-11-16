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

class CDefString { public: long m_index; };

#include "stlport/fable_containers.h"
#include "engine/CPersistContext.h"
class CSpecialAbilitiesLightningSpellDef { public:
    virtual void Transfer(CPersistContext& context);
    unsigned char _base[0x38];                       // CSpecialAbilitiesDef base fields (+0x04)
    std::vector<float> m_faAreaEffect;                // +0x3C
    std::vector<float> m_faDamagePerHit;              // +0x48
    std::vector<float> m_faSubBeamDamagePerHit;       // +0x54
    std::vector<float> m_faActivationTime;            // +0x60
    std::vector<float> m_faDeActivationTime;          // +0x6C
    std::vector<long>  m_naMinMainBeams;              // +0x78
    std::vector<long>  m_naMaxMainBeams;              // +0x84
    std::vector<long>  m_naMinSubBeams;               // +0x90
    std::vector<long>  m_naMaxSubBeams;               // +0x9C
    std::vector<float> m_faTemporaryBeamLife;         // +0xA8
    std::vector<float> m_faTargettingFOV;             // +0xB4
    std::vector<float> m_faStaminaPerSecond;          // +0xC0
    float m_fRandomBeamMinAngleX;                    // +0xCC
    float m_fRandomBeamMaxAngleX;                    // +0xD0
    float m_fRandomBeamMinAngleZ;                    // +0xD4
    float m_fRandomBeamMaxAngleZ;                    // +0xD8
    std::vector<float> m_faMainBeamDecapitationTime;  // +0xDC
    std::vector<float> m_faSubBeamDecapitationTime;   // +0xE8
    std::vector<float> m_faSubBeamDelay;              // +0xF4
    std::vector<float> OnhitEveryNSecs;               // +0x100
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
    context.TransferVector(m_faAreaEffect);
    context.TransferVector(m_faDamagePerHit);
    context.TransferVector(m_faSubBeamDamagePerHit);
    context.TransferVector(m_faActivationTime);
    context.TransferVector(m_faDeActivationTime);
    context.TransferVector(m_naMinMainBeams);
    context.TransferVector(m_naMaxMainBeams);
    context.TransferVector(m_naMinSubBeams);
    context.TransferVector(m_naMaxSubBeams);
    context.TransferVector(m_faTemporaryBeamLife);
    context.TransferVector(m_faTargettingFOV);
    context.TransferVector(m_faStaminaPerSecond);
    context.TransferVector(m_faMainBeamDecapitationTime);
    context.TransferVector(m_faSubBeamDecapitationTime);
    context.TransferVector(m_faSubBeamDelay);
    context.TransferVector(OnhitEveryNSecs);
    context.Transfer(CombatMultiplierIncreaseTime);
    context.Transfer(LightningThemeTransitionInSecs);
    context.Transfer(LightningThemeTransitionOutSecs);
}