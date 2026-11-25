// CTCCarryable::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCCarryable::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCCarryable@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D3D46, 16 bytes: mov ecx,[esp+4]; call CTCCarryable::StatGetClassTypeName 0x004D3CFB; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCCarryable@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123B35C (RTTI .?AVCTCCarryable@@) slot 22 (0x0123B3B4 -> 0x004D3D46) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D3CFB, the landed CTCCarryable::StatGetClassTypeName (?StatGetClassTypeName@CTCCarryable@@SI?AVCCharString@@XZ; literal "CTCCarryable").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCCarryable_GetClassTypeName@4 calling the made-up ?Init_004d3cfb@T@@QAEXXZ.
// Header: engine/CTCCarryable.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define CTCCarryable CTCCarryableLayout
#include "engine/CTCCarryable.h"
#undef CTCCarryable

class CTCCarryable : public CTCCarryableLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D3CFB
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCCarryable::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
