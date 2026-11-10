// CThingAICreature::ForceAIReset() -- forwards to the creature's brain: PBrain->ForceReset() (a tail jump).
// Leaf: ForceAIReset
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006D5AB0, 11 bytes: `mov ecx,[ecx+1A8h] ; jmp 0x0088BB60`.
// Name: Ego_r.pdb + FableWin.pdb ?ForceAIReset@CThingAICreature@@QAEXXZ (engine/CThingAICreature.h DRAINED; rows/008da7a0/NAMING_EVIDENCE.md);
// callee Ego_r.pdb + FableWin.pdb ?ForceReset@CAIBrain@@QAEXXZ 0x0088BB60 (overlay opt-in FABLE_RETAIL_CAIBRAIN_FORCERESET), reached
// through CCountedPointer<CAIBrain>::operator-> on PBrain (+0x1A8, the DEBUGRENDER member set).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_DRAINED
#define FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING
#define FABLE_RETAIL_CTHINGAICREATURE_SAVE
#define FABLE_RETAIL_CTHINGAICREATURE_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGAICREATURE_DEBUGRENDER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CAIBRAIN_FORCERESET
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CDefPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIBrain.h"

void CThingAICreature::ForceAIReset()
{
    PBrain->ForceReset();
}
