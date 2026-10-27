// operator!=(const CBaseObject* object, const CIntelligentPointer<CThing>& pointer) -- true when the object is not the pointer's current item
// Leaf: ??$?9VCThing@@@@YI_NPBVCBaseObject@@ABV?$CIntelligentPointer@VCThing@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005CBDD2, 21 bytes, `__fastcall` (object ecx, pointer edx), plain `ret`.
// Identity (probable): FableWin.pdb ??$?9VCThing@@@@YI_NPBVCBaseObject@@ABV?$CIntelligentPointer@VCThing@@@@@Z; the first of retail's three bodies of this shape (a scan of .text finds only 0x005CBDD2 / 0x00776410 / 0x00981FD0) and the only one called (CTCInventoryWeapons::SetupActiveWeaponsOnCarrier, CDrawHighlightingTargetingCursor::Draw, CDrawSpiritTargetingCursor::Draw, CCreatureAction_UseWallMount::PerformAction -- unrelated subsystems, the generic CThing pointer); FableWin.pdb's !=<CThing> (0x01DE79C0) is its first and sits next to ==<CThingCreatureBase> (0x01DE77F0), as 0x005CBDD2 sits next to the == body 0x005CBDFA.
//   The manifest's "SetLungedThisFrame" label is propagated.
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50.
// Shape (family M154, representative 0x005CBDD2): `return object != pointer.GetPItem();` -- the object kept in esi across the
//   call, `setne` through ecx. Size TU.
#pragma optimize("s", on)
class CBaseObject;
class CThing;
class CBaseIntelligentPointer {
public:
    CBaseObject* PItem;                                   // +0x00
    void*        PNext;                                   // +0x04
    CBaseObject* GetPItem() const;                        // 0x00A01B50
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
template <class T> bool __fastcall operator!=(const CBaseObject* object, const CIntelligentPointer<T>& pointer)
{
    return object != pointer.GetPItem();
}
template bool __fastcall operator!=(const CBaseObject* object, const CIntelligentPointer<CThing>& pointer);