// CThingAICreature::SetPositionUsingBrain() -- the brain places the creature (tail call to CAIBrain::InitCreaturePosition 0x0088BBB0 on PBrain)
// Leaf: ?SetPositionUsingBrain@CThingAICreature@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00830350, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM04): slot 45 of CThingAICreature's vtable 0x0127293C (the slot holds this body in 1 vtable(s), owner CThingAICreature); the Xbox 360 Test PDB virtual order (third source) aligned on 37 TLC-named slots of that vtable puts SetPositionUsingBrain(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?SetPositionUsingBrain@CThingAICreature@@UAEXXZ; no `ret` to check (NORET).
// Was: CThingAICreature::vfunc_45 (rebuild/src/compiled/00/83/gapscan_sub00830350_00830350.cpp).
// Opt-ins as in the landed row CThingAICreature_ForceAIReset_006d5ab0.cpp (the typed PBrain).
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_VSLOTS2
#define FABLE_RETAIL_CAIBRAIN_INITCREATUREPOSITION
#define FABLE_RETAIL_CTHINGAICREATURE_DRAINED
#define FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING
#define FABLE_RETAIL_CTHINGAICREATURE_SAVE
#define FABLE_RETAIL_CTHINGAICREATURE_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGAICREATURE_DEBUGRENDER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_STLP_LIST
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
#include "engine/CThingAICreature.h"
#include "engine/CAIBrain.h"
void CThingAICreature::SetPositionUsingBrain()
{
    PBrain->InitCreaturePosition();
}
