// NTargeting::GetTargetingPositionOnThing(const CWorld& world, const CThing& thing, float interpolate) -- the point to aim at on a thing: its first graphic-appearance targeting helper point (four helper names tried in order), else its interpolated physics position raised by a quarter unit; the interpolate is rescaled when bullet time applies to the thing
// Leaf: ?GetTargetingPositionOnThing@NTargeting@@YI?AVC3DVector@@ABVCWorld@@ABVCThing@@M@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007EA2F0, 411 bytes, __fastcall (hidden C3DVector result in ecx, world in edx), `ret 8`.
// Identity: Ego_r.pdb ?GetTargetingPositionOnThing@NTargeting@@YI?AVC3DVector@@ABVCWorld@@ABVCThing@@M@Z (0x007E4530).
// Callees: CThing::IsAffectedByBulletTime 0x004C9920, CBulletTimeManager::ConvertInterpolateToBulletTimeInterpolate 0x004C6010,
//   NGraphicAppearance::CTCGraphicAppearance::GetHelperPoint 0x004C0560 (x4, index 0, flags 0x20) with the helper-name globals
//   0x013BAC50 ("TARGET_CURSOR_POINT"), 0x013BAC4C, 0x013BACDC, 0x013BAC48; 0.25f 0x0123078C. Inline: CWorld::GetBulletTimeManager,
//   CBulletTimeManager::IsBulletTimeActive (0x00419680's body), CTCPhysicsBase::GetPosition(float) (0x0049D150's body).
#define FABLE_RETAIL_STLP_SET_EMPTY
#define FABLE_RETAIL_STLP_MAP_EMPTY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_BULLETTIME_CONST
#define FABLE_RETAIL_CBULLETTIMEMANAGER
#define FABLE_RETAIL_CBULLETTIMEMANAGER_INTERPOLATE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_INTERPOLATED_POSITION
#define FABLE_RETAIL_CTCPHYSICSBASE_INTERPOLATED_POSITION_BODY
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CWorld.h"
#include "engine/CBulletTimeManager.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/NTargeting.h"

extern CCharString g_CCharString_013bac50;          // "TARGET_CURSOR_POINT" (landed static initialiser 0x0121E700)
extern CCharString g_CCharString_013bac4c;          // landed static initialiser 0x0121E6E0
extern CCharString g_CCharString_013bacdc;
extern CCharString g_CCharString_013bac48;          // landed static initialiser 0x0121E6C0

C3DVector __fastcall NTargeting::GetTargetingPositionOnThing(const CWorld& world, const CThing& thing, float scale)
{
    if (world.GetBulletTimeManager().IsBulletTimeActive() && thing.IsAffectedByBulletTime())
        scale = world.GetBulletTimeManager().ConvertInterpolateToBulletTimeInterpolate(scale);
    C3DVector pos;
    const NGraphicAppearance::CTCGraphicAppearance* appearance = thing.GetTCGraphicAppearance();
    if (appearance) {
        if (appearance->GetHelperPoint(g_CCharString_013bac50, 0, 0x20, pos, scale))
            return pos;
        if (appearance->GetHelperPoint(g_CCharString_013bac4c, 0, 0x20, pos, scale))
            return pos;
        if (appearance->GetHelperPoint(g_CCharString_013bacdc, 0, 0x20, pos, scale))
            return pos;
        if (appearance->GetHelperPoint(g_CCharString_013bac48, 0, 0x20, pos, scale))
            return pos;
    }
    pos = thing.GetPhysicsTC()->GetPosition(scale) + C3DVector(0.0f, 0.0f, 0.25f);
    return pos;
}