// CThingBuilding::PeekPSimDefinition() const -- the building def's sim-building def, fetched by index through the definition manager into a counted pointer; a null pointer when the building def names none
// Leaf: ?PeekPSimDefinition@CThingBuilding@@QBE?AV?$CDefPointer@$$CBVCSimBuildingDef@@@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082D2D0, 221 bytes. `ret 4` (the hidden result).
// Identity: Ego_r.pdb / FableWin.pdb ?PeekPSimDefinition@CThingBuilding@@QBE?AV?$CDefPointer@$$CBVCSimBuildingDef@@@@XZ (caller 0x0090BC1C).
// Callees: CThing::PeekDefinitionManager 0x004C7990 (x2), CDefinitionManager::GetDef<CThingBuildingDef> 0x004AB840 (x2) and
//   GetDef<CSimBuildingDef> 0x0082E1E0, CDefClassBase virtual slot 1 (Delete).
// Shape: the inline PeekDefinition() (expanded twice in retail): a zeroed holder filled by GetDef and released at once, the raw def
//   pointer read after the release (`[edi+74h]`); the result holder is zeroed, filled, copied into the hidden result (add-ref) and
//   released; the empty arm stores a null result.
// Layout: CThing DefGlobalIndex +0x8C; CThingBuildingDef::SimBuildingDef +0x74 (Ego_r.pdb +0x78).
// Name fix (agentNF9 2026-11-11): moved off the local CThingBuilding / CThingBuildingDef / CDefinitionManager / CDefPointer models (two
//   same-TU stand-in GetDef bodies) onto the shared headers; 0x004C7990 binds ?PeekDefinitionManager@CThing@@QBEABVCGameDefinitionManager@@XZ,
//   0x004AB840 ??$GetDef@VCThingBuildingDef@@@..., 0x0082E1E0 ??$GetDef@VCSimBuildingDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCSimBuildingDef@@@@@Z
//   (were ?GetDefinitionManager@CThingBuilding@@... / ?GetDef@CDefinitionManager@@QAE_NJPAV?$CDefPointer@V..@@@@@Z).
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

CDefPointer<const CSimBuildingDef> CThingBuilding::PeekPSimDefinition() const
{
    if (PeekDefinition().SimBuildingDef) {
        CDefPointer<const CSimBuildingDef> sim;
        PeekDefinitionManager().GetDef(PeekDefinition().SimBuildingDef, &sim);
        return sim;
    }
    return CDefPointer<const CSimBuildingDef>();
}
