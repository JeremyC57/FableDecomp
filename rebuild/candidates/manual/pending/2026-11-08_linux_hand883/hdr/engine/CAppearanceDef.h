#ifndef FABLE_ENGINE_CAPPEARANCEDEF_H
#define FABLE_ENGINE_CAPPEARANCEDEF_H
// CAppearanceDef (new shared header, chunk 883, row 0x006096E0 CTCInventoryMap::ConstructLogbookListPC; replaces nothing: the
// flattened engine/_quarantine/CAppearanceDef.h is not on the include path). The sub-def a thing def's appearance lives in, reached
// with CDefClassBase::GetSubDef<CAppearanceDef> (0x0042B0A2). Ego_r.pdb layout (struct_layouts_egor.tsv), same offsets in retail
// (the quarantined map and the landed CThingDeadCreature::SetupDeadAnimation read Animation at +0x34): CDefClassBase (0x25 bytes),
// Graphic +0x25, Animation +0x34. The members after Animation are not modelled (no sizeof assertion). Needs
// FABLE_RETAIL_DEF_CLASS_BASE. Base: RTTI .?AVCAppearanceDef@@ : CSubDefClassBase (as the landed GetSubDef<CAppearanceDef> row
// 0x0042B0A2 models it). StatGetTypeName: Ego_r/FableWin ?StatGetTypeName@CAppearanceDef@@SI?AVCCharString@@XZ, inline (the
// class-name literal GetSubDef<CAppearanceDef> builds for its lookup; body as that landed row).
#include <stddef.h>
#include "rebuild_abi.h"
#include "engine/CSubDefClassBase.h"
#include "fable_string.h"
#include "engine/CEngineGraphic.h"
#include "engine/CAnimationSet.h"
class CAppearanceDef : public CSubDefClassBase {
public:
    static CCharString __fastcall StatGetTypeName() { return CCharString("CAppearanceDef", -1); }   // inline
    CEngineGraphic Graphic;                                              // +0x25
    CAnimationSet  Animation;                                            // +0x34
};
#endif // FABLE_ENGINE_CAPPEARANCEDEF_H
