// CThingBuilding::GetReverbSettings(EReverbEnvironmentType& type, float& level) const -- copies the building def's reverb environment type and level out
// Leaf: ?GetReverbSettings@CThingBuilding@@QBEXAAW4EReverbEnvironmentType@@AAM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082D8C0, 84 bytes (`ret 8`).
// Identity: Ego_r.pdb / FableWin.pdb ?GetReverbSettings@CThingBuilding@@QBEXAAW4EReverbEnvironmentType@@AAM@Z (public const, two
//   reference outs); Ego_r donor order UseHighDetailQuadTree / GetReverbSettings = retail 0x0082D870 / 0x0082D8C0; the body copies
//   the def's ReverbEnvironmentType / ReverbEnvironmentLevel pair (called from 0x006B206B). The landed "GetReverbEnvironment" was
//   descriptive (RENAMES.tsv).
// Callees: CThing::PeekDefinitionManager 0x004C7990, CDefinitionManager::GetDef<CThingBuildingDef> 0x004AB840, the def's Delete
//   (vtable slot 1) on the last release.
// Shape: the inline PeekDefinition() (expanded in retail) fills a counted handle and releases it before the reads.
// Layout: CThing::DefGlobalIndex +0x8C; CThingBuildingDef ReverbEnvironmentType +0x90, ReverbEnvironmentLevel +0x94 (Ego_r +0x94/+0x98).
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

void CThingBuilding::GetReverbSettings(EReverbEnvironmentType& type, float& level) const
{
    const CThingBuildingDef& def = PeekDefinition();
    type = def.ReverbEnvironmentType;
    level = def.ReverbEnvironmentLevel;
}
