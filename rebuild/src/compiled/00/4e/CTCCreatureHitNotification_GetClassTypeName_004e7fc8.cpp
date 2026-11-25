// CTCCreatureHitNotification::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCCreatureHitNotification::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCCreatureHitNotification@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E7FC8, 16 bytes: mov ecx,[esp+4]; call CTCCreatureHitNotification::StatGetClassTypeName 0x004D5956; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCCreatureHitNotification@@UBE?AVCCharString@@XZ; retail
//   vftable 0x012436CC (RTTI .?AVCTCCreatureHitNotification@@) slot 22 (0x01243724 -> 0x004E7FC8) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D5956, the landed CTCCreatureHitNotification::StatGetClassTypeName (?StatGetClassTypeName@CTCCreatureHitNotification@@SI?AVCCharString@@XZ; literal "CTCCreatureHitNotification").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCCreatureHitNotification_GetClassTypeName@4 calling the made-up ?Init_004d5956@T@@QAEXXZ.
// Header: no engine header defines CTCCreatureHitNotification; a local class over its RTTI direct base engine/CTCBase.h (rtti_bases.tsv: .?AVCTCCreatureHitNotification@@ : CTCBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#include "engine/CTCBase.h"

class CTCCreatureHitNotification : public CTCBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D5956
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCCreatureHitNotification::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
