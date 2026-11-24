// CTCSummonSpell::IsValidTeleportPos — check map availability and collisions at the destination.
// Leaf: ?IsValidTeleportPos@CTCSummonSpell@@AAE_NABVC3DVector@@ABVCWorldMap@@PBVCThing@@@Z
// Flags: default
// Inline-budget: the 0x0079BF30 search and 0x004AB490 list base cleanup keep their original bodies visible and out of line.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS_FIRST
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_STATTYPE
#define FABLE_RETAIL_CPHYSICSCOLLIDERESPONSE_CTOR
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_CLIPAREA
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_NOINLINE_PASS27
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_STATTYPE
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_STLP_LIST_BASE_DTOR_VISIBLE_NOINLINE
#define FABLE_RETAIL_CTCSUMMONSPELL
#define FABLE_RETAIL_CTCSUMMONSPELL_TELEPORT
#include "engine/C3DVector.h"
#include "engine/C2DBoxI.h"
#include "engine/CThing.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CPhysicsCollideResponse.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CTCMapwho.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_None.h"
#include "engine/CWorldMap.h"
#include "engine/CTCSummonSpell.h"

bool CTCSummonSpell::IsValidTeleportPos(const C3DVector& pos, const CWorldMap& worldmap, const CThing* minion)
{
    const CTCPhysicsBase* ptc_creature_physics;
    if (minion->PeekTC(&ptc_creature_physics)) {
        long map_no = worldmap.GetMapNumberAt(pos);
        if (worldmap.IsMapLoaded(map_no) && worldmap.IsPosPassable(pos.ToC2DCoordI())) {
            bool collided = false;
            float radius = ptc_creature_physics->GetRadius() * 2;
            if (radius < 1.0f)
                radius = 1.0f;
            C2DBoxI search_area(pos.ToC2DCoordI(), (long)radius);
            std::list<CThing*> things;
            PeekThingSearchTools().GetAllThingsInRoughArea<CThing>(search_area, CThingFilter_None(), things);
            std::list<CThing*>::iterator it, endit;
            for (it = things.begin(), endit = things.end(); it != endit; ++it) {
                CThing* something = *it;
                if (something == minion)
                    continue;

                const CTCPhysicsBase* ptc_physics_thing;
                if (something->PeekTC(&ptc_physics_thing)) {
                    CPhysicsCollideResponse response;
                    if (ptc_creature_physics->HasCollidedWithThingFromTo(*ptc_physics_thing, minion->GetPos(),
                                                                       pos + C3DVector(0, 0, 0.1f), response)) {
                        collided = true;
                        break;
                    }
                }
            }
            return !collided;
        }
    }
    return false;
}