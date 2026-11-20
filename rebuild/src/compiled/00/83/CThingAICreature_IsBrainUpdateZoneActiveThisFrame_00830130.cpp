// CThingAICreature::IsBrainUpdateZoneActiveThisFrame() const -- whether the creature's brain is in an update zone active this frame (asks PBrain)
// Leaf: ?IsBrainUpdateZoneActiveThisFrame@CThingAICreature@@QBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00830130, 11 bytes: `mov ecx,[ecx+1A8h] ; jmp 0x0088B810` (PBrain's object, then
//   CAIBrain::IsUpdateZoneActiveThisFrame).
// NF123 (2026-11-20, q4714): re-spelled from the stand-in ?Run@T@@QAEXXZ -> ?Run_0088b810@Sub@@QAEXXZ (local T / Sub models) to Ego_r.pdb
//   0x00536F40 + FableWin.pdb 0x01D731C0 ?IsBrainUpdateZoneActiveThisFrame@CThingAICreature@@QBE_NXZ on the shared engine/CThingAICreature.h
//   (WIFE_QUERIES opt-in, DEBUGRENDER for the PBrain member; the landed caller 0x006D1E30 binds it) and engine/CAIBrain.h (NF123 ISUPDATEZONEACTIVE opt-in: Ego_r 0x0082F5E0 +
//   FableWin 0x02804440 ?IsUpdateZoneActiveThisFrame@CAIBrain@@QBE_NXZ for the jump target 0x0088B810).
// Include set: the landed sibling forwarder 0x00830350 CThingAICreature::SetPositionUsingBrain (PBrain is declared by DEBUGRENDER).
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_VSLOTS2
#define FABLE_RETAIL_CTHINGAICREATURE_WIFE_QUERIES
#define FABLE_RETAIL_CAIBRAIN_ISUPDATEZONEACTIVE
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

bool CThingAICreature::IsBrainUpdateZoneActiveThisFrame() const
{
    return PBrain->IsUpdateZoneActiveThisFrame();
}
