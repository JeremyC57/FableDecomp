#ifndef FABLE_ENGINE_CAPPEARANCEDEF_H
#define FABLE_ENGINE_CAPPEARANCEDEF_H
// CAppearanceDef (new shared header, chunk 1006, row 0x005B3E4E CTCInventoryClothing::GlobalUpdate; the generated flat model is in
// engine/_quarantine/). Retail class: RTTI .?AVCAppearanceDef@@ : CSubDefClassBase (ghidra_out/rtti_bases.tsv). Ego_r.pdb members
// (packed after the 0x25-byte base): Graphic +0x25 (CEngineGraphic, opaque here), Animation +0x34 (CAnimationSet: the row adds 34h
// to the def pointer before GetAnim, 0x005B3EC0); the members after Animation are not modelled. Reached through
// CDefClassBase::GetSubDef<CAppearanceDef> (out of line in retail at 0x0042B0A2). Needs FABLE_RETAIL_DEF_CLASS_BASE.
#include "rebuild_abi.h"
#include "fable_string.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CAnimationSet.h"
#pragma pack(push, 1)
class CAppearanceDef : public CSubDefClassBase {
public:
    // Ego_r.pdb / FableWin.pdb ?StatGetTypeName@CAppearanceDef@@SI?AVCCharString@@XZ (inline: the class-name key GetSubDef looks up).
    static CCharString __fastcall StatGetTypeName() { return CCharString("CAppearanceDef", -1); }
    unsigned char Graphic[0xf];                              // +0x25 CEngineGraphic
    CAnimationSet Animation;                                 // +0x34
};
#pragma pack(pop)
#endif // FABLE_ENGINE_CAPPEARANCEDEF_H
