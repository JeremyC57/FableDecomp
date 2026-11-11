// CThingCreatureBase::SerialiseGameState(CPersistContext& context) -- serialise the physical thing's state; unless the context is saving, re-fetch the creature definition into PDef
// Leaf: ?SerialiseGameState@CThingCreatureBase@@UAE_NAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006628E0, length 74 bytes (+ int3 padding).
// Identity: Ego_r.pdb / FableWin.pdb ?SerialiseGameState@CThingCreatureBase@@UAE_NAAVCPersistContext@@@Z (public virtual, slot 3).
// Callees: CThingPhysical::SerialiseGameState 0x006A5AF0 (called by name), CThing::PeekDefinitionManager 0x004C7990,
//   CDefinitionManager::GetDef<CThingCreatureDef> 0x00513160.
// Shape: the context pointer is kept in edi across the base call; the inline CPersistContext::IsSaving (Mode +0x18 against 1 and 3)
//   skips the definition re-fetch.
// Name fix (agentNF9 2026-11-11): moved off the local CThingPhysical / CThingCreatureBase / CPersistContext / CDefinitionManager models
//   onto the shared headers; 0x004C7990 binds ?PeekDefinitionManager@CThing@@QBEABVCGameDefinitionManager@@XZ, 0x00513160 the PDB
//   GetDef<CThingCreatureDef>, 0x006A5AF0 ?SerialiseGameState@CThingPhysical@@UAE_NAAVCPersistContext@@@Z (was QAE); the row's own
//   symbol is the public virtual (was QAE).
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
#define FABLE_RETAIL_CTHINGCREATUREBASE_CONSTRUCT
#define FABLE_RETAIL_CTHINGPHYSICAL_SERIALISE
#define FABLE_RETAIL_CPERSISTCONTEXT
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CPersistContext.h"

bool CThingCreatureBase::SerialiseGameState(CPersistContext& context)
{
    if (CThingPhysical::SerialiseGameState(context)) {
        if (!context.IsSaving())
            PeekDefinitionManager().GetDef(DefGlobalIndex, &PDef);
        return true;
    }
    return false;
}
