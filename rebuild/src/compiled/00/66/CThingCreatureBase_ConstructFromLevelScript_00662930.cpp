// CThingCreatureBase::ConstructFromLevelScript(const char* script) -- construct the game object from its level-script text, then fetch the creature definition into PDef
// Leaf: ?ConstructFromLevelScript@CThingCreatureBase@@UAE_NPBD@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00662930, length 58 bytes (+ int3 padding).
// Identity: Ego_r.pdb / FableWin.pdb ?ConstructFromLevelScript@CThingCreatureBase@@UAE_NPBD@Z (public virtual, slot 4; called by name
//   from CThingAICreature::ConstructFromLevelScript 0x008315C0).
// Callees: CThingGameObject::ConstructFromLevelScript 0x00838930 (called by name), CThing::PeekDefinitionManager 0x004C7990,
//   CDefinitionManager::GetDef<CThingCreatureDef> 0x00513160.
// Shape: the success path is the `if` body, the GetDef arguments pushed before the accessor runs, `return false` the cold tail.
// Name fix (agentNF9 2026-11-11): moved off the local CThingPhysical / CThingCreatureBase / CDefinitionManager models onto the shared
//   headers; 0x00838930 binds ?ConstructFromLevelScript@CThingGameObject@@UAE_NPBD@Z (was ?ConstructFromLevelScript@CThingPhysical@@QAE_NPBD@Z),
//   0x004C7990 ?PeekDefinitionManager@CThing@@QBEABVCGameDefinitionManager@@XZ, 0x00513160 the PDB GetDef<CThingCreatureDef>; the row's
//   own symbol is the public virtual (was QAE).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CTHINGCREATUREBASE_LEVELSCRIPT
#define FABLE_RETAIL_CTHINGGAMEOBJECT_LEVELSCRIPT
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CGameDefinitionManager.h"

bool CThingCreatureBase::ConstructFromLevelScript(const char* script)
{
    if (CThingGameObject::ConstructFromLevelScript(script)) {
        PeekDefinitionManager().GetDef(DefGlobalIndex, &PDef);
        return true;
    }
    return false;
}
