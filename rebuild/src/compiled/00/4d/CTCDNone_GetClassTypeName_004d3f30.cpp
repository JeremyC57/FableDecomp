// CTCDNone::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCDNone::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCDNone@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D3F30, 16 bytes: mov ecx,[esp+4]; call CTCDNone::StatGetClassTypeName 0x004D3EF9; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF417, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCDNone@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123B504 (RTTI .?AVCTCDNone@@) slot 22 (0x0123B55C -> 0x004D3F30) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D3EF9, CTCDNone::StatGetClassTypeName (?StatGetClassTypeName@CTCDNone@@SI?AVCCharString@@XZ; literal "CTCDNone"; respelt by NF417 batch s1, which this row needs).
// Respell NF417 (RULINGS r1 of NF412): was the extern "C" __stdcall stand-in _CTCDNone_GetClassTypeName@4 calling the made-up ?Init_004d3ef9@T@@QAEXXZ.
// Header: no engine header defines CTCDNone; a local class over its RTTI direct base engine/CTCDriverBase.h (rtti_bases.tsv: .?AVCTCDNone@@ : CTCDriverBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#include "engine/CTCDriverBase.h"

class CTCDNone : public CTCDriverBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D3EF9
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCDNone::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
