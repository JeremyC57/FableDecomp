// CTCVillageMember::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCVillageMember::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCVillageMember@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DAF1B, 16 bytes: mov ecx,[esp+4]; call CTCVillageMember::StatGetClassTypeName 0x004D31D8; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCVillageMember@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0124007C (RTTI .?AVCTCVillageMember@@) slot 22 (0x012400D4 -> 0x004DAF1B) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D31D8, the landed CTCVillageMember::StatGetClassTypeName (?StatGetClassTypeName@CTCVillageMember@@SI?AVCCharString@@XZ; literal "CTCVillageMember").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCVillageMember_GetClassTypeName@4 calling the made-up ?Init_004d31d8@T@@QAEXXZ.
// Header: engine/CTCVillageMember.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define CTCVillageMember CTCVillageMemberLayout
#include "engine/CTCVillageMember.h"
#undef CTCVillageMember

class CTCVillageMember : public CTCVillageMemberLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D31D8
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCVillageMember::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
