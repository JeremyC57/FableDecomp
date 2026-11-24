#ifndef FABLE_ENGINE_CANIMATINGOBJECTDEF_H
#define FABLE_ENGINE_CANIMATINGOBJECTDEF_H
// CAnimatingObjectDef (new shared header, hand3c, row 0x007F8C20 CTCAnimatingObject::AddQueuedAnimation; the generated flat model is in
// engine/_quarantine/). Retail class: RTTI .?AVCAnimatingObjectDef@@ : CSubDefClassBase (ghidra_out/rtti_bases.tsv). Ego_r.pdb members:
// Animation +0x28 (CAnimationSet; struct_layouts_egor.tsv, and the row adds 28h to the def pointer before AnimExists / GetAnim,
// 0x007F8C20). The bytes between the 0x25-byte base and Animation are unnamed padding in Ego_r. Reached through
// CDefClassBase::GetSubDef<CAnimatingObjectDef> (out of line in retail at 0x007F9330). Needs FABLE_RETAIL_DEF_CLASS_BASE.
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CAnimationSet.h"
#pragma pack(push, 1)
class CAnimatingObjectDef : public CSubDefClassBase {
public:
    // Ego_r.pdb / FableWin.pdb ?StatGetTypeName@CAnimatingObjectDef@@SI?AVCCharString@@XZ (FableWin 0x02676280; inline in retail: the class-name key GetSubDef looks up).
    static CCharString __fastcall StatGetTypeName() { return CCharString("CAnimatingObjectDef", -1); }
    unsigned char _pad_0x25[0x3];                            // +0x25
    CAnimationSet Animation;                                 // +0x28
#ifdef FABLE_RETAIL_CANIMATINGOBJECTDEF_RTTI_DECLS
    // Opt-in (NF324, 2026-11-24): declarations only, no layout change. Each member below is the one the retail RTTI vftable placement
    // (ghidra_out/labels_rtti_port.tsv) names at its address, in the PDB spelling (access, const, types); each overrides a virtual
    // its base chain declares (clang override probe, made-up-name control fails): no new slot.
    // 0x004EA20A: ?GetSizeofClass@CAnimatingObjectDef@@UAEKXZ (FableWin.pdb)
public:
    virtual unsigned long GetSizeofClass();
#endif
};
#pragma pack(pop)
#endif // FABLE_ENGINE_CANIMATINGOBJECTDEF_H
