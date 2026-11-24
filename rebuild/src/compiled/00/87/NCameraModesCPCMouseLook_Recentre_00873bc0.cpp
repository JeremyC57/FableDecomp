// 00873bc0 NCameraModes::CPCMouseLook::Recentre
// Leaf: ?Recentre@CPCMouseLook@NCameraModes@@UAEXXZ
// Flags: none
#define FABLE_RETAIL_C3DVECTOR_CROSS_COMPONENTS_GROUPED
#define FABLE_RETAIL_PCMOUSELOOK_VIRTUAL_ACCESS_PASS26
#define FABLE_RETAIL_GFANGLES_SIGNED_ROUNDING
#define FABLE_RETAIL_PCMOUSELOOK_PASS24
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_GETTC_FORCEINLINE
#define FABLE_RETAIL_CTHING_PEEKPLAYER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_STATIC_FOCAL
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_DOT_ORDERED_ZYX
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_LASTPOSITION
#define FABLE_RETAIL_FIRE_RANGED_WEAPON
#define FABLE_RETAIL_NCAMERAMODES_CBASE_CYLINDER
#define FABLE_RETAIL_NCAMERAMODES_CBASE_INLINE_ACCESSORS
#define FABLE_RETAIL_CCAMERAMODEDEF_CYLINDER
#define FABLE_RETAIL_CPLAYER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CGAMEPLAYERINTERFACE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETPPLAYERINTERFACE
#define FABLE_RETAIL_BASE_OBJECT
#include "fable_string.h"
#include "engine/CThingGameObject.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCTargetingBase.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/EPlayerMode.h"
#include "engine/CPlayer.h"
#include "engine/EGameAction.h"
#include "engine/CGamePlayerInterface.h"
#include "engine/CMainGameComponent.h"
#include "engine/CUserProfileManager.h"
#include "engine/CCameraModeDef.h"
#include "engine/NCameraModes_CPCMouseLook.h"
#include "math.h"

#include "engine/GFAngles.h"

namespace NCameraModes {
void CPCMouseLook::Recentre()
{
    C3DVector facing = GetObservedThing()->GetPhysicsTC()->GetFacingVector();
    C3DVector old_axis_x(AxisX);
    old_axis_x.Rotate(AxisZ, StateCurrent.AngleXY);
    AxisZ = C3DVector(0,0,1);
    AxisX = facing * -1.0f;
    AxisY = AxisX.Cross(AxisZ);
    float cos_thet = old_axis_x.Dot(AxisX);
    if (cos_thet < -1.0f)
        cos_thet = -1.0f;
    else if (cos_thet > 1.0f)
        cos_thet = 1.0f;
    StateCurrent.AngleXY = GFArcCos(cos_thet);
    if (old_axis_x.Cross(AxisX).Z < 0.0f)
        StateCurrent.AngleXY *= -1.0f;
    const float RECENTRE_SECONDS = GetCameraModeDef()->SecondsToReset;
    StateCurrent.RecentreTimer = (long)(CMainGameComponent::GetConstantFPS() * RECENTRE_SECONDS);
    StateCurrent.DeltaAngleXY = StateCurrent.AngleXY * -1.0f / StateCurrent.RecentreTimer;
    --StateCurrent.RecentreTimer;
}
}