// CThingObject::ConstructFromLevelScript(const char* script) -- base CThingGameObject construction; on success re-fetches the object def by its global index
// Leaf: ?ConstructFromLevelScript@CThingObject@@MAE_NPBD@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00703110, length 58 bytes (CThingObject vtable 0x01264DCC slot 4).
// Identity: Ego_r.pdb / FableWin.pdb ?ConstructFromLevelScript@CThingObject@@MAE_NPBD@Z (protected virtual; the catalog's "vfunc_4").
// Callees: CThingGameObject::ConstructFromLevelScript 0x00838930 (slot 4 of the CThingGameObject vtable 0x01244F7C, called by name),
//   CThing::PeekDefinitionManager 0x004C7990, CDefinitionManager::GetDef<CThingObjectDef> 0x0042B040.
// Shape: the argument is pushed straight from its slot, &PDef (+0xD0) is a scratch register (no edi save).
// Name fix (agentNF9 2026-11-11): moved off the local models onto the shared headers; 0x00838930 binds
//   ?ConstructFromLevelScript@CThingGameObject@@UAE_NPBD@Z (was ..UAE_NABUCLevelScriptThingInfo@@@Z), 0x004C7990
//   ?PeekDefinitionManager@CThing@@QBEABVCGameDefinitionManager@@XZ, 0x0042B040 the PDB GetDef<CThingObjectDef>; the row's own
//   parameter is the PDB's `const char*` (was a CLevelScriptThingInfo reference).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CTHINGOBJECTDEF_VALIDATE
#define FABLE_RETAIL_CTHINGOBJECT_DEFINITION
#define FABLE_RETAIL_CTHINGGAMEOBJECT_LEVELSCRIPT
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingObject.h"

bool CThingObject::ConstructFromLevelScript(const char* script)
{
    if (CThingGameObject::ConstructFromLevelScript(script)) {
        PeekDefinitionManager().GetDef(DefGlobalIndex, &PDef);
        return true;
    }
    return false;
}
