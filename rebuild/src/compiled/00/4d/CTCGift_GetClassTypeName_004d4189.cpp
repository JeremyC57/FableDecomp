// CTCGift::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCGift::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCGift@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D4189, 16 bytes: mov ecx,[esp+4]; call CTCGift::StatGetClassTypeName 0x004D415A; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCGift@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123B83C (RTTI .?AVCTCGift@@) slot 22 (0x0123B894 -> 0x004D4189) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D415A, the landed CTCGift::StatGetClassTypeName (?StatGetClassTypeName@CTCGift@@SI?AVCCharString@@XZ; literal "CTCGift").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCGift_GetClassTypeName@4 calling the made-up ?Init_004d415a@T@@QAEXXZ.
// Header: engine/CTCGift.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_GIVE_ITEM_ACTION_PASS26
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#define CTCGift CTCGiftLayout
#include "engine/CTCGift.h"
#undef CTCGift

class CTCGift : public CTCGiftLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D415A
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCGift::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
