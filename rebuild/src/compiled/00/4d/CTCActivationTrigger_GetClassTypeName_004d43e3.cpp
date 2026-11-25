// CTCActivationTrigger::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCActivationTrigger::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCActivationTrigger@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D43E3, 16 bytes: mov ecx,[esp+4]; call CTCActivationTrigger::StatGetClassTypeName 0x004D43A6; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCActivationTrigger@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123BBAC (RTTI .?AVCTCActivationTrigger@@) slot 22 (0x0123BC04 -> 0x004D43E3) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D43A6, the landed CTCActivationTrigger::StatGetClassTypeName (?StatGetClassTypeName@CTCActivationTrigger@@SI?AVCCharString@@XZ; literal "CTCActivationTrigger").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCActivationTrigger_GetClassTypeName@4 calling the made-up ?Init_004d43a6@T@@QAEXXZ.
// Header: no engine header defines CTCActivationTrigger; a local class over its RTTI direct base engine/CTCBase.h (rtti_bases.tsv: .?AVCTCActivationTrigger@@ : CTCBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#include "engine/CTCBase.h"

class CTCActivationTrigger : public CTCBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D43A6
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCActivationTrigger::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
