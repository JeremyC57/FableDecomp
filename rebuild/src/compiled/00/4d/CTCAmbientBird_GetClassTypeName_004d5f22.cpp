// CTCAmbientBird::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCAmbientBird::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCAmbientBird@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D5F22, 16 bytes: mov ecx,[esp+4]; call CTCAmbientBird::StatGetClassTypeName 0x004D5EE8; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCAmbientBird@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123DC0C (RTTI .?AVCTCAmbientBird@@) slot 22 (0x0123DC64 -> 0x004D5F22) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D5EE8, the landed CTCAmbientBird::StatGetClassTypeName (?StatGetClassTypeName@CTCAmbientBird@@SI?AVCCharString@@XZ; literal "CTCAmbientBird").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCAmbientBird_GetClassTypeName@4 calling the made-up ?Init_004d5ee8@T@@QAEXXZ.
// Header: no engine header defines CTCAmbientBird; a local class over its RTTI direct base engine/CTCBase.h (rtti_bases.tsv: .?AVCTCAmbientBird@@ : CTCBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#include "engine/CTCBase.h"

class CTCAmbientBird : public CTCBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D5EE8
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCAmbientBird::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
