// CThingBuilding::PeekPMaterial() const -- the building def's wall material def, fetched by index through the definition manager into a counted pointer; a null pointer when the building def names none
// Leaf: ?PeekPMaterial@CThingBuilding@@MBE?AV?$CDefPointer@$$CBVCMaterialDef@@@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082D5C0, 227 bytes. `ret 4` (the hidden result).
// Identity: CThingBuilding vtable 0x0124509C slot 58 (CThingPhysical's ?PeekPMaterial@CThingPhysical@@UBE?AV?$CDefPointer@$$CBVCMaterialDef@@@@XZ
//   slot, 0x005297F0); Ego_r.pdb ?PeekPMaterial@CThingBuilding@@MBE?AV?$CDefPointer@$$CBVCMaterialDef@@@@XZ (protected const virtual),
//   donor order PeekPMaterial / GetMaterialOnHitByMelee = retail 0x0082D5C0 / 0x0082D6B0. Same shape as PeekPSimDefinition 0x0082D2D0.
// Callees: CThing::PeekDefinitionManager 0x004C7990 (x2), CDefinitionManager::GetDef<CThingBuildingDef> 0x004AB840 (x2) and
//   GetDef<CMaterialDef> 0x0066C340, CDefClassBase virtual slot 1 (Delete).
// Layout: CThing DefGlobalIndex +0x8C; CThingBuildingDef::WallMaterial +0x88 (Ego_r.pdb +0x8C).
// Name fix (agentNF9 2026-11-11): moved off the local models (two same-TU stand-in GetDef bodies, a CWallMaterial stand-in type) onto
//   the shared headers; 0x0066C340 binds ??$GetDef@VCMaterialDef@@@CDefinitionManager@@QBE_NJPAV?$CDefPointer@$$CBVCMaterialDef@@@@@Z
//   (was ?GetDef@CDefinitionManager@@QAE_NJPAV?$CDefPointer@UCWallMaterial@@@@@Z), 0x004AB840 / 0x004C7990 the PDB spellings; the
//   row's own symbol is now the protected virtual (was ?PeekPMaterial@CThingBuilding@@QBE?AV?$CDefPointer@UCWallMaterial@@@@XZ).
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

CDefPointer<const CMaterialDef> CThingBuilding::PeekPMaterial() const
{
    if (PeekDefinition().WallMaterial) {
        CDefPointer<const CMaterialDef> material;
        PeekDefinitionManager().GetDef(PeekDefinition().WallMaterial, &material);
        return material;
    }
    return CDefPointer<const CMaterialDef>();
}
