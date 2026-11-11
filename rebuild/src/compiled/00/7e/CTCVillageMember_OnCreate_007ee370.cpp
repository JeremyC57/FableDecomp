// CTCVillageMember::OnCreate() -- when the owner has a definition index, fetches its definition and resolves the village-member sub-definition into PDef
// Leaf: ?OnCreate@CTCVillageMember@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007EE370, length 86 bytes. CTCVillageMember vtable 0x0124007C slot 4, the
// CTCBase OnCreate slot (PDB `?OnCreate@CTCVillageMember@@UAEXXZ`).
// Callees: CTCBase::GetDefinitionManager() const 0x006869D0, CDefinitionManager::GetDef<CThingBaseDef>(long, CDefPointer*) 0x0042AF3C,
// CDefClassBase::GetSubDef<CVillageMemberDef>(CDefPointer*) 0x007EE6E0, the def's Release.
// Shape: the def index is a zero-extended word of the owner (+0x8C); the counted local is zeroed after the argument pushes, GetDef's
// arguments are pushed before the manager call; GetDef is the header's noinline template body (out of line in retail), so the local's
// object is read once into esi for the GetSubDef call and its release.
// Layout: engine/CTCVillageMember.h (retail = PDB): PDef +0x0C.
// Name fix (agentNF8 2026-11-11): moved off the local CDefPointerClassBase / CDefClassBase / ReleaseDef stand-ins onto the shared
// headers: 0x0042AF3C binds GetDef<CThingBaseDef> (was GetDef<CDefClassBase>) and 0x007EE6E0 the PDB
// ??$GetSubDef@VCVillageMemberDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCVillageMemberDef@@@@@Z.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFPOINTER_RELEASE_SELFDELETING_POINTEE
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CTCVillageMember.h"
#include "engine/CGameDefinitionManager.h"

void CTCVillageMember::OnCreate()
{
    if (ParentThing->GetDefGlobalIndex() != 0) {
        CDefPointer<const CThingBaseDef> def;
        CThing* thing = ParentThing;
        GetDefinitionManager().GetDef(thing->GetDefGlobalIndex(), &def);
        def->GetSubDef(&PDef);
    }
}
