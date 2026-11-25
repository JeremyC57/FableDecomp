// CTCBlacksmiths::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCBlacksmiths::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCBlacksmiths@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D40DB, 16 bytes: mov ecx,[esp+4]; call CTCBlacksmiths::StatGetClassTypeName 0x004D40AC; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCBlacksmiths@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123B6C4 (RTTI .?AVCTCBlacksmiths@@) slot 22 (0x0123B71C -> 0x004D40DB) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D40AC, the landed CTCBlacksmiths::StatGetClassTypeName (?StatGetClassTypeName@CTCBlacksmiths@@SI?AVCCharString@@XZ; literal "CTCBlacksmiths").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCBlacksmiths_GetClassTypeName@4 calling the made-up ?Init_004d40ac@T@@QAEXXZ.
// Header: no engine header defines CTCBlacksmiths; a local class over its RTTI direct base engine/CTCBase.h (rtti_bases.tsv: .?AVCTCBlacksmiths@@ : CTCBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#include "engine/CTCBase.h"

class CTCBlacksmiths : public CTCBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D40AC
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCBlacksmiths::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
