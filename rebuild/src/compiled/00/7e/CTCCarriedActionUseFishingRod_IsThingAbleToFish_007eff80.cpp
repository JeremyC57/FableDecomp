// 007EFF80 CTCCarriedActionUseFishingRod::IsThingAbleToFish -- test water depth, shore height and clear casting sight.
// Leaf: ?IsThingAbleToFish@CTCCarriedActionUseFishingRod@@SI_NABVCThingCreatureBase@@@Z
// Retail: 0x007EFF80,619 bytes,static fastcall.
// Flags: none
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_HEIGHTZBELOW
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_PASS22_STATICFOCAL
#define FABLE_RETAIL_CTCACTIONUSEBASE_VSLOTS
#define FABLE_RETAIL_CTCCARRIEDACTIONUSEFISHINGROD_PASS22
#define FABLE_RETAIL_CFISHINGDEF_PASS22
#define FABLE_RETAIL_CTCFISHING_PASS22
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_PASS22_WATERHEIGHT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD
#define FABLE_RETAIL_CGAMECAMERAMANAGER_CAMTOOLS_TYPED
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS_PASS22_LOS
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCFishing.h"
#include "engine/CTCCarriedActionUseFishingRod.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CGameCameraManager.h"
#include "engine/NCameraModes_CTools.h"
bool FABLE_FASTCALL CTCCarriedActionUseFishingRod::IsThingAbleToFish(const CThingCreatureBase& fisher)
{
    C3DVector facing_dir=fisher.GetPhysicsTC()->GetFacingVector();
    facing_dir.Normalise();
    facing_dir*=4.0f;
    C3DVector fishing_pos=fisher.GetPos()+facing_dir;
    const CWorldMap& world_map=CMainGameComponent::Get()->GetWorld().PeekWorldMap();
    float water_depth=world_map.GetWaterSizeZAt(fishing_pos);
    if(water_depth>0.05f) {
        C3DVector check_pos=fishing_pos;
        check_pos.Z+=0.5f;
        float check_depth=fisher.GetHeightZBelowAt(check_pos,true,true);
        if(check_depth<water_depth) {
            const CTCFishing* ptc_fishing;
            fisher.PeekTC(&ptc_fishing);
            CDefPointer<const CFishingDef> pdef=ptc_fishing->GetPDef();
            float ground_height=world_map.GetGroundSizeZAt(fishing_pos);
            if(water_depth-ground_height>pdef->FishingSpotMinDepth) {
                float height_diff=world_map.GetGroundSizeZAt(fisher.GetPos())-water_depth;
                if(height_diff>=pdef->FishingSpotMinHeightDiff&&height_diff<=pdef->FishingSpotMaxHeightDiff) {
                    const CGameCameraManager* pcam_man;
                    CMainGameComponent::Get()->GetWorld().PeekGameCamera()->GetCameraManager(&pcam_man);
                    const CThing* phit_thing;
                    C3DVector normal_vector;
                    float camera_radius;
                    C3DVector collision_pos;
                    if(pcam_man->GetCameraTools().IsLineOfSight(fisher.GetTCGraphicAppearance()->GetStaticFocalPos(),check_pos,collision_pos,normal_vector,camera_radius,&phit_thing,NCameraModes::CTools::TEST_ALL_BUT_CREATURES,false))
                        return true;
                }
            }
        }
    }
    return false;
}