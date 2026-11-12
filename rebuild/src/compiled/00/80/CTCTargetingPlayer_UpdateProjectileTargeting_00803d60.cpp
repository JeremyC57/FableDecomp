// CTCTargetingPlayer::UpdateProjectileTargeting() -- projectile aiming along the game camera: build the camera's look direction
// (look-at minus eye, accurately normalised, scaled by MaxTargetingDistance), gather the things in the rough box around the eye that
// are inside the camera FOV, are not the parent, are not nearer the camera (2D) than the parent, were not shot by the parent and are
// viable aiming targets; aim TargetedPos at the end of the look ray, cast a line of sight from the eye along it, and when the ray is
// blocked at a point inside the FOV, aim at that point and (if the hit thing is alive, targetable and new) make it the projectile
// target.
// Retail: Fable.exe 0x00803D60, 699 bytes, thiscall, plain ret (no SEH frame; the std::vector local is freed inline).
// Leaf: ?UpdateProjectileTargeting@CTCTargetingPlayer@@IAEXXZ
// Flags: /d2inlT68
// Callees: CTCBase::GetWorld 0x00686A70; CAIGameCameraBase::GetCameraManager (vslot +0x24); CThing::GetPos 0x004C73D0;
//   C3DVector::AccurateNormalise 0x00A14440; CCamera::GetFOV 0x00A0BEB0; CThingFilter_IsInFOV ctor 0x006A7150 / operator() 0x006A7270;
//   _ftol2 0x00BFEA70; CTCBase::PeekThingSearchTools 0x00686D90; CThingSearchTools::GetAllThingsInRoughArea<const CThing,...> 0x0080A550;
//   NCameraModes::CTools::IsLineOfSight 0x00877840; CBaseIntelligentPointer::GetPItem 0x00A01B50; CIntelligentPointer<CThing> ctor
//   0x004A96F0 / operator= 0x004A9760; ~CBaseIntelligentPointer 0x00A01C10; free 0x00BFEA14.
// Shape: same TU and prelude as the landed sibling UpdateTargetList 0x00804020. The 2D range is the magnitude of the X/Y of a
// 3D eye->parent offset (Z dead), through C2DVector's Y-first squared magnitude (FABLE_RETAIL_C2DVECTOR_SQUAREDMAGNITUDE_YX: retail
// squares Y first) and GFFastSqrt. The new projectile target goes through a named CIntelligentPointer (retail re-takes its address
// for operator=). Overlay opt-ins: CGAMECAMERAMANAGER_GETCAMERA, NCAMERAMODES_CBASE_LOOKATPHASE_INLINE, CTCTARGETINGPLAYER_UPDATE_PROJECTILE.
#define FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
#define FABLE_RETAIL_CBASEOBJECT_ORIGINAL_ACCESS
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SUBTRACT_ASSIGN
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTCENEMY_QUERIES
#define FABLE_RETAIL_EXPRESSION_FRAME
#define FABLE_RETAIL_CTHING_PEEKPLAYER
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCSTOCKITEM_TARGETING
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_RAW_NOT_EQUAL
#define FABLE_RETAIL_CINTELLIGENTPOINTER_EQUAL_RAW
#define FABLE_RETAIL_UPDATE_NEAREST_INTERESTING
#define FABLE_RETAIL_CTCTARGETINGPLAYER
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_LASTPOSITION
#define FABLE_RETAIL_CTCTARGETED
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_CGAMECAMERAMANAGER_LOOKAROUND
#define FABLE_RETAIL_CGAMECAMERAMANAGER_GETCAMERA
#define FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_PROJECTILE
#define FABLE_RETAIL_CCAMERA_FOV
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_FROM3D
#define FABLE_RETAIL_C2DVECTOR_SQUAREDMAGNITUDE_YX
#define FABLE_RETAIL_NCAMERAMODES_CBASE_CYLINDER
#define FABLE_RETAIL_NCAMERAMODES_CBASE_LOOKATPHASE_INLINE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_MAP_VALIDATE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_FILTER_BIND3_FUNCTION
#define FABLE_RETAIL_STLP_FOR_EACH_BODY
#define FABLE_RETAIL_STLP_FIND_RANDOM_DISPATCH
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_STLP_VECTOR_C
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FOCAL
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_STATIC_FOCAL
#include "engine/EPlayerMode.h"
#include "engine/CThing.h"
#include "engine/ThingAlive.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCTargetingPlayer.h"
#include "engine/CTargetingDef.h"
#include "engine/CPlayer.h"
#include "engine/CGamePlayerInterface.h"
#include "engine/CWorld.h"
#include "engine/CCamera.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CGameCameraManager.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCTargeted.h"
#include "engine/CTCCarrying.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CTCMapwho.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingFilter_IsNot.h"
#include "engine/CThingFilter_EnemyCreature.h"
#include "engine/CTCEnemy.h"
#include "engine/C2DBoxI.h"
#include "engine/CTCAIScratchpad.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CTCStockItem.h"
#include "engine/TargetingPlayerSupport.h"
#include "engine/NCameraModes_CBase.h"
#include "engine/CThingFilter_IsInFOV.h"
#include "engine/CThingFilter_In2DRange.h"
#include "engine/C2DVector.h"
#include "stlport/targeting_algorithms.h"
extern const float DELTA_NOTIONAL_ZERO;
#include "engine/CMainGameComponent.h"



