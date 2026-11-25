// CTCDHeroSuit::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCDHeroSuit::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCDHeroSuit@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E07C6, 16 bytes: mov ecx,[esp+4]; call CTCDHeroSuit::StatGetClassTypeName 0x004D4232; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF406, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCDHeroSuit@@UBE?AVCCharString@@XZ; retail
//   vftable 0x01242004 (RTTI .?AVCTCDHeroSuit@@) slot 22 (0x0124205C -> 0x004E07C6) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D4232, the respelt CTCDHeroSuit::StatGetClassTypeName (NF402 b1: FableWin tc_register.obj + retail literal "CTCDHeroSuit").
// Respell NF406: was the extern "C" __stdcall stand-in _CTCDHeroSuit_GetClassTypeName@4 calling the made-up ?Init_004d4232@T@@QAEXXZ.
// Header: no engine/CTCDHeroSuit.h exists; a local class over the RTTI direct base engine/CTCDriverBase.h (rtti_bases.tsv: .?AVCTCDHeroSuit@@ : CTCDriverBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#include "engine/CTCDriverBase.h"

class CTCDHeroSuit : public CTCDriverBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D4232
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCDHeroSuit::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
