// CTCAISummon::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCAISummon::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCAISummon@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DF309, 16 bytes: mov ecx,[esp+4]; call CTCAISummon::StatGetClassTypeName 0x004D61A4; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCAISummon@@UBE?AVCCharString@@XZ; retail
//   vftable 0x01241C8C (RTTI .?AVCTCAISummon@@) slot 22 (0x01241CE4 -> 0x004DF309) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D61A4, the landed CTCAISummon::StatGetClassTypeName (?StatGetClassTypeName@CTCAISummon@@SI?AVCCharString@@XZ; literal "CTCAISummon").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCAISummon_GetClassTypeName@4 calling the made-up ?Init_004d61a4@T@@QAEXXZ.
// Header: no engine header defines CTCAISummon; a local class over its RTTI direct base engine/CTCBase.h (rtti_bases.tsv: .?AVCTCAISummon@@ : CTCBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#include "engine/CTCBase.h"

class CTCAISummon : public CTCBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D61A4
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCAISummon::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
