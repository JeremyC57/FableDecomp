// CTCSmashableBarrier::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCSmashableBarrier::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCSmashableBarrier@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DB040, 16 bytes: mov ecx,[esp+4]; call CTCSmashableBarrier::StatGetClassTypeName 0x004D3E86; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCSmashableBarrier@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0124017C (RTTI .?AVCTCSmashableBarrier@@) slot 22 (0x012401D4 -> 0x004DB040) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D3E86, the landed CTCSmashableBarrier::StatGetClassTypeName (?StatGetClassTypeName@CTCSmashableBarrier@@SI?AVCCharString@@XZ; literal "CTCSmashableBarrier").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCSmashableBarrier_GetClassTypeName@4 calling the made-up ?Init_004d3e86@T@@QAEXXZ.
// Header: no engine header defines CTCSmashableBarrier; a local class over its RTTI direct base engine/CTCSmashableBase.h (rtti_bases.tsv: .?AVCTCSmashableBarrier@@ : CTCSmashableBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CTCSmashableBase.h"

class CTCSmashableBarrier : public CTCSmashableBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D3E86
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCSmashableBarrier::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
