// CTCWallMount::GetObjectAttached() -- the trophy hung on this wall mount
// Leaf: ?GetObjectAttached@CTCWallMount@@QAEPAVCThing@@XZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x007DE4E0, 8 bytes: `add ecx,10h ; jmp 0x00A01B50` (Trophy.GetPItem(), tail call).
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?GetObjectAttached@CTCWallMount@@QAEPAVCThing@@XZ, first of the Ego_r donor pair
//   GetObjectAttached 0x007DA9A0 / PeekObjectAttached 0x007DA9B0 as retail 0x007DE4E0 precedes 0x007DE4F0; both read the
//   CIntelligentPointer<CThing> Trophy (Ego_r struct_layouts +0x10); its callers use the trophy mutably (CTCActionUseWallMount::OnActionUse
//   0x007EFEDC, CCreatureAction_UseWallMount::PerformAction 0x0084819C). The catalog's "_Cons_val<..CParticleMorphs..>" was propagated.
// Callees: ?GetPItem@CBaseIntelligentPointer@@QBEPAVCBaseObject@@XZ 0x00A01B50.
// Header: engine/CTCWallMount.h (new).
// Twin: 0x007DE4F0 PeekObjectAttached (same bytes, const).
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CTCWallMount.h"
CThing* CTCWallMount::GetObjectAttached()
{
    return Trophy.Get();
}