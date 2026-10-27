// operator!=(const CBaseObject* object, const CIntelligentPointer<CThingCreatureBase>& pointer) -- true when the object is not the pointer's current item
// Leaf: ??$?9VCThingCreatureBase@@@@YI_NPBVCBaseObject@@ABV?$CIntelligentPointer@VCThingCreatureBase@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00776410, 21 bytes, `__fastcall` (object ecx, pointer edx), plain `ret`.
// Identity (probable): FableWin.pdb ??$?9VCThingCreatureBase@@@@YI_NPBVCBaseObject@@ABV?$CIntelligentPointer@VCThingCreatureBase@@@@@Z; the second of retail's three bodies of this shape (no callers); FableWin.pdb's second != instantiation by address is <CThingCreatureBase> (0x02628460).
//   The manifest's "SetLungedThisFrame" label is propagated.
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50.
// Shape (family M154, representative 0x005CBDD2): `return object != pointer.GetPItem();` -- the object kept in esi across the
//   call, `setne` through ecx. Size TU.
#pragma optimize("s", on)
class CBaseObject;
class CThingCreatureBase;
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
template bool __fastcall operator!=(const CBaseObject* object, const CIntelligentPointer<CThingCreatureBase>& pointer);