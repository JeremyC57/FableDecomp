// Leaf: ?IsThingCollision@CGameCameraBase@@AAE_NXZ
// Flags:
#define FABLE_RETAIL_CGAMECAMERABASE_RENDER
#define FABLE_RETAIL_CGAMECAMERABASE_UPDATE
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CCAMERA_RENDER_INTERPOLATION
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_LASTPOSITION
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_LINEOFSIGHT
#define FABLE_RETAIL_CWORLDMAP_FINDLASTUNBLOCKEDGROUND
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_CTHINGGAMEOBJECT_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGMAP
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTCMAPWHO_PASS22_SHARED_NODE
#define FABLE_RETAIL_CTHINGMAP_LAYERINFO
#define FABLE_RETAIL_MAP_VALIDATE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_COPY
#include "engine/CWorldMap.h"
#include "fable_string.h"
#include "engine/CGameCameraBase.h"
#include "engine/CThing.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CWorld.h"
#include "engine/CThingGameObject.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CTCMapwho.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_MinAllowedRadius.h"
#include "engine/CThingFilter_OverlappingSphere.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CMeshIntersectionData.h"
#include "engine/CPhysicsMeshObject.h"

bool CGameCameraBase::IsThingCollision()
{
    bool has_a_collision_occured = false;
    std::list<CThingGameObject*> thing_list;
    C3DVector cam_pos = InternalCam.PeekWorldPos();
    float distance = 5.0f;
    C2DBoxI area(C2DCoordI((long)PointingAt.X, (long)PointingAt.Y), (long)distance);
    World.GetThingSearchTools().GetAllThingsInRoughArea<CThingGameObject>(
        area, GFFilter_Bind2(CThingFilter_MinAllowedRadius(0.5f),
                            CThingFilter_OverlappingSphere(PointingAt, distance)), thing_list);
    DrawCoordList.clear();
    LocalThingPosList.clear();
    std::list<CThingGameObject*>::iterator it;
    for (it = thing_list.begin(); it != thing_list.end(); ++it)
    {
        CThingGameObject& object = **it;
        const CTCPhysicsBase* physics = object.GetPhysicsTC();
        LocalThingPosList.push_back(object.GetPos());
        if (physics->GetPhysicsMeshInfo())
        {
            CMeshIntersectionData intersection_data;
            CPhysicsMeshObject physics_object = physics->GetPhysicsMeshObject();
            GoingIntoBuilding = BuildingUID > 0;
            if (physics_object.GetPointIntersectionFromTo(PointingAt, cam_pos, intersection_data))
            {
                C3DVector displacement_vec(0.0f, 0.0f, 0.0f);
                if (BuildingUID > 0 && !GoingIntoBuilding)
                    displacement_vec.Z = 5.0f;
                if (BuildingUID == object.GetUniqueID())
                    DrawCoordList.push_front(intersection_data.IntersectionPosition + displacement_vec);
                else
                    DrawCoordList.push_back(intersection_data.IntersectionPosition + displacement_vec);
                has_a_collision_occured = true;
            }
        }
    }
    return has_a_collision_occured;
}