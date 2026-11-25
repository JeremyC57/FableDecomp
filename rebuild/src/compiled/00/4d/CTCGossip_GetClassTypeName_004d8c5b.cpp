// CTCGossip::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCGossip::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCGossip@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D8C5B, 16 bytes: mov ecx,[esp+4]; call CTCGossip::StatGetClassTypeName 0x004D52D5; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCGossip@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123EE44 (RTTI .?AVCTCGossip@@) slot 22 (0x0123EE9C -> 0x004D8C5B) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D52D5, the landed CTCGossip::StatGetClassTypeName (?StatGetClassTypeName@CTCGossip@@SI?AVCCharString@@XZ; literal "CTCGossip").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCGossip_GetClassTypeName@4 calling the made-up ?Init_004d52d5@T@@QAEXXZ.
// Header: engine/CTCGossip.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define CTCGossip CTCGossipLayout
#include "engine/CTCGossip.h"
#undef CTCGossip

class CTCGossip : public CTCGossipLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D52D5
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCGossip::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
