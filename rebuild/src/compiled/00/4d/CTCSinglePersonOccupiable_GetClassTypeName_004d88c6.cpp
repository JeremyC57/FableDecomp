// CTCSinglePersonOccupiable::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCSinglePersonOccupiable::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCSinglePersonOccupiable@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D88C6, 16 bytes: mov ecx,[esp+4]; call CTCSinglePersonOccupiable::StatGetClassTypeName 0x004D4E53; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCSinglePersonOccupiable@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123ED54 (RTTI .?AVCTCSinglePersonOccupiable@@) slot 22 (0x0123EDAC -> 0x004D88C6) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D4E53, the landed CTCSinglePersonOccupiable::StatGetClassTypeName (?StatGetClassTypeName@CTCSinglePersonOccupiable@@SI?AVCCharString@@XZ; literal "CTCSinglePersonOccupiable").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCSinglePersonOccupiable_GetClassTypeName@4 calling the made-up ?Init_004d4e53@T@@QAEXXZ.
// Header: engine/CTCSinglePersonOccupiable.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define CTCSinglePersonOccupiable CTCSinglePersonOccupiableLayout
#include "engine/CTCSinglePersonOccupiable.h"
#undef CTCSinglePersonOccupiable

class CTCSinglePersonOccupiable : public CTCSinglePersonOccupiableLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D4E53
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCSinglePersonOccupiable::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
