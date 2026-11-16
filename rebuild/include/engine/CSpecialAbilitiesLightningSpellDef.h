#ifndef FABLE_ENGINE_CSPECIALABILITIESLIGHTNINGSPELLDEF_H
#define FABLE_ENGINE_CSPECIALABILITIESLIGHTNINGSPELLDEF_H
// NEW shared header (agent wfE10, row 0x007A1B20 CTCWillLightning::CLightningBeam::FrameUpdate): the retail
// CSpecialAbilitiesLightningSpellDef. Ego_r.pdb members in PDB order and spelling (ghidra_out/struct_layouts_egor.tsv, 364 bytes with
// 16-byte vectors); retail offsets with 12-byte STLport vectors after the 0x3C CParentDefClassBase, like the sibling
// engine/CSpecialAbilities*Def.h headers. Retail evidence: InitSound +0x110 (landed ctor 0x007A07F0), LoopingSound +0x114 (landed
// StartLoopingSound 0x007A0400), EndAttackeeSound +0x11C (row 0x007A1B20). Needs FABLE_RETAIL_CDEFSTRINGTABLE and FABLE_RETAIL_CCHARSTRING.
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#endif
#include "engine/CParentDefClassBase.h"
#include "engine/CDefString.h"
#include "stlport/fable_containers.h"
class CSpecialAbilitiesLightningSpellDef : public CParentDefClassBase {
public:
    std::vector<float,std::allocator<float> > m_faAreaEffect;                    // +0x3C
    std::vector<float,std::allocator<float> > m_faDamagePerHit;                  // +0x48
    std::vector<float,std::allocator<float> > m_faSubBeamDamagePerHit;           // +0x54
    std::vector<float,std::allocator<float> > m_faActivationTime;                // +0x60
    std::vector<float,std::allocator<float> > m_faDeActivationTime;              // +0x6C
    std::vector<long,std::allocator<long> > m_naMinMainBeams;                    // +0x78
    std::vector<long,std::allocator<long> > m_naMaxMainBeams;                    // +0x84
    std::vector<long,std::allocator<long> > m_naMinSubBeams;                     // +0x90
    std::vector<long,std::allocator<long> > m_naMaxSubBeams;                     // +0x9C
    std::vector<float,std::allocator<float> > m_faTemporaryBeamLife;             // +0xA8
    std::vector<float,std::allocator<float> > m_faTargettingFOV;                 // +0xB4
    std::vector<float,std::allocator<float> > m_faStaminaPerSecond;              // +0xC0
    float m_fRandomBeamMinAngleX;                                                // +0xCC
    float m_fRandomBeamMaxAngleX;                                                // +0xD0
    float m_fRandomBeamMinAngleZ;                                                // +0xD4
    float m_fRandomBeamMaxAngleZ;                                                // +0xD8
    std::vector<float,std::allocator<float> > m_faMainBeamDecapitationTime;      // +0xDC
    std::vector<float,std::allocator<float> > m_faSubBeamDecapitationTime;       // +0xE8
    std::vector<float,std::allocator<float> > m_faSubBeamDelay;                  // +0xF4
    std::vector<float,std::allocator<float> > OnhitEveryNSecs;                   // +0x100
    float CombatMultiplierIncreaseTime;                                          // +0x10C
    CDefString InitSound;                                                        // +0x110
    CDefString LoopingSound;                                                     // +0x114
    CDefString EndHeroSound;                                                     // +0x118
    CDefString EndAttackeeSound;                                                 // +0x11C
    float LightningThemeTransitionInSecs;                                        // +0x120
    float LightningThemeTransitionOutSecs;                                       // +0x124
};
#endif
