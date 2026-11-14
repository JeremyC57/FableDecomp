// CTCTargeted::GetAllTargetPoints(std::vector<std::pair<C3DVector, long> >& points, float scale) const -- list every targeting-cursor helper point of the thing with its index, or the single default targeting position when the graphic has none
// Leaf: ?GetAllTargetPoints@CTCTargeted@@QBEXAAV?$vector@U?$pair@VC3DVector@@J@std@@V?$allocator@U?$pair@VC3DVector@@J@std@@@2@@std@@M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007EA580, 457 bytes, three `ret 8` (last 0x007EA746).
// Same-TU bodies above the row, in retail order (texts equal to the landed rows): CTCTargeted::GetNumTargetPoints 0x007EA2D0
//   (expanded), NTargeting::GetTargetingPositionOnThing 0x007EA2F0 (called; visible body = the returned position is not captured),
//   CTCTargeted::GetTargetPoint 0x007EA490 (called in the loop, expanded for the default point).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCTARGETED
#define FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FLARE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_HELPERNAMES
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CTCTARGETED_GETALLTARGETPOINTS
#define FABLE_RETAIL_STLP_SET_EMPTY
#define FABLE_RETAIL_STLP_MAP_EMPTY
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_BULLETTIME_CONST
#define FABLE_RETAIL_CBULLETTIMEMANAGER
#define FABLE_RETAIL_CBULLETTIMEMANAGER_INTERPOLATE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_INTERPOLATED_POSITION
#define FABLE_RETAIL_CTCPHYSICSBASE_INTERPOLATED_POSITION_BODY
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CTCBase.h"
#include "engine/CWorld.h"
#include "engine/CBulletTimeManager.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/NTargeting.h"
#include "engine/CTCTargeted.h"

extern const CCharString TARGETING_CURSOR_POSITION_NAME;                 // 0x013BAC50 "TARGET_CURSOR_POINT"

extern const CCharString CAMERA_FOCAL_POSITION_NAME;                     // landed static initialiser 0x0121E6E0
extern const CCharString STAB_POS;
extern const CCharString MAIN_FOCAL_POSITION_NAME;                       // landed static initialiser 0x0121E6C0

// CTCTargeted::GetNumTargetPoints 0x007EA2D0 (landed row; same TU, expanded in GetAllTargetPoints).
long CTCTargeted::GetNumTargetPoints() const
{
    const NGraphicAppearance::CTCGraphicAppearance* appearance = ParentThing->GetTCGraphicAppearance();
    if (appearance)
        return appearance->GetNoHelperPoints(TARGETING_CURSOR_POSITION_NAME, 1.0f);
    return 0;
}

// NTargeting::GetTargetingPositionOnThing 0x007EA2F0 (landed row; same TU, defined before the CTCTargeted methods: with its body
// visible c2 knows the returned position is never captured).
C3DVector __fastcall NTargeting::GetTargetingPositionOnThing(const CWorld& world, const CThing& thing, float scale)
{
    if (world.GetBulletTimeManager().IsBulletTimeActive() && thing.IsAffectedByBulletTime())
        scale = world.GetBulletTimeManager().ConvertInterpolateToBulletTimeInterpolate(scale);
    C3DVector pos;
    const NGraphicAppearance::CTCGraphicAppearance* appearance = thing.GetTCGraphicAppearance();
    if (appearance) {
        if (appearance->GetHelperPoint(TARGETING_CURSOR_POSITION_NAME, 0, 0x20, pos, scale))
            return pos;
        if (appearance->GetHelperPoint(CAMERA_FOCAL_POSITION_NAME, 0, 0x20, pos, scale))
            return pos;
        if (appearance->GetHelperPoint(STAB_POS, 0, 0x20, pos, scale))
            return pos;
        if (appearance->GetHelperPoint(MAIN_FOCAL_POSITION_NAME, 0, 0x20, pos, scale))
            return pos;
    }
    pos = thing.GetPhysicsTC()->GetPosition(scale) + C3DVector(0.0f, 0.0f, 0.25f);
    return pos;
}

// CTCTargeted::GetTargetPoint 0x007EA490 (landed row; same TU).
C3DVector CTCTargeted::GetTargetPoint(long index, float radius) const
{
    if (index >= 0) {
        const NGraphicAppearance::CTCGraphicAppearance* appearance = ParentThing->GetTCGraphicAppearance();
        C3DVector point;
        if (appearance->GetHelperPoint(TARGETING_CURSOR_POSITION_NAME, index, 0x20, point, 1.0f))
            return point;
    }
    const CThing& thing = *ParentThing;
    return NTargeting::GetTargetingPositionOnThing(PeekWorld(), thing, radius);
}

void CTCTargeted::GetAllTargetPoints(std::vector<std::pair<C3DVector, long> >& points, float scale) const
{
    long num_points = GetNumTargetPoints();
    long i = 0;
    if (num_points != 0) {
        for (; i != num_points; ++i)
            points.push_back(std::pair<C3DVector, long>(GetTargetPoint(i, scale), i));
    } else {
        points.push_back(std::pair<C3DVector, long>(GetTargetPoint(-1, scale), 0));
    }
}
