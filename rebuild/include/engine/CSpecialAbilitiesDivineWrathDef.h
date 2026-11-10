#ifndef FABLE_ENGINE_CSPECIALABILITIESDIVINEWRATHDEF_H
#define FABLE_ENGINE_CSPECIALABILITIESDIVINEWRATHDEF_H
// NEW shared header (chunk 1008b, row 0x005EE49A CTCInventoryAbilities::IsAbleToUseAbility): the retail CSpecialAbilitiesDivineWrathDef.
// Ego_r.pdb gives it the same 22 members, in the same order and spelling and at the same offsets, as CSpecialAbilitiesUnholyPowerDef
// (ghidra_out/struct_layouts_egor.tsv, size 312 for both), so it is laid out as engine/CSpecialAbilitiesUnholyPowerDef.h (retail 0x100,
// 12-byte STLport vectors). The row reads EnvChangeSecs +0xB8 and StaminaCostPerSec +0xDC (0x005EE607..0x005EE625) under the def name
// "SPECIAL_ABILITIES_DIVINE_WRATH_DEF", exactly as it reads them from the unholy-power def.
// Needs FABLE_RETAIL_CDEFSTRINGTABLE and FABLE_RETAIL_CCHARSTRING (engine/CDefString.h).
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#endif
#include "engine/CParentDefClassBase.h"
#include "engine/CDefString.h"
#include "stlport/fable_containers.h"
class CSpecialAbilitiesDivineWrathDef : public CParentDefClassBase {
public:
    std::vector<CDefString,std::allocator<CDefString> > StartPowerupSound;       // +0x3C
    std::vector<CDefString,std::allocator<CDefString> > LoopingPowerupSound;     // +0x48
    std::vector<CDefString,std::allocator<CDefString> > ReleaseSound;            // +0x54
    std::vector<CDefString,std::allocator<CDefString> > ReleaseLoopSound;        // +0x60
    std::vector<CDefString,std::allocator<CDefString> > FinishSound;             // +0x6C
    std::vector<CDefString,std::allocator<CDefString> > CancelSound;             // +0x78
    float RandomFactorForNumSecsBetweenDamageHit;                                  // +0x84
    std::vector<float,std::allocator<float> > NumSecsBetweenDamageHit;           // +0x88
    std::vector<float,std::allocator<float> > DamagePerHit;                      // +0x94
    std::vector<float,std::allocator<float> > DurationOfSpellSecs;               // +0xA0
    std::vector<float,std::allocator<float> > RadiusOfEffect;                    // +0xAC
    std::vector<float,std::allocator<float> > EnvChangeSecs;                     // +0xB8
    std::vector<float,std::allocator<float> > EnvReturnSecs;                     // +0xC4
    bool DoLifting;                                                              // +0xD0
    float VelocityOfLift;                                                        // +0xD4
    float DistanceToLift;                                                        // +0xD8
    std::vector<float,std::allocator<float> > StaminaCostPerSec;                 // +0xDC
    std::vector<float,std::allocator<float> > DamageTakenMultiplier;             // +0xE8
    float SmallRingRadius;                                                       // +0xF4
    float LargeRingRadius;                                                       // +0xF8
};
#endif
