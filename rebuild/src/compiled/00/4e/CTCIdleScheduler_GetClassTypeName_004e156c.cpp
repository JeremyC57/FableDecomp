// CTCIdleScheduler::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCIdleScheduler::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCIdleScheduler@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E156C, 16 bytes: mov ecx,[esp+4]; call CTCIdleScheduler::StatGetClassTypeName 0x004D5EA4; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCIdleScheduler@@UBE?AVCCharString@@XZ; retail
//   vftable 0x012426C4 (RTTI .?AVCTCIdleScheduler@@) slot 22 (0x0124271C -> 0x004E156C) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D5EA4, the landed CTCIdleScheduler::StatGetClassTypeName (?StatGetClassTypeName@CTCIdleScheduler@@SI?AVCCharString@@XZ; literal "CTCIdleScheduler").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCIdleScheduler_GetClassTypeName@4 calling the made-up ?Init_004d5ea4@T@@QAEXXZ.
// Header: engine/CTCIdleScheduler.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define CTCIdleScheduler CTCIdleSchedulerLayout
#include "engine/CTCIdleScheduler.h"
#undef CTCIdleScheduler

class CTCIdleScheduler : public CTCIdleSchedulerLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D5EA4
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCIdleScheduler::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
