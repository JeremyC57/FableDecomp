// NThingShot::CTCShotDefinitionSpreadShotProjectile::GetClassTypeName() const -- the class-type name as a CCharString: returns NThingShot::CTCShotDefinitionSpreadShotProjectile::StatGetClassTypeName()
// Leaf: ?GetClassTypeName@CTCShotDefinitionSpreadShotProjectile@NThingShot@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DCA52, 16 bytes: mov ecx,[esp+4]; call NThingShot::CTCShotDefinitionSpreadShotProjectile::StatGetClassTypeName 0x004D53EE; mov eax,[esp+4];
//   ret 4 -- a const virtual __thiscall returning CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one
//   stack argument and goes to the static __fastcall callee in ecx.
// Identity (NF412, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell ?GetClassTypeName@CTCShotDefinitionSpreadShotProjectile@NThingShot@@UBE?AVCCharString@@XZ; retail
//   vftable 0x01240D84 (RTTI .?AVCTCShotDefinitionSpreadShotProjectile@NThingShot@@) slot 22 (0x01240DDC -> 0x004DCA52) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h);
//   the body calls 0x004D53EE, the landed NThingShot::CTCShotDefinitionSpreadShotProjectile::StatGetClassTypeName (?StatGetClassTypeName@CTCShotDefinitionSpreadShotProjectile@NThingShot@@SI?AVCCharString@@XZ; literal "CTCShotDefinitionSpreadShotProjectile").
// Respell NF412: was the extern "C" __stdcall stand-in _CTCShotDefinitionSpreadShotProjectile_GetClassTypeName@4 calling the made-up ?Init_004d53ee@T@@QAEXXZ.
// Header: engine/NThingShot_CTCShotDefinitionSpreadShotProjectile.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form).
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_MOVEMENT
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#define CTCShotDefinitionSpreadShotProjectile CTCShotDefinitionSpreadShotProjectileLayout
#include "engine/NThingShot_CTCShotDefinitionSpreadShotProjectile.h"
#undef CTCShotDefinitionSpreadShotProjectile

namespace NThingShot {
class CTCShotDefinitionSpreadShotProjectile : public CTCShotDefinitionSpreadShotProjectileLayout
{
public:
    static CCharString __fastcall StatGetClassTypeName();   // 0x004D53EE
    virtual CCharString GetClassTypeName() const;
};
}

#pragma optimize("s", on)
CCharString NThingShot::CTCShotDefinitionSpreadShotProjectile::GetClassTypeName() const
{
    return StatGetClassTypeName();
}
