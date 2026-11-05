// operator!=(const CBaseObject* object, const CIntelligentPointer<CThing>& pointer) -- true when the object is not the pointer's current item
// Leaf: ??$?9VCThing@@@@YI_NPBVCBaseObject@@ABV?$CIntelligentPointer@VCThing@@@@@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) @ 0x005CBDD2, 21 bytes, `__fastcall` (object ecx, pointer edx), plain `ret`.
// Identity (probable): FableWin.pdb ??$?9VCThing@@@@YI_NPBVCBaseObject@@ABV?$CIntelligentPointer@VCThing@@@@@Z; the first of retail's three bodies of this shape (a scan of .text finds only 0x005CBDD2 / 0x00776410 / 0x00981FD0) and the only one called (CTCInventoryWeapons::SetupActiveWeaponsOnCarrier, CDrawHighlightingTargetingCursor::Draw, CDrawSpiritTargetingCursor::Draw, CCreatureAction_UseWallMount::PerformAction -- unrelated subsystems, the generic CThing pointer); FableWin.pdb's !=<CThing> (0x01DE79C0) is its first and sits next to ==<CThingCreatureBase> (0x01DE77F0), as 0x005CBDD2 sits next to the == body 0x005CBDFA.
//   The manifest's "SetLungedThisFrame" label is propagated.
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50.
// Shape (family M154, representative 0x005CBDD2): `return object != pointer.GetPItem();` -- the object kept in esi across the
//   call, `setne` through ecx. Size TU.
#pragma optimize("s", on)
class CBaseObject;
class CThing;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
template <class T> bool __fastcall operator!=(const CBaseObject* object, const CIntelligentPointer<T>& pointer)
{
    return object != pointer.GetPItem();
}
template bool __fastcall operator!=(const CBaseObject* object, const CIntelligentPointer<CThing>& pointer);