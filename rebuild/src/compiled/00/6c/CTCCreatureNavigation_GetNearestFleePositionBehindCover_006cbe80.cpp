// CTCCreatureNavigation chooses a point behind cover and clips it to the map.
// Leaf: ?GetNearestFleePositionBehindCover@CTCCreatureNavigation@@QBE_NABVC3DVector@@AAV2@@Z
// Flags: /Gr
#define FABLE_RETAIL_CTHING_SCANINCLUSION_ISALIVE
#define FABLE_RETAIL_CREATURE_NAV_FLEE_COVER_PASS30
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_WORLDSEED
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_FRAMEUPDATE
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CMAP_WORLDACCESSORS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D_ORIGINAL_SUM_PASS23
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_C3DVECTOR_INITFROMANGLES_GFSIN
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTTHING_BODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTTHING_BODY_NULLTEST
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_CTHINGGAMEOBJECT_STATTYPEFLAGS
#define FABLE_RETAIL_STLP_CONST_MEM_FUN_REF

#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CThingSearchTools.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CMap.h"
#include "engine/CWorldMap.h"
#include "engine/CThingCompare_Nearest.h"
#include "engine/CThingFilter_In2DRange.h"
#include "engine/CFilter_Bind2.h"
#include "engine/CFilter_Not.h"
#include "engine/GFFloatRandom.h"
#include "engine/GFLimit.h"
#include "stlport/fable_algorithm.h"

bool CTCCreatureNavigation::GetNearestFleePositionBehindCover(const C3DVector& threat_pos, C3DVector& pos) const
{
    CThingCreatureBase& creature = GetParentCreature();
    const CMap& map = PeekWorldMap().PeekMapAt(creature.PeekPos().ToC2DCoordI());
    const C2DCoordI& map_world_pos = map.GetWorldPosition();
    CThingGameObject* object = 0;
    float chicken_sacrifice = 0.5f;
    const float threat_too_close_distance = 4.0f;
    if (PeekThingSearchTools().GetPBestThing(
            GFFilter_Bind2(std::mem_fun_ref(&CThingGameObject::IsCover),
                           GFFilter_NOT(CThingFilter_In2DRange(threat_pos, threat_too_close_distance))),
            CThingCompare_Nearest(creature.PeekPos()), &object)) {
        C3DVector vector = object->PeekPos() - threat_pos;
        vector.Z = 0.0f;
        float distance = vector.GetMagnitude2D();
        if (distance < DELTA_NOTIONAL_ZERO)
            vector.InitialiseFromAngles(GFFloatRandom(1.0f, GetPWorldSeed()), 0.0f, 1.0f);
        else
            vector.Normalise();
        float radius = object->GetPhysicsTC()->GetRadius() + creature.GetPhysicsTC()->GetRadius() + chicken_sacrifice;
        vector *= radius;
        C3DVector hide_pos = object->PeekPos() + vector;
        GFLimit(hide_pos.X, map_world_pos.X, map_world_pos.X + map.GetWorldSizeX());
        GFLimit(hide_pos.Y, map_world_pos.Y, map_world_pos.Y + map.GetWorldSizeY());
        hide_pos.Z = creature.GetMapSizeZBelowAt(hide_pos);
        pos = hide_pos;
        return true;
    }
    return false;
}