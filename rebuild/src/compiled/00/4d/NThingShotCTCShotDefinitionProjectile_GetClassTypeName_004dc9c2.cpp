// NThingShot::CTCShotDefinitionProjectile::GetClassTypeName() const -- the class-type name as a CCharString: returns NThingShot::CTCShotDefinitionProjectile::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCShotDefinitionProjectile@NThingShot@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DC9C2, 16 bytes: mov ecx,[esp+4]; call NThingShot::CTCShotDefinitionProjectile::StatGetClassTypeName 0x004D53A8; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCShotDefinitionProjectile@NThingShot@@UBE?AVCCharString@@XZ; retail
//   vftable 0x01240CCC (RTTI .?AVCTCShotDefinitionProjectile@NThingShot@@) slot 22 (0x01240D24 -> 0x004DC9C2) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D53A8, the landed NThingShot::CTCShotDefinitionProjectile::StatGetClassTypeName (?StatGetClassTypeName@CTCShotDefinitionProjectile@NThingShot@@SI?AVCCharString@@XZ; literal "CTCShotDefinitionProjectile").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCShotDefinitionProjectile_GetClassTypeName@4 calling the made-up ?Init_004d53a8@T@@QAEXXZ.
// Header: engine/NThingShot_CTCShotDefinitionProjectilePredictive.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_MOVEMENT
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#define CTCShotDefinitionProjectile CTCShotDefinitionProjectileLayout
#include "engine/NThingShot_CTCShotDefinitionProjectilePredictive.h"
#undef CTCShotDefinitionProjectile

namespace NThingShot {
class CTCShotDefinitionProjectile : public CTCShotDefinitionProjectileLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D53A8
    virtual CCharString GetClassTypeName() const;
};
}

#pragma optimize("s", on)
CCharString NThingShot::CTCShotDefinitionProjectile::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
