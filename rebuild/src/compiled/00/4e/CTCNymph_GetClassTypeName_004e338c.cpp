// CTCNymph::GetClassTypeName() const -- the class-type name as a CCharString: returns CTCNymph::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCNymph@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E338C, 16 bytes: mov ecx,[esp+4]; call CTCNymph::StatGetClassTypeName 0x004D617B; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCNymph@@UBE?AVCCharString@@XZ; retail
//   vftable 0x01242AD4 (RTTI .?AVCTCNymph@@) slot 22 (0x01242B2C -> 0x004E338C) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D617B, the landed CTCNymph::StatGetClassTypeName (?StatGetClassTypeName@CTCNymph@@SI?AVCCharString@@XZ; literal "CTCNymph").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCNymph_GetClassTypeName@4 calling the made-up ?Init_004d617b@T@@QAEXXZ.
// Header: engine/CTCNymph.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_NYMPH_ATTACK_PASS27
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#include "engine/CThingCreatureBase.h"
#define CTCNymph CTCNymphLayout
#include "engine/CTCNymph.h"
#undef CTCNymph

class CTCNymph : public CTCNymphLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D617B
    virtual CCharString GetClassTypeName() const;
};

#pragma optimize("s", on)
CCharString CTCNymph::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
