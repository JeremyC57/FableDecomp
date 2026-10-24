// operator!=(const CIntelligentPointer<const CThing>& pointer, const CBaseObject* object) -- true when the intelligent pointer does not currently point at object
// Leaf: ??$?9$$CBVCThing@@@@YI_NABV?$CIntelligentPointer@$$CBVCThing@@@@PBVCBaseObject@@@Z
//
// Retail 0x00689270, 19 bytes (+ int3 padding to 32), `ret` (__fastcall: the pointer in ecx, the object in edx).
// Callee: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (thiscall on the pointer, which is its own base at +0).
// Caller: CInputProcessZTarget::ProcessInput 0x00688EB0 (landed) -- `PTarget != targeting->GetTargetThing()`, PTarget
//   being CIntelligentPointer<const CThing> at +0x38 (Ego_r.pdb layout of CInputProcessZTarget).
// Shape: `return pointer.GetPItem() != object;` -- the bool is built as `xor ecx,ecx ; cmp ; setne cl ; mov al,cl`.
// Name: Ego_r.pdb carries `??$?9$$CBVCThing@@@@YI_NABV?$CIntelligentPointer@$$CBVCThing@@@@PBVCBaseObject@@@Z`
//   (the YI = __fastcall free operator template over CIntelligentPointer<T> and a const CBaseObject*); the catalog's
//   "SetLungedThisFrame" is a propagated label.
class CBaseObject;
class CThing;

class CBaseIntelligentPointer {
public:
    CBaseObject* GetPItem() const;                          // 0x00A01B50
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
};

template <class T>
bool __fastcall operator!=(const CIntelligentPointer<T>& pointer, const CBaseObject* object)
{
    return pointer.GetPItem() != object;
}

template bool __fastcall operator!=<const CThing>(const CIntelligentPointer<const CThing>& pointer,
                                                  const CBaseObject* object);