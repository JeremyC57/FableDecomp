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
#ifdef FABLE_RETAIL_COPINIONPERSONALITYDEF_ATTITUDEMAP
// Opt-in (egod e07, row 0x00681E60 CTCCreatureOpinionOfHero::SetPersonalityOverride(long, bool)): AttitudeEnabled typed as the Ego_r / Ego_d
// std::map<EOpinionAttitudeType,bool> (retail STLport map, 12 bytes: the row reads its header node at +0x44 and walks it, 0x00681F4B) and
// EOpinionAttitudeType from the shared engine/EOpinionAttitudeType.h. The 4 bytes at +0x50 stay unmodelled (ToleranceToBeingHit +0x54 is retail-proven).
#include "stlport/fable_map.h"
#include "engine/EOpinionAttitudeType.h"                               // the shared full enum (Ego_r / Ego_d enumerators)
#endif
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
FABLE_STATIC_ASSERT_AT(25, sizeof(COpinionPersonalityTraits) == 0x24);
class COpinionPersonalityDef : public CParentDefClassBase {
public:
    COpinionPersonalityTraits* PersonalityTraits;                        // +0x3c
    bool          AttitudeEnabledDefaultForNotNone;                      // +0x40
#ifdef FABLE_RETAIL_COPINIONPERSONALITYDEF_ATTITUDEMAP
    std::map<EOpinionAttitudeType, bool> AttitudeEnabled;                // +0x44
    unsigned char Unmodeled_0x50[0x4];                                   // +0x50 (not modelled)
#else
    unsigned char AttitudeEnabled[0x10];                                 // +0x44 map<EOpinionAttitudeType,bool> (opaque)
#endif
    float         ToleranceToBeingHit;                                   // +0x54
#ifdef FABLE_RETAIL_COPINIONPERSONALITYDEF_COPY_DECLS
    // Opt-in (NF337, 2026-11-24): declarations only, no layout change, for COpinionPersonalityDef::Copy 0x0045E65B, which tail-calls the out-of-line
    // operator= (retail `jmp` 0x0045E660). ?Copy@COpinionPersonalityDef@@UAEXPBVCDefClassBase@@@Z is the member the retail RTTI vftable placement
    // (ghidra_out/labels_rtti_port.tsv) names at 0x0045E65B (slot 19; it overrides CDefClassBase::Copy: clang override probe, made-up-name
    // control fails); ??4COpinionPersonalityDef@@QAEABV0@ABV0@@Z (Ego_r.pdb + FableWin.pdb) is the operator= at 0x0045E660.
public:
    virtual void Copy(const CDefClassBase*);
    const COpinionPersonalityDef& operator=(const COpinionPersonalityDef&);
#endif
};
FABLE_STATIC_ASSERT_AT(33, offsetof(COpinionPersonalityDef, PersonalityTraits) == 0x3c);
FABLE_STATIC_ASSERT_AT(34, offsetof(COpinionPersonalityDef, ToleranceToBeingHit) == 0x54);
#endif
