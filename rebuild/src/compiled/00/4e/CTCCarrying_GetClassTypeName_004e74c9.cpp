// CTCCarrying::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCCarrying::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCCarrying@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E74C9, 16 bytes: mov ecx,[esp+4]; call CTCCarrying::StatGetClassTypeName 0x004D3D56; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCCarrying@@UBE?AVCCharString@@XZ; retail
//   vftable 0x012433F4 (RTTI .?AVCTCCarrying@@) slot 22 (0x0124344C -> 0x004E74C9) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D3D56, the landed CTCCarrying::StatGetClassTypeName (?StatGetClassTypeName@CTCCarrying@@SI?AVCCharString@@XZ; literal "CTCCarrying").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCCarrying_GetClassTypeName@4 calling the made-up ?Init_004d3d56@T@@QAEXXZ.
// Header: engine/CTCCarrying.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define CTCCarrying CTCCarryingLayout
#include "engine/CTCCarrying.h"
#undef CTCCarrying

class CTCCarrying : public CTCCarryingLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D3D56
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCCarrying::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
