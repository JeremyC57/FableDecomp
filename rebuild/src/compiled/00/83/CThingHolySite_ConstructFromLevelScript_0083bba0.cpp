// CThingHolySite::ConstructFromLevelScript(const char* script) -- base CThingGameObject construction; on success re-fetches the def by index and reads PlayerStart from it
// Leaf: ?ConstructFromLevelScript@CThingHolySite@@UAE_NPBD@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083BBA0, length 74 bytes (CThingHolySite vtable 0x012452DC slot 4).
// Identity: Ego_r.pdb / FableWin.pdb ?ConstructFromLevelScript@CThingHolySite@@UAE_NPBD@Z (the catalog's "vfunc_4").
// Callees: CThingGameObject::ConstructFromLevelScript 0x00838930 (called by name), CThing::PeekDefinitionManager 0x004C7990,
//   CDefinitionManager::GetDef<CThingHolySiteDef> 0x0083BD10.
// Shape: the argument is pushed straight from its slot; edi is saved only inside the success arm and holds &Def (+0xD0) across both
//   calls; the def's PlayerStart into the byte at +0xD4 (`setne al`).
// Name fix (agentNF9 2026-11-11): moved off the local models onto the shared headers; 0x00838930 binds
//   ?ConstructFromLevelScript@CThingGameObject@@UAE_NPBD@Z (was ..UAE_NABUCLevelScriptThingInfo@@@Z), 0x004C7990
//   ?PeekDefinitionManager@CThing@@QBEABVCGameDefinitionManager@@XZ, 0x0083BD10 the PDB GetDef<CThingHolySiteDef>; the row's own
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
#define FABLE_RETAIL_CTHINGHOLYSITE_DEFINITION
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTHINGGAMEOBJECT_LEVELSCRIPT
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingHolySite.h"

bool CThingHolySite::ConstructFromLevelScript(const char* script)
{
    if (CThingGameObject::ConstructFromLevelScript(script)) {
        PeekDefinitionManager().GetDef(DefGlobalIndex, &Def);
        PlayerStart = PeekDefinition().PlayerStart != 0;
        return true;
    }
    return false;
}
