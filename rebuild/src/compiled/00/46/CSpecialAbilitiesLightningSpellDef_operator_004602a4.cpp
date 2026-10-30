// CSpecialAbilitiesLightningSpellDef::operator=(const CSpecialAbilitiesLightningSpellDef&) -- memberwise copy: CParentDefClassBase part, the per-level beam vectors, the random-angle floats, the four sound names and the theme transition times; returns *this
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004602A4, length 405 bytes.
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised (every scalar copy goes through eax/al).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0, vector<float>::operator= @ 0x0045EF2D, vector<long>::operator= @ 0x00454886.
// Identity: the catalog label "CParentDefClassBase::operator=" is propagated. Slot 19 (Copy) of CSpecialAbilitiesLightningSpellDef's
// vtable 0x0123460C is the 5-byte `jmp 0x004602A4` right before this body; the PDB members (+0x40..) sit at
// retail +0x3c.. (retail CParentDefClassBase is 0x39 bytes and its vectors are 12 bytes, not the PDB 16).
// Shape: the compiler-style memberwise assignment written out -- base part first (out of line), then each
// member in declaration order: containers through their out-of-line operator=, scalars as eax/al moves.
// The engine header CParentDefClassBase.h is the PDB layout (0x3d bytes, 16-byte SubDefInfoMap); the retail
// base is modelled locally as a packed polymorphic base exactly as the landed sibling 0x00460256 does.
#pragma pack(push, 1)
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
#pragma pack(pop)

#define FABLE_RETAIL_VECTOR(Name) \
    struct Name { void* m_first; void* m_last; void* m_end; Name& operator=(const Name& other); }

FABLE_RETAIL_VECTOR(CFloatVector);                  // vector<float>::operator= 0x0045EF2D
FABLE_RETAIL_VECTOR(CLongVector);                   // vector<long>::operator= 0x00454886

class CDefString { public: long m_index; };               // 4-byte string-table handle, copied as one dword

class CSpecialAbilitiesLightningSpellDef : public CParentDefClassBase { public:          // vtable 0x0123460C
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