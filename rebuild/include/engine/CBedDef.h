#ifndef FABLE_ENGINE_CBEDDEF_H
#define FABLE_ENGINE_CBEDDEF_H
// CBedDef (new shared header, chunk 2026, row 0x0092DA20 CAIStateGroup_MinionSleep::Update): the bed def a CTCBed points at.
// RTTI .?AVCBedDef@@ : CSubDefClassBase (ghidra_out/rtti_bases.tsv). Members Ego_r.pdb (struct_layouts_egor.tsv, sizeof 0x3C):
// GetInBedAnimName +0x28, SleepInBedAnimName +0x2C, GetUpFromBedAnimName +0x30, GetUpFromBedFastAnimName +0x34 (CDefString each),
// Animating +0x38. Retail reads them at the same offsets (no container before them): MinionSleep::Update converts +0x28 / +0x2C /
// +0x30 / +0x34 through CDefStringTable::GetString (0x0092DC66, 0x0092DDE9, 0x0092E076, 0x0092DFAF) and compares +0x34 with ""
// (0x0092DF3E); the landed CTCBed::StartGetOutOfBedAnimation 0x00746A70 tests Animating +0x38. Needs FABLE_RETAIL_DEF_CLASS_BASE and
// FABLE_RETAIL_CDEFSTRINGTABLE. (rebuild/include/engine/_quarantine/CBedDef.h is the flat generated layout.)
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CDefString.h"
class CBedDef : public CSubDefClassBase {
public:
    CDefString GetInBedAnimName;                                         // +0x28
    CDefString SleepInBedAnimName;                                       // +0x2c
    CDefString GetUpFromBedAnimName;                                     // +0x30
    CDefString GetUpFromBedFastAnimName;                                 // +0x34
    bool       Animating;                                                // +0x38
#ifdef FABLE_RETAIL_CBEDDEF_RTTI_DECLS
    // Opt-in (NF324, 2026-11-24): declarations only, no layout change. Each member below is the one the retail RTTI vftable placement
    // (ghidra_out/labels_rtti_port.tsv) names at its address, in the PDB spelling (access, const, types); each overrides a virtual
    // its base chain declares (clang override probe, made-up-name control fails): no new slot.
    // 0x004D7A46: ?GetSizeofClass@CBedDef@@UAEKXZ (FableWin.pdb)
public:
    virtual unsigned long GetSizeofClass();
#endif
};
FABLE_STATIC_ASSERT_AT(22, sizeof(CBedDef) == 0x3c);
#endif // FABLE_ENGINE_CBEDDEF_H
