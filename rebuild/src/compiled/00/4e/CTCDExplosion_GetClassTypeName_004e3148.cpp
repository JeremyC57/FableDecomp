// CTCDExplosion::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCDExplosion::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCDExplosion@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E3148, 16 bytes: mov ecx,[esp+4]; call CTCDExplosion::StatGetClassTypeName 0x004D5CE6; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF406, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCDExplosion@@UBE?AVCCharString@@XZ; retail
//   vftable 0x01242A54 (RTTI .?AVCTCDExplosion@@) slot 22 (0x01242AAC -> 0x004E3148) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D5CE6, the respelt CTCDExplosion::StatGetClassTypeName (NF402 b1: FableWin tc_register.obj + retail literal "CTCDExplosion").
// Respell NF406: was the extern "C" __stdcall stand-in _CTCDExplosion_GetClassTypeName@4 calling the made-up ?Init_004d5ce6@T@@QAEXXZ.
// Header: engine/CTCDExplosion.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form; prelude as 0x004D5CE6).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#include "engine/ETCInterfaceType.h"
#define CTCDExplosion CTCDExplosionLayout
#include "engine/CTCDExplosion.h"
#undef CTCDExplosion

class CTCDExplosion : public CTCDExplosionLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D5CE6
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCDExplosion::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
