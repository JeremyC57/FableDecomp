// CTCActivationReceptorDoor::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCActivationReceptorDoor::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCActivationReceptorDoor@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DE67B, 16 bytes: mov ecx,[esp+4]; call CTCActivationReceptorDoor::StatGetClassTypeName 0x004D4380; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCActivationReceptorDoor@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0124146C (RTTI .?AVCTCActivationReceptorDoor@@) slot 22 (0x012414C4 -> 0x004DE67B) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D4380, the landed CTCActivationReceptorDoor::StatGetClassTypeName (?StatGetClassTypeName@CTCActivationReceptorDoor@@SI?AVCCharString@@XZ; literal "CTCActivationReceptorDoor").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCActivationReceptorDoor_GetClassTypeName@4 calling the made-up ?Init_004d4380@T@@QAEXXZ.
// Header: no engine header defines CTCActivationReceptorDoor; a local class over engine/CTCBase.h, its nearest RTTI ancestor whose header reaches the
//   polymorphic CTCBase (rtti_bases.tsv: .?AVCTCActivationReceptorDoor@@ : CTCActivationReceptorBase : CTCBase; engine/CTCActivationReceptorBase.h is a
//   generated flat layout that does not derive from CTCBase, in either branch).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#include "engine/CTCBase.h"

class CTCActivationReceptorDoor : public CTCBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D4380
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCActivationReceptorDoor::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
