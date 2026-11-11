// CThingDeadCreature::PeekDefinition() const -- the creature definition of this dead creature, fetched through a temporary counted def pointer
// Leaf: ?PeekDefinition@CThingDeadCreature@@IBEABVCThingCreatureDef@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00835B90, length 60 bytes (+ int3 padding).
// Identity: Ego_r.pdb ?PeekDefinition@CThingDeadCreature@@IBEABVCThingCreatureDef@@XZ (protected const); out of line in retail, called by
//   OnCreate 0x00835FD0 and WorldUpdate 0x00836260.
// Callees: CThing::PeekDefinitionManager 0x004C7990, CDefinitionManager::GetDef<CThingCreatureDef> 0x00513160, the def's Delete
//   (slot 1) when the temporary drops the last reference.
// Shape: the handle is zero-initialised in the `push ecx` slot, the arguments are pushed before the accessor runs, the def is kept in
//   esi across the inline release and returned.
// Name fix (agentNF9 2026-11-11): moved off the local CThing / CThingDeadCreature / CDefinitionManager / CDefPointer models (a same-TU
//   stand-in GetDef body) onto the shared headers; 0x004C7990 binds ?PeekDefinitionManager@CThing@@QBEABVCGameDefinitionManager@@XZ,
//   0x00513160 ??$GetDef@VCThingCreatureDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCThingCreatureDef@@@@@Z (was
//   ?GetDef@CDefinitionManager@@QAE_NJAAV?$CDefPointer@VCThingCreatureDef@@@@@Z); the row's own symbol is the protected PDB spelling.
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
#define FABLE_RETAIL_CTHINGDEADCREATURE_PEEKDEFINITION
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureDef.h"
#include "engine/CThingDeadCreature.h"
#include "engine/CGameDefinitionManager.h"

const CThingCreatureDef& CThingDeadCreature::PeekDefinition() const
{
    CDefPointer<const CThingCreatureDef> def;
    PeekDefinitionManager().GetDef(DefGlobalIndex, &def);
    return *def;
}
