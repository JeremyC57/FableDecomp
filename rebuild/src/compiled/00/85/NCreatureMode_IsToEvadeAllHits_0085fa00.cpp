// NCreatureMode::IsToEvadeAllHits(const CThing& thing) -- true when a creature's current action evades all hits, a physical thing is flagged to evade all hits, the thing is knocked down or underground, or its mode manager has the intangible or Briar Rose intangible mode active
// Leaf: ?IsToEvadeAllHits@NCreatureMode@@YI_NABVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0085FA00, 154 bytes, static `__fastcall` (ecx = thing), bool in al.
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGPHYSICAL_EVADEALLHITS
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_NCREATUREMODE_QUERIES
#include "fable_string.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/NCreatureMode.h"

namespace NCreatureMode {

bool __fastcall IsToEvadeAllHits(const CThing& thing)
{
    if (thing.GetTypeFlags() & 8) {
        const CCreatureActionBase* action = static_cast<const CThingCreatureBase&>(thing).PeekPCurrentAction();
        if (action && action->IsToEvadeAllHits())
            return true;
    }
    if ((thing.GetTypeFlags() & 2) && static_cast<const CThingPhysical&>(thing).IsToEvadeAllHits())
        return true;
    if (IsKnockedDown(thing))
        return true;
    if (IsUnderground(thing))
        return true;
    if (thing.HasTC(TCI_ENTITY_MODE_MANAGER)) {
        const CTCCreatureModeManager& modes = static_cast<const CTCCreatureModeManager&>(thing.GetTC(TCI_ENTITY_MODE_MANAGER));
        if (modes.IsModeActive(MODE_INTANGIBLE) || modes.IsModeActive(MODE_BRIAR_ROSE_INTANGIBLE))
            return true;
    }
    return false;
}

}