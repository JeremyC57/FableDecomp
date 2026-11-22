// 00874790 NCameraModes::CPCMouseLook::OnInit
// Leaf: ?OnInit@CPCMouseLook@NCameraModes@@UAEXXZ
// Flags: none
#define FABLE_RETAIL_PCMOUSELOOK_GETMODE_PASS26
#define FABLE_RETAIL_CGAMECAMERAMANAGER
#define FABLE_RETAIL_NCAMERAMODES_CPHYSICSLEAF_CYLINDER
#define FABLE_RETAIL_C3DVECTOR_CROSS_COMPONENTS_GROUPED
#define FABLE_RETAIL_PCMOUSELOOK_VIRTUAL_ACCESS_PASS26

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


#include "engine/CGameCameraManager.h"
#include "engine/NCameraModes_CPhysicsLeaf.h"
namespace NCameraModes {
void CPCMouseLook::OnInit()
{
    StateCurrent.ViewPosOffset = C3DVector(0,0,0);
    CPCMouseLook* pold_mode = 0;
    if (!GetCameraManager().GetCameraMode()) {
        C3DVector facing = GetObservedThing()->GetPhysicsTC()->GetFacingVector();
        AxisZ = C3DVector(0.0f,0.0f,1.0f);
        AxisX = facing * -1.0f;
        AxisY = AxisX.Cross(AxisZ);
        StateCurrent.AngleXY = 0.0f;
        StateCurrent.AngleUpDown = 0.125f;
        StateCurrent.DollyLength = GetCameraModeDef()->DollyLength;
    } else if (GetCameraManager().GetCameraMode(&pold_mode)) {
        AxisZ = pold_mode->AxisZ;
        AxisX = pold_mode->AxisX;
        AxisY = pold_mode->AxisY;
        StateCurrent.AngleXY = pold_mode->StateCurrent.AngleXY;
        StateCurrent.AngleUpDown = pold_mode->StateCurrent.AngleUpDown;
        StateCurrent.DollyLength = pold_mode->StateCurrent.DollyLength;
    } else {
        C3DVector facing = GetObservedThing()->GetPhysicsTC()->GetFacingVector();
        AxisZ = C3DVector(0.0f,0.0f,1.0f);
        AxisX = facing * -1.0f;
        AxisY = AxisX.Cross(AxisZ);
        StateCurrent.AngleXY = 0.0f;
        StateCurrent.AngleUpDown = 0.125f;
        StateCurrent.DollyLength = GetCameraModeDef()->DollyLength;
    }
    AddSubComponent(CCountedPointer<CSubComponentBase>(
        new CPhysicsLeaf<CPhysicsTargetModelGoTo,CPhysicsForceModelRigid,CPhysicsCoordRigid>(*this,GetCameraManager())),false);
}
}