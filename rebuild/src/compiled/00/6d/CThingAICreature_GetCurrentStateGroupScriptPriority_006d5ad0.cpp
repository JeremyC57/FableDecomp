// CThingAICreature::GetCurrentStateGroupScriptPriority() const -- the script priority of the brain's current state group (tail call to
//   CAIBrain::GetCurrentStateGroupScriptPriority 0x0088B760 on PBrain)
// Leaf: ?GetCurrentStateGroupScriptPriority@CThingAICreature@@QBE?AW4EScriptAIPriority@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006D5AD0: `mov ecx,[ecx+1A8h] ; jmp 0x0088B760`.
// Name (NF118, PRIORITY q5047): Ego_r.pdb 0x0070A8F0 / FableWin.pdb 0x02496040
//   ?GetCurrentStateGroupScriptPriority@CThingAICreature@@QBE?AW4EScriptAIPriority@@XZ; the landed caller StartScriptingEntity 0x0089B5B0
//   binds that spelling here and compares the result with its EScriptAIPriority argument. Callee: Ego_r 0x0082F530 / FableWin 0x02804180
//   ?GetCurrentStateGroupScriptPriority@CAIBrain@@QBE?AW4EScriptAIPriority@@XZ (0x0088B760, landed). Was the stand-in T::Run on a padded
//   local struct.
// Opt-ins as in the landed row CThingAICreature_SetPositionUsingBrain_00830350.cpp (the typed PBrain, +0x1A8).
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_SCRIPTPRIORITY
#define FABLE_RETAIL_CAIBRAIN_GETCURRENTSTATEGROUPSCRIPTPRIORITY
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
EScriptAIPriority CThingAICreature::GetCurrentStateGroupScriptPriority() const
{
    return PBrain->GetCurrentStateGroupScriptPriority();
}
