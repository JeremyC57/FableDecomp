// CThingSwitch::PeekDefinition() const -- the switch's def, fetched from the definition manager by the thing's def index and returned as a counted pointer
// Leaf: ?PeekDefinition@CThingSwitch@@IBE?AV?$CDefPointer@$$CBVCThingSwitchDef@@@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0083C1E0, 76 bytes (+ int3 padding).
// Identity: Ego_r.pdb ?PeekDefinition@CThingSwitch@@IBE?AV?$CDefPointer@$$CBVCThingSwitchDef@@@@XZ (protected const, the def by value): the
//   body returns a counted def pointer through the hidden pointer (`ret 4`) after CDefinitionManager::GetDef on the thing's own def index,
//   and its caller is CThingSwitch's WorldUpdate (vtable slot 19, 0x0083CD70). The landed "GetSwitchDef" name and the manifest label
//   "CThingBuilding::GetFloorMaterial" are invented / propagated (RENAMES.tsv).
// Callees: CThing::PeekDefinitionManager 0x004C7990, CDefinitionManager::GetDef<CThingSwitchDef> 0x0083CFA0 (FableWin.pdb
//   ??$GetDef@VCThingSwitchDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCThingSwitchDef@@@@@Z), the def's Release.
// Layout: CThing::DefGlobalIndex (unsigned short) at retail +0x8c.
// Shape: a CDefPointer local filled by the manager (the header's noinline GetDef body), returned by value through the hidden pointer:
//   the copy constructor's add-ref and the local's release share one null test and keep both count stores.
// Name fix (agentNF8 2026-11-11): moved off the local CDefinitionManager / CDefPointer / CThingSwitch stand-ins (a same-TU stand-in GetDef
// body) onto the shared headers: 0x004C7990 binds ?PeekDefinitionManager@CThing@@QBEABVCGameDefinitionManager@@XZ (was
// ?GetDefinitionManager@...), 0x0083CFA0 the PDB GetDef<CThingSwitchDef> (was ?GetDef@CDefinitionManager@@QAE_NJPAV?$CDefPointer@VCThingPhysicalSwitchDef@@@@@Z).
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
#define FABLE_RETAIL_CTHINGSWITCH_PEEKDEFINITION
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CThingSwitch.h"
#include "engine/CGameDefinitionManager.h"

CDefPointer<const CThingSwitchDef> CThingSwitch::PeekDefinition() const
{
    CDefPointer<const CThingSwitchDef> def;
    PeekDefinitionManager().GetDef(DefGlobalIndex, &def);
    return def;
}
