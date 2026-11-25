// CTCActionUseWallMount::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCActionUseWallMount::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCActionUseWallMount@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DB255, 16 bytes: mov ecx,[esp+4]; call CTCActionUseWallMount::StatGetClassTypeName 0x004D5CD3; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCActionUseWallMount@@UBE?AVCCharString@@XZ; retail
//   vftable 0x012405AC (RTTI .?AVCTCActionUseWallMount@@) slot 22 (0x01240604 -> 0x004DB255) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D5CD3, the landed CTCActionUseWallMount::StatGetClassTypeName (?StatGetClassTypeName@CTCActionUseWallMount@@SI?AVCCharString@@XZ; literal "CTCActionUseWallMount").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCActionUseWallMount_GetClassTypeName@4 calling the made-up ?Init_004d5cd3@T@@QAEXXZ.
// Header: no engine header defines CTCActionUseWallMount; a local class over its RTTI direct base engine/CTCActionUseBase.h (rtti_bases.tsv: .?AVCTCActionUseWallMount@@ : CTCActionUseBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CTCActionUseBase.h"

class CTCActionUseWallMount : public CTCActionUseBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D5CD3
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCActionUseWallMount::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
