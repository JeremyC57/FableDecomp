// NThingShot::CTCShotDefinitionDragonFireball::GetClassTypeName() const -- the class-type name as a CCharString: returns NThingShot::CTCShotDefinitionDragonFireball::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCShotDefinitionDragonFireball@NThingShot@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D558D, 16 bytes: mov ecx,[esp+4]; call NThingShot::CTCShotDefinitionDragonFireball::StatGetClassTypeName 0x004D5562; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCShotDefinitionDragonFireball@NThingShot@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123D0CC (RTTI .?AVCTCShotDefinitionDragonFireball@NThingShot@@) slot 22 (0x0123D124 -> 0x004D558D) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D5562, the landed NThingShot::CTCShotDefinitionDragonFireball::StatGetClassTypeName (?StatGetClassTypeName@CTCShotDefinitionDragonFireball@NThingShot@@SI?AVCCharString@@XZ; literal "CTCShotDefinitionDragonFireball").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCShotDefinitionDragonFireball_GetClassTypeName@4 calling the made-up ?Init_004d5562@T@@QAEXXZ.
// Header: no engine header defines NThingShot::CTCShotDefinitionDragonFireball; a local class over its RTTI direct base engine/NThingShot_CTCShotDefinitionBase.h (rtti_bases.tsv: .?AVCTCShotDefinitionDragonFireball@NThingShot@@ : NThingShot::CTCShotDefinitionBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#include "engine/NThingShot_CTCShotDefinitionBase.h"

namespace NThingShot {
class CTCShotDefinitionDragonFireball : public CTCShotDefinitionBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D5562
    virtual CCharString GetClassTypeName() const;
};
}

#pragma optimize("s", on)
CCharString NThingShot::CTCShotDefinitionDragonFireball::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
