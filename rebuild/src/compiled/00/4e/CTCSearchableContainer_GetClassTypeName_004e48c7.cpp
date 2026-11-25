// CTCSearchableContainer::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCSearchableContainer::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCSearchableContainer@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E48C7, 16 bytes: mov ecx,[esp+4]; call CTCSearchableContainer::StatGetClassTypeName 0x004D38CD; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCSearchableContainer@@UBE?AVCCharString@@XZ; retail
//   vftable 0x012431F4 (RTTI .?AVCTCSearchableContainer@@) slot 22 (0x0124324C -> 0x004E48C7) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D38CD, the landed CTCSearchableContainer::StatGetClassTypeName (?StatGetClassTypeName@CTCSearchableContainer@@SI?AVCCharString@@XZ; literal "CTCSearchableContainer").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCSearchableContainer_GetClassTypeName@4 calling the made-up ?Init_004d38cd@T@@QAEXXZ.
// Header: no engine header defines CTCSearchableContainer; a local class over its RTTI direct base engine/CTCContainerRewardHero.h (rtti_bases.tsv: .?AVCTCSearchableContainer@@ : CTCContainerRewardHero).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#include "engine/CTCContainerRewardHero.h"

class CTCSearchableContainer : public CTCContainerRewardHero
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D38CD
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCSearchableContainer::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
