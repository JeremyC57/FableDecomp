// CThingBuilding::PeekPCameraManagerSetDefinition() const -- camera set
// Leaf: PeekPCameraManagerSetDefinition
#include "fable_compiler.h"
// NF49 (round 7): the def, region and manager classes come from the shared headers (were local CThingBuildingDef / CCameraManagerSetDef / CDefClassBase / CRegionDef / CCharString / CDefPointer / CDefinitionManager / CRegion models
// with same-TU stand-in GetDef bodies over made-up FindDef_CDefinitionManager_GetDef_0/1/2 callees): GetDef 0x004AB990 is the member
// template ??$GetDef@VCCameraManagerSetDef@@@CDefinitionManager@@QBE_NABVCCharString@@PAV?$CDefPointer@$$CBVCCameraManagerSetDef@@@@@Z,
// with its by-index siblings 0x004AB920 (CCameraManagerSetDef) and 0x004AB840 (CThingBuildingDef) (FableWin.pdb), the bodies visible
// and kept out of line (FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_BODY / _GETDEF_GLOBAL_BODY). The building def's +0x9C and the
// region def's +0x4C are CameraManagerSetIndex / CameraManagerSetBuildingIndex (engine/CThingBuildingDef.h, CRegionDef.h); the
// region's def comes from the inline CRegion::GetPRegionDef (FableWin.pdb). The row's own return type is now the PDB's
// CDefPointer<const CCameraManagerSetDef> (Ego_r.pdb / FableWin.pdb, RENAMES section 1).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD   // NF346: GetDef<T> copies = retail (the re-reading SetDynamicCast, owner 0x0066C340; as 0x0066B370)
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_BODY
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTHINGBUILDINGDEF
#define FABLE_RETAIL_CREGION
#define FABLE_RETAIL_CREGION_PREGIONDEF
#include "fable_string.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CDefinitionManager.h"
#include "engine/CThingBuildingDef.h"
#include "engine/CRegionDef.h"
#include "engine/CRegion.h"
#include "engine/CCameraManagerSetDef.h"
class CWorldMap { public:
    long GetLoadedRegion() const;            // 0x004FB150
public:
    const CRegion& GetRegion(long) const;   // 0x004FC180
};
class CWorld { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12();
    virtual CWorldMap* GetWorldMap();              // slot 13 (+0x34)
};
class CThingBuilding { public:
    unsigned char  _pad00[0x8c];
    unsigned short DefGlobalIndex;                   // +0x8c
    CDefinitionManager* GetDefinitionManager() const;   // 0x004C7990
    CWorld* GetWorld() const;                       // 0x004C7AB0
    FABLE_FORCEINLINE const CThingBuildingDef* PeekPDef() const
    {
        CDefPointer<const CThingBuildingDef> def;
        GetDefinitionManager()->GetDef(DefGlobalIndex, &def);
        return def.Get();
    }
    CDefPointer<const CCameraManagerSetDef> PeekPCameraManagerSetDefinition() const;
};
CDefPointer<const CCameraManagerSetDef> CThingBuilding::PeekPCameraManagerSetDefinition() const
{
    CDefPointer<const CCameraManagerSetDef> def;
    if (PeekPDef()->CameraManagerSetIndex > 0) {
        GetDefinitionManager()->GetDef(PeekPDef()->CameraManagerSetIndex, &def);
        return def;
    }
    CDefPointer<const CRegionDef> regionDef(GetWorld()->GetWorldMap()->GetRegion(GetWorld()->GetWorldMap()->GetLoadedRegion()).GetPRegionDef());
    const CRegionDef* rdef = regionDef.Get();
    if (rdef && rdef->CameraManagerSetBuildingIndex > 0) {
        GetDefinitionManager()->GetDef(rdef->CameraManagerSetBuildingIndex, &def);
        return def;
    }
    GetDefinitionManager()->GetDef("CAMERA_MANAGER_SET_BUILDING", &def);
    return def;
}