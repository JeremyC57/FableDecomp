// CThingObject::SerialiseGameState(CPersistContext& context) -- base CThingPhysical serialisation; unless the context is saving, re-fetches the object def by its global index
// Leaf: ?SerialiseGameState@CThingObject@@MAE_NAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007030C0, length 74 bytes (CThingObject vtable 0x01264DCC slot 3).
// Identity: Ego_r.pdb ?SerialiseGameState@CThingObject@@MAE_NAAVCPersistContext@@@Z (protected virtual; the catalog's "vfunc_3").
// Callees: CThingPhysical::SerialiseGameState 0x006A5AF0 (called by name), CThing::PeekDefinitionManager 0x004C7990,
//   CDefinitionManager::GetDef<CThingObjectDef> 0x0042B040.
// Shape: the context stays in edi for the inline CPersistContext::IsSaving (`Mode` +0x18 against 1 and 3); the GetDef arguments
//   (the 16-bit def index at +0x8C, &PDef at +0xD0) are pushed before the manager call.
// Name fix (agentNF9 2026-11-11): moved off the local CThingPhysical / CThingObject / CPersistContext / CDefinitionManager models onto
//   the shared headers; 0x004C7990 binds ?PeekDefinitionManager@CThing@@QBEABVCGameDefinitionManager@@XZ, 0x0042B040
//   ??$GetDef@VCThingObjectDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCThingObjectDef@@@@@Z (was
//   ?GetDef@CDefinitionManager@@QAE_NJAAUCDefPointerThingObjectDef@@@Z); the row's own symbol is the protected virtual (was UAE).
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
#define FABLE_RETAIL_CTHINGPHYSICAL_SERIALISE
#define FABLE_RETAIL_CPERSISTCONTEXT
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingObject.h"
#include "engine/CPersistContext.h"

bool CThingObject::SerialiseGameState(CPersistContext& context)
{
    if (CThingPhysical::SerialiseGameState(context)) {
        if (!context.IsSaving())
            PeekDefinitionManager().GetDef(DefGlobalIndex, &PDef);
        return true;
    }
    return false;
}
