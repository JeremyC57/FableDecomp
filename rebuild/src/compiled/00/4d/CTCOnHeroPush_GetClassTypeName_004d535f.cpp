// CTCOnHeroPush::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCOnHeroPush::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCOnHeroPush@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D535F, 16 bytes: mov ecx,[esp+4]; call CTCOnHeroPush::StatGetClassTypeName 0x004D532E; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCOnHeroPush@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123CBC4 (RTTI .?AVCTCOnHeroPush@@) slot 22 (0x0123CC1C -> 0x004D535F) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D532E, the landed CTCOnHeroPush::StatGetClassTypeName (?StatGetClassTypeName@CTCOnHeroPush@@SI?AVCCharString@@XZ; literal "CTCOnHeroPush").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCOnHeroPush_GetClassTypeName@4 calling the made-up ?Init_004d532e@T@@QAEXXZ.
// Header: engine/CTCOnHeroPush.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define CTCOnHeroPush CTCOnHeroPushLayout
#include "engine/CTCOnHeroPush.h"
#undef CTCOnHeroPush

class CTCOnHeroPush : public CTCOnHeroPushLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D532E
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCOnHeroPush::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
