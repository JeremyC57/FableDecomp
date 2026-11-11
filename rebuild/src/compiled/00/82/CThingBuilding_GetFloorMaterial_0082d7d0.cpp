// CThingBuilding::GetFloorMaterial() const -- the building def's floor material
// Leaf: ?GetFloorMaterial@CThingBuilding@@UBEJXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082D7D0, 64 bytes.
// Identity: CThingBuilding vtable 0x0124509C slot 40 (CThing's ?GetFloorMaterial@CThing@@UBEJXZ slot); Ego_r.pdb
//   ?GetFloorMaterial@CThingBuilding@@UBEJXZ, donor order OnCreate / GetFloorMaterial = retail 0x0082D6F0 / 0x0082D7D0.
// Callees: CThing::PeekDefinitionManager 0x004C7990, CDefinitionManager::GetDef<CThingBuildingDef> 0x004AB840, the def's Delete
//   (vtable slot 1) on the last release.
// Shape: the inline PeekDefinition() (expanded in retail) fills a counted handle and releases it before the member read.
// Layout: CThing::DefGlobalIndex +0x8C; CThingBuildingDef::FloorMaterial +0x8C (Ego_r +0x90).
// Name fix (agentNF9 2026-11-11): moved off the local CThingBuilding / CThingBuildingDef / CDefinitionManager / CDefPointer models (a
//   same-TU stand-in GetDef body) onto the shared headers; 0x004C7990 binds ?PeekDefinitionManager@CThing@@QBEABVCGameDefinitionManager@@XZ
//   and 0x004AB840 ??$GetDef@VCThingBuildingDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCThingBuildingDef@@@@@Z; the row
//   is now the const virtual the PDB spells (was non-const ?GetFloorMaterial@CThingBuilding@@QAEJXZ).
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

long CThingBuilding::GetFloorMaterial() const
{
    return PeekDefinition().FloorMaterial;
}
