// NThingShot::CTCShotDefinitionQueenHornetShot::OnCollision(ECollideType, C3DVector const&, C3DVector const&, CThing*) -- the queen hornet shot collides like the base shot definition
// Leaf: ?OnCollision@CTCShotDefinitionQueenHornetShot@NThingShot@@UAEXW4ECollideType@@ABVC3DVector@@1PAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BBB36, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 28 of NThingShot::CTCShotDefinitionQueenHornetShot's vtable 0x0126DFEC (the slot holds this body in 1 vtable(s), owner NThingShot::CTCShotDefinitionQueenHornetShot); the Xbox 360 Test PDB virtual order (third source) aligned on 24 TLC-named slots of that vtable puts OnCollision(ECollideType, C3DVector const &, C3DVector const &, CThing *) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnCollision@CTCShotDefinitionQueenHornetShot@NThingShot@@UAEXW4ECollideType@@ABVC3DVector@@1PAVCThing@@@Z; no `ret` to check (NORET).
// Was: NThingShot::CTCShotDefinitionQueenHornetShot::vfunc_28 (rebuild/src/compiled/00/7b/gapscan_sub007bbb36_007bbb36.cpp).
// No shared header for NThingShot::CTCShotDefinitionQueenHornetShot: minimal local definition on its shared base
// NThingShot::CTCShotDefinitionBase (RTTI via CTCShotDefinitionSpreadShotProjectile / CTCShotDefinitionProjectile, which have no
// header and do not override OnCollision: slot 28 of their vtables is the base's 0x007BF510).
#pragma optimize("s", on)
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_ONHITBYMELEE
#include "engine/CThing.h"
#include "engine/NThingShot_CTCShotDefinitionBase.h"
namespace NThingShot {
class CTCShotDefinitionQueenHornetShot : public CTCShotDefinitionBase {
public:
    virtual void OnCollision(ECollideType, const C3DVector&, const C3DVector&, CThing*);   // slot 28
};
void CTCShotDefinitionQueenHornetShot::OnCollision(ECollideType type, const C3DVector& position, const C3DVector& normal, CThing* thing)
{
    CTCShotDefinitionBase::OnCollision(type, position, normal, thing);
}
}
