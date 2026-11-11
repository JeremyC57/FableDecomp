// CThingPhysicalSwitch::PeekDefinition() const -- the physical switch's definition, fetched through a temporary counted def pointer
// Leaf: ?PeekDefinition@CThingPhysicalSwitch@@IBEABVCThingPhysicalSwitchDef@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083D900, length 60 bytes (+ int3 padding).
// Identity: Ego_r.pdb ?PeekDefinition@CThingPhysicalSwitch@@IBEABVCThingPhysicalSwitchDef@@XZ (protected const, returns a reference).
//   The landed name "CThingDeadCreature::PeekDefinition" was wrong: the row sits in the CThingPhysicalSwitch block (OnCreate 0x0083D6F0 ..
//   UntriggerMe 0x0083DF20) and its only callers are CThingPhysicalSwitch::TriggerMe 0x0083DDC0 and UntriggerMe 0x0083DF20, which also
//   call its GetDef 0x0083E0D0 directly; that instantiation is GetDef<CThingPhysicalSwitchDef> (FableWin.pdb
//   ??$GetDef@VCThingPhysicalSwitchDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCThingPhysicalSwitchDef@@@@@Z) (RENAMES.tsv).
// Callees: CThing::PeekDefinitionManager 0x004C7990, CDefinitionManager::GetDef<CThingPhysicalSwitchDef> 0x0083E0D0, the def's Delete
//   (slot 1) when the count drops to zero.
// Shape: the handle is zero-initialised in the `push ecx` slot, the arguments are pushed before the accessor runs; the filled pointer is
//   loaded once into esi, released through it (`dec [esi+4] ; jne`) and returned.
// Name fix (agentNF9 2026-11-11): moved off the local models (a same-TU stand-in GetDef_0083e0d0 body) onto the shared headers.
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
#define FABLE_RETAIL_CTHINGPHYSICALSWITCH_PEEKDEFINITION
#define FABLE_RETAIL_CTHINGBASEDEF
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingPhysicalSwitchDef.h"
#include "engine/CThingPhysicalSwitch.h"
#include "engine/CGameDefinitionManager.h"

const CThingPhysicalSwitchDef& CThingPhysicalSwitch::PeekDefinition() const
{
    CDefPointer<const CThingPhysicalSwitchDef> def;
    PeekDefinitionManager().GetDef(DefGlobalIndex, &def);
    return *def;
}
