// CThingBuilding::UseHighDetailQuadTree() const -- whether the building def asks for the high-detail quad tree
// Leaf: ?UseHighDetailQuadTree@CThingBuilding@@UBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082D870, 66 bytes.
// Identity: CThingBuilding vtable 0x0124509C slot 57 (CThingPhysical's ?UseHighDetailQuadTree@CThingPhysical@@UBE_NXZ slot); Ego_r.pdb
//   ?UseHighDetailQuadTree@CThingBuilding@@UBE_NXZ, donor order GetFloorMaterial / UseHighDetailQuadTree / GetReverbSettings = retail
//   0x0082D7D0 / 0x0082D870 / 0x0082D8C0. The landed "UsesHighDetailQuadTree" was a misspelling (RENAMES.tsv).
// Callees: CThing::PeekDefinitionManager 0x004C7990, CDefinitionManager::GetDef<CThingBuildingDef> 0x004AB840, the def's Delete
//   (vtable slot 1) on the last release.
// Shape: the inline PeekDefinition() (expanded in retail) fills a counted handle and releases it before the bit read
//   (`mov al,[esi+0A0h] ; and al,1`: a bool:1 bitfield).
// Layout: CThing::DefGlobalIndex +0x8C; CThingBuildingDef::UseHighDetailQuadTree bit 0 of +0xA0 (Ego_r +0xA4).
// Name fix (agentNF9 2026-11-11): moved off the local CThingBuilding / CThingBuildingDef / CDefinitionManager / CDefPointer models (a
//   same-TU stand-in GetDef body) onto the shared headers; 0x004C7990 binds ?PeekDefinitionManager@CThing@@QBEABVCGameDefinitionManager@@XZ
//   and 0x004AB840 ??$GetDef@VCThingBuildingDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCThingBuildingDef@@@@@Z.
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
#define FABLE_RETAIL_CTHINGBUILDING
#define FABLE_RETAIL_CTHINGBUILDINGDEF
#define FABLE_RETAIL_CTHINGBUILDING_DEFINITION
#define FABLE_RETAIL_CTHINGBUILDING_PEEKSIMDEF
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CThingBuilding.h"

bool CThingBuilding::UseHighDetailQuadTree() const
{
    return PeekDefinition().UseHighDetailQuadTree;
}