namespace {
// Anonymous-namespace filters of this source file (Ego_r.pdb ?A0x58ed6966; ctors FableWin 0x024CFDE0 / 0x024CFE00, inline in
// retail; operator() Ego_r 0x00802AB0 / 0x00801590, called only from the GetAllThingsInRoughArea instance 0x0080A550).
class CThingFilter_ShotFiredByThing:public std::unary_function<CThing,bool>{
public:
 CThingFilter_ShotFiredByThing(const CThing& thing):Thing((CThing*)&thing){}
 bool operator()(const CThing&)const;
private:
 CThing* Thing;
};
class CThingFilter_ViableProjectileAimingTarget:public std::unary_function<CThing,bool>{
public:
 CThingFilter_ViableProjectileAimingTarget(){}
 bool operator()(const CThing&)const;
};
}

void CTCTargetingPlayer::UpdateProjectileTargeting()
{
    CGameCameraManager* pcam_man;
    GetWorld().GetGameCamera()->GetCameraManager(&pcam_man);
    const C3DVector& pos=GetParentThing().GetPos();
    const C3DVector& eye=pcam_man->GetCamera().PeekWorldPos();
    C3DVector dir=pcam_man->GetCameraMode()->GetLookAtPhase().Position-eye;
    dir.AccurateNormalise();
    dir*=MaxTargetingDistance;
    C3DVector offset=pos-eye;
    const CThing* phit_thing=0;
    std::vector<const CThing*> things;
    float fov=pcam_man->GetCamera().GetFOV();
    CThingFilter_IsInFOV fov_filter(eye,dir,fov,0.0f,false);
    PeekThingSearchTools().GetAllThingsInRoughArea<const CThing>(C2DBoxI(eye.ToC2DCoordI(),(long)MaxTargetingDistance),
        GFFilter_Bind5(fov_filter,CThingFilter_IsNot(GetParentThing()),std::not1(CThingFilter_In2DRange(eye,GFFastSqrt(C2DVector(offset).GetSquaredMagnitude()))),
            std::not1(CThingFilter_ShotFiredByThing(GetParentThing())),CThingFilter_ViableProjectileAimingTarget()),things);
    TargetedPos=dir+eye;
    C3DVector int_pos,normal_vec;
    float cam_rad;
    if(!pcam_man->GetCameraTools().IsLineOfSight(things,eye,dir+eye,int_pos,normal_vec,cam_rad,&phit_thing,NCameraModes::CTools::TEST_ALL,false)){
        if(fov_filter(int_pos)){
            if(IS_THING_ALIVE(phit_thing) && PProjectileTargetedThing!=phit_thing && phit_thing->HasTC(TCI_TARGETED))
            {
                CIntelligentPointer<CThing> ptarget((CThing*)phit_thing);
                PProjectileTargetedThing=ptarget;
            }
            TargetedPos=int_pos;
        }
    }
}