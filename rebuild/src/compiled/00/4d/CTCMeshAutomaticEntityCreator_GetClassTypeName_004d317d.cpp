// CTCMeshAutomaticEntityCreator::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCMeshAutomaticEntityCreator::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCMeshAutomaticEntityCreator@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D317D, 16 bytes: mov ecx,[esp+4]; call CTCMeshAutomaticEntityCreator::StatGetClassTypeName 0x004D314E; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCMeshAutomaticEntityCreator@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123A54C (RTTI .?AVCTCMeshAutomaticEntityCreator@@) slot 22 (0x0123A5A4 -> 0x004D317D) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D314E, the landed CTCMeshAutomaticEntityCreator::StatGetClassTypeName (?StatGetClassTypeName@CTCMeshAutomaticEntityCreator@@SI?AVCCharString@@XZ; literal "CTCMeshAutomaticEntityCreator").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCMeshAutomaticEntityCreator_GetClassTypeName@4 calling the made-up ?Init_004d314e@T@@QAEXXZ.
// Header: no engine header defines CTCMeshAutomaticEntityCreator; a local class over its RTTI direct base engine/CTCBase.h (rtti_bases.tsv: .?AVCTCMeshAutomaticEntityCreator@@ : CTCBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#include "engine/CTCBase.h"

class CTCMeshAutomaticEntityCreator : public CTCBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D314E
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCMeshAutomaticEntityCreator::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
