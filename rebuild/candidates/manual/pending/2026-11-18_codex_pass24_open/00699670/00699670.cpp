// CGameCameraManager::UpdateHeightLimits() -- sample the ground / theme / water heights and line of sight at the observed thing and the camera mode's position, and widen the camera's minimum / maximum height to the highest sample (plus the mode's clearances)
// Grade: DIFFER(759v761) with 28 strict instruction hunks.
// Leaf: ?UpdateHeightLimits@CGameCameraManager@@AAEXXZ
// Flags: none
// Retail: 0x00699670, 761 bytes.

#define FABLE_RETAIL_CGAMECAMERAMANAGER_HEIGHTLIMITS_PASS24
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_GROUNDFACENORMAL
#define FABLE_RETAIL_CWORLDMAP_PASS22_WATERHEIGHT
#define FABLE_RETAIL_CWORLDMAP_THEMEMINCAMERASIZEZOFFSET
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CGAMECAMERAMANAGER_FULL
#define FABLE_RETAIL_NCAMERAMODES_CBASE_CYLINDER
#define FABLE_RETAIL_NCAMERAMODES_CBASE_GETPOSITION_INLINE
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS_CTOR
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS_PASS22_LOS
#define FABLE_RETAIL_CCAMERA_CTORS
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_VECTOR_OVERFLOW_OUTOFLINE
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_IS_THING_ALIVE
#include "fable_string.h"
#include "engine/CThingGameObject.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/CGameCameraManager.h"
#include "engine/CCameraManagerDef.h"
#include "engine/NCameraModes_CBase.h"
#include "engine/ThingAlive.h"
#include "math.h"

#pragma intrinsic(cos)

void CGameCameraManager::UpdateHeightLimits()
{
    const float WATER_OFFSET = PDef->BaseHeightToKeepAboveWater;
    const float GROUND_OFFSET = PDef->BaseHeightToKeepAboveFlatLandscape;
    const float MAX_HEIGHT_RELATIVE_TO_GROUND = PDef->MaxHeightRelativeToGround;
    const float MAX_ANGLE_BETWEEN_GROUND_NORMAL_AND_UPWARD_VERTICAL_FOR_CONSIDERATION =
        PDef->AngleBetweenGroundNormalAndVerticalForLandscapeToBeConsideredNotFlat * 0.0027777778f;
    const C3DVector& player_pos = PObservedThing->GetPos();
    const CWorldMap& wmap = World.PeekWorldMap();
    const CThing* pmesh_to_check = PObservedThing->GetThingStandingOn();
    std::vector<const CThing*> standing_on_things;
    if (IS_THING_ALIVE(pmesh_to_check))
        standing_on_things.push_back(pmesh_to_check);
    C3DVector cam_pos = player_pos;
    if (!CurrentMode.IsNull())
        cam_pos = CurrentMode->GetPosition();
    float max_min_absolute_height = player_pos.Z;
    float min_max_absolute_height = player_pos.Z + MAX_HEIGHT_RELATIVE_TO_GROUND;
    std::vector<C3DVector> posns_to_check;
    posns_to_check.push_back(player_pos);
    posns_to_check.push_back(C3DVector(cam_pos));
    for (std::vector<C3DVector>::iterator it = posns_to_check.begin(); it != posns_to_check.end(); ++it) {
        const C3DVector& r_pos = *it;
        C3DVector normal = wmap.GetGroundFaceNormalAt(r_pos);
        float cos_thet = (normal.X + normal.Y) * 0.0f + normal.Z;
        if (cos_thet > cos(MAX_ANGLE_BETWEEN_GROUND_NORMAL_AND_UPWARD_VERTICAL_FOR_CONSIDERATION * 6.2831854820251465)) {
            float ground_height = wmap.GetGroundSizeZAt(r_pos);
            float theme_height = wmap.GetThemeMinCameraSizeZOffsetAt(r_pos) + ground_height;
            float water_height = wmap.GetWaterSizeZAt(r_pos);
            float mesh_height = ground_height;
            C3DVector coll_pos;
            C3DVector coll_normal;
            float cam_rad;
            const CThing* phit_thing;
            if (!CamTools.IsLineOfSight(standing_on_things, r_pos, C3DVector(r_pos.X, r_pos.Y, 0.0f),
                coll_pos, coll_normal, cam_rad, &phit_thing, NCameraModes::CTools::TEST_THINGS, false))
                mesh_height = coll_pos.Z;
            float min_absolute_height;
            if (mesh_height > theme_height) {
                if (mesh_height > water_height)
                    min_absolute_height = mesh_height + GROUND_OFFSET;
                else
                    min_absolute_height = water_height + WATER_OFFSET;
            } else if (theme_height > water_height)
                min_absolute_height = theme_height + GROUND_OFFSET;
            else
                min_absolute_height = water_height + WATER_OFFSET;
            float max_absolute_height = min_absolute_height + MAX_HEIGHT_RELATIVE_TO_GROUND;
            if (min_absolute_height > max_min_absolute_height)
                max_min_absolute_height = min_absolute_height;
            if (max_absolute_height > min_max_absolute_height)
                min_max_absolute_height = max_absolute_height;
        }
    }
    MinWorldHeight = max_min_absolute_height;
    MaxWorldHeight = min_max_absolute_height;
}
