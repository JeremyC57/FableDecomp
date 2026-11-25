// CTCCreatureHive::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCCreatureHive::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCCreatureHive@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D45E7, 16 bytes: mov ecx,[esp+4]; call CTCCreatureHive::StatGetClassTypeName 0x004D45B4; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCCreatureHive@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123BDCC (RTTI .?AVCTCCreatureHive@@) slot 22 (0x0123BE24 -> 0x004D45E7) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D45B4, the landed CTCCreatureHive::StatGetClassTypeName (?StatGetClassTypeName@CTCCreatureHive@@SI?AVCCharString@@XZ; literal "CTCCreatureHive").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCCreatureHive_GetClassTypeName@4 calling the made-up ?Init_004d45b4@T@@QAEXXZ.
// Header: engine/CTCCreatureHive.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define CTCCreatureHive CTCCreatureHiveLayout
#include "engine/CTCCreatureHive.h"
#undef CTCCreatureHive

class CTCCreatureHive : public CTCCreatureHiveLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D45B4
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCCreatureHive::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
