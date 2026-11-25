// NThingShot::CTCShotDefinitionTearInGround::GetClassTypeName() const -- the class-type name as a CCharString: returns NThingShot::CTCShotDefinitionTearInGround::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCShotDefinitionTearInGround@NThingShot@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004D5519, 16 bytes: mov ecx,[esp+4]; call NThingShot::CTCShotDefinitionTearInGround::StatGetClassTypeName 0x004D54ED; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCShotDefinitionTearInGround@NThingShot@@UBE?AVCCharString@@XZ; retail
//   vftable 0x0123CF9C (RTTI .?AVCTCShotDefinitionTearInGround@NThingShot@@) slot 22 (0x0123CFF4 -> 0x004D5519) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D54ED, the landed NThingShot::CTCShotDefinitionTearInGround::StatGetClassTypeName (?StatGetClassTypeName@CTCShotDefinitionTearInGround@NThingShot@@SI?AVCCharString@@XZ; literal "CTCShotDefinitionTearInGround").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCShotDefinitionTearInGround_GetClassTypeName@4 calling the made-up ?Init_004d54ed@T@@QAEXXZ.
// Header: no engine header defines NThingShot::CTCShotDefinitionTearInGround; a local class over its RTTI direct base engine/NThingShot_CTCShotDefinitionBase.h (rtti_bases.tsv: .?AVCTCShotDefinitionTearInGround@NThingShot@@ : NThingShot::CTCShotDefinitionBase).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#include "engine/NThingShot_CTCShotDefinitionBase.h"

namespace NThingShot {
class CTCShotDefinitionTearInGround : public CTCShotDefinitionBase
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D54ED
    virtual CCharString GetClassTypeName() const;
};
}

#pragma optimize("s", on)
CCharString NThingShot::CTCShotDefinitionTearInGround::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
