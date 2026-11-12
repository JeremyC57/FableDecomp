// CThingPlayerCreature::GetCurrentGraphic() const -- returns the definition's Graphic
// Leaf: ?GetCurrentGraphic@CThingPlayerCreature@@MBEPBVCEngineGraphic@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006A99C0, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 90 of CThingPlayerCreature's vtable 0x012457FC (the slot holds this body in 1 vtable(s), owner CThingPlayerCreature); the Xbox 360 Test PDB virtual order (third source) aligned on 33 TLC-named slots of that vtable puts GetCurrentGraphic(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetCurrentGraphic@CThingPlayerCreature@@MBEPBVCEngineGraphic@@XZ; `ret` matches the signature.
// Was: CThingPlayerCreature::vfunc_90 (rebuild/src/compiled/00/6a/CThingPlayerCreature_vfunc90_006a99c0.cpp).
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE_VSLOTS2
#define FABLE_RETAIL_CTHINGCREATUREDEF_MEMBERS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CThing.h"
#include "engine/CDefPointer.h"
#include "stlport/fable_containers.h"
#include "engine/EMovementType.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingCreatureDef.h"
#include "engine/CThingPlayerCreature.h"
const CEngineGraphic* CThingPlayerCreature::GetCurrentGraphic() const
{
    return &PDef->Graphic;
}
