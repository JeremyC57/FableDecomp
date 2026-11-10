// CTCWeaponTrail::SetLastProjectileWeaponTrailPoint(const C3DVector&) -- replace the last projectile trail endpoints.
// Leaf: ?SetLastProjectileWeaponTrailPoint@CTCWeaponTrail@@QAEXABVC3DVector@@@Z
// Retail: 0x0076E290,84 bytes,__fastcall.
// Flags: none
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_REVERSE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CCAMERA_UP_DIRECTION
#include "engine/CTCWeaponTrail.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CCamera.h"
#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
namespace {
std::pair<C3DVector,C3DVector> FABLE_FASTCALL GetProjectileTrailElementFromPoint(const C3DVector& point)
{
    CCamera current_cam=CMainGameComponent::Get()->GetWorld().GetGameCamera()->GetRenderCamera(1.0f);
    const C3DVector& cam_up=current_cam.PeekUpDirection();
    C3DVector offset=cam_up*0.125f;
    return std::make_pair(point-offset*0.5f,point+offset*0.5f);
}
}
#ifdef _MSC_VER
#pragma auto_inline(on)
#endif
void CTCWeaponTrail::SetLastProjectileWeaponTrailPoint(const C3DVector& point)
{
    std::list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry>::reverse_iterator rit=ProjectileWeaponTrail.rbegin();
    if(rit!=ProjectileWeaponTrail.rend()) {
        CEnginePrimitiveWeaponTrail::CWeaponTrailEntry& trail_entry=*rit;
        std::pair<C3DVector,C3DVector> desired_element=GetProjectileTrailElementFromPoint(point);
        trail_entry.Top=desired_element.first;
        trail_entry.Bottom=desired_element.second;
    }
}