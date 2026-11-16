#ifndef FABLE_ENGINE_COPINIONPERSONALITYDEF_H
#define FABLE_ENGINE_COPINIONPERSONALITYDEF_H
// COpinionPersonalityDef / COpinionPersonalityTraits (acctC wfA1, row 0x00682810; new shared header). RTTI
// .?AVCOpinionPersonalityDef@@ single base CParentDefClassBase (ghidra_out/rtti_bases.tsv). Ego_r.pdb members (struct_layouts_egor.tsv,
// 92 bytes there): PersonalityTraits (COpinionPersonalityTraits*, an array indexed by EOpinion), AttitudeEnabledDefaultForNotNone,
// AttitudeEnabled (map<EOpinionAttitudeType,bool>), ToleranceToBeingHit. Retail offsets are 4 lower than Ego_r's (CParentDefClassBase is
// 0x39 bytes here): PersonalityTraits +0x3C (retail `mov edx,[edx+3Ch]`, 0x006833EA; landed TransformForcedOpinion 0x00681790 reads
// the same), ToleranceToBeingHit +0x54 (Ego_r +88). COpinionPersonalityTraits: Ego_r.pdb, nine floats, 36 bytes (landed 0x00681790
// model). The map is kept opaque (16 bytes, as Ego_r).
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CParentDefClassBase.h"
class COpinionPersonalityTraits {
public:
    float BaseOffset;                                                    // +0x00
    float HeroStatScaling;                                               // +0x04
    float VillageOpinionScaling;                                         // +0x08
    float TimeScalingRunIn;                                              // +0x0c
    float TimeScalingRunOut;                                             // +0x10
    float PeakScalingIfPos;                                              // +0x14
    float PeakScalingIfNeg;                                              // +0x18
    float PersistScalingIfPos;                                           // +0x1c
    float PersistScalingIfNeg;                                           // +0x20
};
FABLE_STATIC_ASSERT(sizeof(COpinionPersonalityTraits) == 0x24);
class COpinionPersonalityDef : public CParentDefClassBase {
public:
    COpinionPersonalityTraits* PersonalityTraits;                        // +0x3c
    bool          AttitudeEnabledDefaultForNotNone;                      // +0x40
    unsigned char AttitudeEnabled[0x10];                                 // +0x44 map<EOpinionAttitudeType,bool> (opaque)
    float         ToleranceToBeingHit;                                   // +0x54
};
FABLE_STATIC_ASSERT(offsetof(COpinionPersonalityDef, PersonalityTraits) == 0x3c);
FABLE_STATIC_ASSERT(offsetof(COpinionPersonalityDef, ToleranceToBeingHit) == 0x54);
#endif
