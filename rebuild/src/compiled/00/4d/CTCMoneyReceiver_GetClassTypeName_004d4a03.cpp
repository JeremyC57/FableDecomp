// CTCMoneyReceiver::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCMoneyReceiver::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCMoneyReceiver@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D4A03, 16 bytes: mov ecx,[esp+4]; call CTCMoneyReceiver::StatGetClassTypeName 0x004D49CA; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCMoneyReceiver@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123C1C4 (RTTI .?AVCTCMoneyReceiver@@) slot 22 (0x0123C21C -> 0x004D4A03) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D49CA, the landed CTCMoneyReceiver::StatGetClassTypeName (?StatGetClassTypeName@CTCMoneyReceiver@@SI?AVCCharString@@XZ; literal "CTCMoneyReceiver").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCMoneyReceiver_GetClassTypeName@4 calling the made-up ?Init_004d49ca@T@@QAEXXZ.
// Header: no engine header defines CTCMoneyReceiver; a local class over its RTTI direct base engine/CTCBase.h (rtti_bases.tsv: .?AVCTCMoneyReceiver@@ : CTCBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#include "engine/CTCBase.h"

class CTCMoneyReceiver : public CTCBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D49CA
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCMoneyReceiver::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
