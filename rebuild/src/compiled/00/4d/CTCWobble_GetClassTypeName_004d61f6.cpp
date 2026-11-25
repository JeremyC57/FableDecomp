// CTCWobble::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCWobble::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCWobble@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D61F6, 16 bytes: mov ecx,[esp+4]; call CTCWobble::StatGetClassTypeName 0x004D61B7; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCWobble@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123DF34 (RTTI .?AVCTCWobble@@) slot 22 (0x0123DF8C -> 0x004D61F6) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D61B7, the landed CTCWobble::StatGetClassTypeName (?StatGetClassTypeName@CTCWobble@@SI?AVCCharString@@XZ; literal "CTCWobble").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCWobble_GetClassTypeName@4 calling the made-up ?Init_004d61b7@T@@QAEXXZ.
// Header: engine/CTCWobble.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCWOBBLE_PASS22
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define CTCWobble CTCWobbleLayout
#include "engine/CTCWobble.h"
#undef CTCWobble

class CTCWobble : public CTCWobbleLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D61B7
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCWobble::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
