// 0085c470 NCameraModes::CMoveOnCylinder::Update
// Flags: none
// Leaf: ?Update@CMoveOnCylinder@NCameraModes@@UAEXXZ
// Retail: 0x0085C470, 857 bytes, thiscall.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP_CYLINDER_QUERY
#define FABLE_RETAIL_CWORLDMAP_HEIGHTZ
#define FABLE_RETAIL_NCAMERAMODES_CBASE_CYLINDER
#define FABLE_RETAIL_NCAMERAMODES_CBASE_INLINE_ACCESSORS
#define FABLE_RETAIL_NCAMERAMODES_CBASE_CYLINDER_INPUT
#define FABLE_RETAIL_CUSERPROFILEMANAGER_CYLINDER_INPUT
#define FABLE_RETAIL_CCAMERAMODEDEF_CYLINDER
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CCameraModeDef.h"
#include "engine/CThingGameObject.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCHitLocations.h"
#include "engine/CUserProfileManager.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/NCameraModes_CMoveOnCylinder.h"

namespace NCameraModes {
void CMoveOnCylinder::Update()
{
    const CThingGameObject* thing = GetObservedThing();
    if (!thing || (thing->IsDead()))
        return;
    {
        C2DVector cam_stick = GetLeftStickVector();
        if (CUserProfileManager::Get().IsCameraPCViewHeroXYInverted())
            cam_stick.X *= -1.0f;
        if (CUserProfileManager::Get().IsCameraPCViewHeroYZInverted())
            cam_stick.Y *= -1.0f;
        float XY_ROTATION_SPEED = GetCameraModeDef()->ViewHeroXYRotationMaxVelocity;
        float VERTICAL_ROTATION_SPEED = GetCameraModeDef()->ViewHeroHeightParamMaxVelocity;
        float cam_sensitivity = CUserProfileManager::Get().GetCameraSensitivity();
        if (cam_sensitivity < 0.0f)
            cam_sensitivity = 0.0f;
        else if (cam_sensitivity > 1.0f)
            cam_sensitivity = 1.0f;
        cam_sensitivity += 0.5f;
        Theta += XY_ROTATION_SPEED * cam_sensitivity * cam_stick.X;
        HeightParam += cam_sensitivity * VERTICAL_ROTATION_SPEED * cam_stick.Y;
        if (HeightParam < 0.0)
            HeightParam = 0.0f;
        else if (HeightParam > 1.0f)
            HeightParam = 1.0f;
    }

    float rad = GetObservedThing()->GetPhysicsTC()->GetRadius();
    float height_limit = GetCameraModeDef()->ViewHeroXYOffsetMultiplier;
    const CThingGameObject* observed = GetObservedThing();
    const CTCHitLocations* ptc_hit;
    observed->PeekTC(&ptc_hit);
    C3DVector centre_pos;
    float height;
    ptc_hit->GetBoundingSphere(centre_pos, height, false);
    height *= 2.0f;
    C3DVector xy_out = AxisX;
    xy_out.Rotate(AxisZ, Theta);
    C3DVector base_pos = GetObservedThing()->GetPos();
    C3DVector adjust_x = xy_out * rad * height_limit;
    C3DVector ground_test_pos = base_pos + adjust_x + AxisZ;
    C3DVector view_pos = base_pos + AxisZ * height * HeightParam;
    C3DVector new_pos = view_pos + adjust_x;
    float gheight = GetWorld().PeekWorldMap().GetHeightZBelowPos(ground_test_pos);
    gheight += GetCameraModeDef()->ViewHeroGroundHeightOffset;
    if (new_pos.Z < gheight)
        new_pos.Z = gheight;
    AddAttractorGoTo(new_pos, 1.0f);
    LookAt.Position = view_pos;
}
}