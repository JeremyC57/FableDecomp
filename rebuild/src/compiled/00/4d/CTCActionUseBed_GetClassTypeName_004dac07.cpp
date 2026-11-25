// CTCActionUseBed::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCActionUseBed::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCActionUseBed@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DAC07, 16 bytes: mov ecx,[esp+4]; call CTCActionUseBed::StatGetClassTypeName 0x004D2C7B; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCActionUseBed@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123F784 (RTTI .?AVCTCActionUseBed@@) slot 22 (0x0123F7DC -> 0x004DAC07) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D2C7B, the landed CTCActionUseBed::StatGetClassTypeName (?StatGetClassTypeName@CTCActionUseBed@@SI?AVCCharString@@XZ; literal "CTCActionUseBed").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCActionUseBed_GetClassTypeName@4 calling the made-up ?Init_004d2c7b@T@@QAEXXZ.
// Header: engine/CTCActionUseBed.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_BED_USE_PASS28
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#define CTCActionUseBed CTCActionUseBedLayout
#include "engine/CTCActionUseBed.h"
#undef CTCActionUseBed

class CTCActionUseBed : public CTCActionUseBedLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D2C7B
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCActionUseBed::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
