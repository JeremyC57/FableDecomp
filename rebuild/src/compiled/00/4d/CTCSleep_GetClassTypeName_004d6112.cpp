// CTCSleep::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCSleep::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCSleep@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D6112, 16 bytes: mov ecx,[esp+4]; call CTCSleep::StatGetClassTypeName 0x004D60D2; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCSleep@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123DD94 (RTTI .?AVCTCSleep@@) slot 22 (0x0123DDEC -> 0x004D6112) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D60D2, the landed CTCSleep::StatGetClassTypeName (?StatGetClassTypeName@CTCSleep@@SI?AVCCharString@@XZ; literal "CTCSleep").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCSleep_GetClassTypeName@4 calling the made-up ?Init_004d60d2@T@@QAEXXZ.
// Header: engine/CTCSleep.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCSLEEP
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define CTCSleep CTCSleepLayout
#include "engine/CTCSleep.h"
#undef CTCSleep

class CTCSleep : public CTCSleepLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D60D2
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCSleep::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
