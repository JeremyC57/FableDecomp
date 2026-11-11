// CThingObject::PeekPMaterial() const -- the object def's material def, fetched by index through the definition manager into a counted pointer; a null pointer when the def names none
// Leaf: ?PeekPMaterial@CThingObject@@MBE?AV?$CDefPointer@$$CBVCMaterialDef@@@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00703150, length 104 bytes (`ret 4`, + int3 padding). CThingObject vtable 0x01264DCC
//   slot 58 (the catalog's "vfunc_58"; CThingPhysical's ?PeekPMaterial@CThingPhysical@@UBE?AV?$CDefPointer@$$CBVCMaterialDef@@@@XZ slot).
// Identity: Ego_r.pdb ?PeekPMaterial@CThingObject@@MBE?AV?$CDefPointer@$$CBVCMaterialDef@@@@XZ (protected const virtual).
// Callees: CThing::PeekDefinitionManager 0x004C7990, CDefinitionManager::GetDef<CMaterialDef> 0x0066C340, the def's Delete (slot 1)
//   on release.
// Layout: CThingObject::PDef (CDefPointer<const CThingObjectDef>) +0xD0 (Ego_r +0xE8); CThingObjectDef::Material +0x84.
// Shape: returned by value through the hidden pointer; the hit arm builds a zeroed handle, fills it through GetDef and returns it by
//   copy (no NRVO: the copy's add-ref and the local's release share one null test), the miss arm returns a null pointer.
// Name fix (agentNF9 2026-11-11): moved off the local CThingObject / CThingObjectDef / CDefinitionManager / CDefPointer models (a same-TU
//   stand-in GetDef body) onto the shared headers; 0x004C7990 binds ?PeekDefinitionManager@CThing@@QBEABVCGameDefinitionManager@@XZ
//   (was ?GetDefinitionManager@CThingObject@@QBEPAVCDefinitionManager@@XZ) and 0x0066C340
//   ??$GetDef@VCMaterialDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCMaterialDef@@@@@Z (was the non-template QAE spelling);
//   the row's own symbol is the protected virtual (was QBE).
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
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingObject.h"

CDefPointer<const CMaterialDef> CThingObject::PeekPMaterial() const
{
    if (PeekDefinition().Material) {
        CDefPointer<const CMaterialDef> material;
        PeekDefinitionManager().GetDef(PeekDefinition().Material, &material);
        return material;
    }
    return CDefPointer<const CMaterialDef>();
}
