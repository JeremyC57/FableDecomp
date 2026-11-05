#ifndef FABLE_ENGINE_CSPECIALABILITIESUNHOLYPOWERDEF_H
#define FABLE_ENGINE_CSPECIALABILITIESUNHOLYPOWERDEF_H
// New shared header (chunk 782, row 0x007941D0 CTCUnholyPower ctor): the retail CSpecialAbilitiesUnholyPowerDef, laid out as the
// sibling CSpecialAbilities*Def headers (CParentDefClassBase base, packed 0x39, the first member at +0x3C): the 22 Ego_r.pdb members in
// order and spelling (ghidra_out/struct_layouts_egor.tsv, size 312 there) with 12-byte STLport vectors (retail 0x100). The ctor reads
// StartPowerupSound +0x3C, LoopingPowerupSound +0x48, NumSecsBetweenDamageHit +0x88, DurationOfSpellSecs +0xA0, RadiusOfEffect +0xAC,
// EnvChangeSecs +0xB8, StaminaCostPerSec +0xDC, SmallRingRadius +0xF4 and LargeRingRadius +0xF8 (all matched by the row).
// Needs FABLE_RETAIL_CDEFSTRINGTABLE and FABLE_RETAIL_CCHARSTRING (engine/CDefString.h).
#ifndef FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#endif
#include "engine/CParentDefClassBase.h"
#include "engine/CDefString.h"
#include "stlport/fable_containers.h"
class CSpecialAbilitiesUnholyPowerDef : public CParentDefClassBase {
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
