// CTCAnimatingObject::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCAnimatingObject::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCAnimatingObject@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DE86E, 16 bytes: mov ecx,[esp+4]; call CTCAnimatingObject::StatGetClassTypeName 0x004D4A56; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCAnimatingObject@@UBE?AVCCharString@@XZ; retail
//   vftable 0x012417E4 (RTTI .?AVCTCAnimatingObject@@) slot 22 (0x0124183C -> 0x004DE86E) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D4A56, the landed CTCAnimatingObject::StatGetClassTypeName (?StatGetClassTypeName@CTCAnimatingObject@@SI?AVCCharString@@XZ; literal "CTCAnimatingObject").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCAnimatingObject_GetClassTypeName@4 calling the made-up ?Init_004d4a56@T@@QAEXXZ.
// Header: engine/CTCAnimatingObject.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define CTCAnimatingObject CTCAnimatingObjectLayout
#include "engine/CTCAnimatingObject.h"
#undef CTCAnimatingObject

class CTCAnimatingObject : public CTCAnimatingObjectLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D4A56
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCAnimatingObject::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
