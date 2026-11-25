// CTCActivationReceptorChest::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCActivationReceptorChest::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCActivationReceptorChest@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E07FB, 16 bytes: mov ecx,[esp+4]; call CTCActivationReceptorChest::StatGetClassTypeName 0x004D4326; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCActivationReceptorChest@@UBE?AVCCharString@@XZ; retail
//   vftable 0x01242084 (RTTI .?AVCTCActivationReceptorChest@@) slot 22 (0x012420DC -> 0x004E07FB) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D4326, the landed CTCActivationReceptorChest::StatGetClassTypeName (?StatGetClassTypeName@CTCActivationReceptorChest@@SI?AVCCharString@@XZ; literal "CTCActivationReceptorChest").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCActivationReceptorChest_GetClassTypeName@4 calling the made-up ?Init_004d4326@T@@QAEXXZ.
// Header: no engine header defines CTCActivationReceptorChest; a local class over engine/CTCBase.h, its nearest RTTI ancestor whose header reaches the
//   polymorphic CTCBase (rtti_bases.tsv: .?AVCTCActivationReceptorChest@@ : CTCActivationReceptorBase : CTCBase; engine/CTCActivationReceptorBase.h is a
//   generated flat layout that does not derive from CTCBase, in either branch).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#include "engine/CTCBase.h"

class CTCActivationReceptorChest : public CTCBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D4326
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCActivationReceptorChest::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
