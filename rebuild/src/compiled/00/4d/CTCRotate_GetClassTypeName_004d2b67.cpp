// CTCRotate::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCRotate::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCRotate@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D2B67, 16 bytes: mov ecx,[esp+4]; call CTCRotate::StatGetClassTypeName 0x004D2B17; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCRotate@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123A024 (RTTI .?AVCTCRotate@@) slot 22 (0x0123A07C -> 0x004D2B67) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D2B17, the landed CTCRotate::StatGetClassTypeName (?StatGetClassTypeName@CTCRotate@@SI?AVCCharString@@XZ; literal "CTCRotate").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCRotate_GetClassTypeName@4 calling the made-up ?Init_004d2b17@T@@QAEXXZ.
// Header: engine/CTCRotate.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define CTCRotate CTCRotateLayout
#include "engine/CTCRotate.h"
#undef CTCRotate

class CTCRotate : public CTCRotateLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D2B17
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCRotate::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
