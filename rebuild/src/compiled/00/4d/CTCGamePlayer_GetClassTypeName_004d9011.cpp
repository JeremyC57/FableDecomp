// CTCGamePlayer::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCGamePlayer::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCGamePlayer@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D9011, 16 bytes: mov ecx,[esp+4]; call CTCGamePlayer::StatGetClassTypeName 0x004D5ADD; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCGamePlayer@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123EEBC (RTTI .?AVCTCGamePlayer@@) slot 22 (0x0123EF14 -> 0x004D9011) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D5ADD, the landed CTCGamePlayer::StatGetClassTypeName (?StatGetClassTypeName@CTCGamePlayer@@SI?AVCCharString@@XZ; literal "CTCGamePlayer").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCGamePlayer_GetClassTypeName@4 calling the made-up ?Init_004d5add@T@@QAEXXZ.
// Header: no engine header defines CTCGamePlayer; a local class over its RTTI direct base engine/CTCBase.h (rtti_bases.tsv: .?AVCTCGamePlayer@@ : CTCBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#include "engine/CTCBase.h"

class CTCGamePlayer : public CTCBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D5ADD
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCGamePlayer::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
