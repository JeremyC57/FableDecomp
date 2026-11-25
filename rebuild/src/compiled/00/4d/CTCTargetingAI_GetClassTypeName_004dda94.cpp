// CTCTargetingAI::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCTargetingAI::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCTargetingAI@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DDA94, 16 bytes: mov ecx,[esp+4]; call CTCTargetingAI::StatGetClassTypeName 0x004D2A74; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF406, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCTargetingAI@@UBE?AVCCharString@@XZ; retail
//   vftable 0x012411FC (RTTI .?AVCTCTargetingAI@@) slot 22 (0x01241254 -> 0x004DDA94) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D2A74, the respelt CTCTargetingAI::StatGetClassTypeName (NF402 b1: FableWin tc_register.obj + retail literal "CTCTargetingAI").
// Respell NF406: was the extern "C" __stdcall stand-in _CTCTargetingAI_GetClassTypeName@4 calling the made-up ?Init_004d2a74@T@@QAEXXZ.
// Header: engine/CTCTargetingAI.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form; prelude as 0x004D2A74 / 0x007F6160).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#define CTCTargetingAI CTCTargetingAILayout
#include "engine/CTCTargetingAI.h"
#undef CTCTargetingAI

class CTCTargetingAI : public CTCTargetingAILayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D2A74
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCTargetingAI::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
