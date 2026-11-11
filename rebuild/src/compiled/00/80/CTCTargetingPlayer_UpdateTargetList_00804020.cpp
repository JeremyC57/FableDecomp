// CTCTargetingPlayer::UpdateTargetList() -- rebuild and select the player's targeting candidates.
// Grade: RELOCATION_MATCH -- actual COFF operands checked against their native targets.
// Inline-budget: native first vector copy expands; old/shuffle copies call808470. T68 and scopeddepth0 retain the native split.
// Leaf: ?UpdateTargetList@CTCTargetingPlayer@@IAEXXZ
// Retail: 0x00804020, 5587 bytes, thiscall.
// Flags: /d2inlT68
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
#include "stlport/targeting_algorithms.h"
extern const float DELTA_NOTIONAL_ZERO;
#include "engine/CMainGameComponent.h"

namespace {
float FABLE_FASTCALL GetTargetingAngleFalloffForThing(const CThing& thing,const CTargetingDef& def,bool ztarget_target_select_mode){
 if(ztarget_target_select_mode)return def.ZLockTargetSelectAngleFalloff;
 if(thing.PeekPlayer().IsFirstPersonTargeting())return def.FirstPersonTargetingAngleFalloff;
 return def.ThirdPersonTargetingAngleFalloff;
}
float FABLE_FASTCALL GetTargetingSqrDistanceFalloffForThing(const CThing& thing,const CTargetingDef& def,bool ztarget_target_select_mode){
 if(ztarget_target_select_mode)return def.ZLockTargetSelectDistanceFalloff;
 if(thing.PeekPlayer().IsFirstPersonTargeting())return def.FirstPersonTargetingDistanceFalloff;
 return def.ThirdPersonTargetingDistanceFalloff;
}
float FABLE_FASTCALL GetTargetingShortRangeOverrideFactorForThing(const CThing& thing,const CTargetingDef& def){
 if(thing.PeekPlayer().IsFirstPersonTargeting())return def.FirstPersonTargetingOverrideShortRangeFactor;
 return def.ThirdPersonTargetingOverrideShortRangeFactor;
}
float FABLE_FASTCALL GetTargetingShortRangeOverrideFalloffForThing(const CThing& thing,const CTargetingDef& def){
 if(thing.PeekPlayer().IsFirstPersonTargeting())return def.FirstPersonTargetingOverrideShortRangeFalloff;
 return def.ThirdPersonTargetingOverrideShortRangeFalloff;
}
float FABLE_FASTCALL GetTargetingEnemyPreferenceFactor(const CThing& thing,const CTargetingDef& def){
 if(thing.PeekPlayer().IsFirstPersonTargeting())return def.FirstPersonTargetingEnemyPreferenceFactor;
 return def.ThirdPersonTargetingEnemyPreferenceFactor;
}
}

float CTCTargetingPlayer::GetFOVToBeInToBeTargetable()const
{
 if(GetParentThing().PeekPlayer().IsPCProjectileWeaponThirdPersonAimingMode())return GetTargetingDef().TargetArcPCThirdPersonAimingMode/360.0f;
 return GetTargetingDef().TargetArc/360.0f;
}

void CTCTargetingPlayer::UpdateTargetList()
{
    CThing& parent_thing=GetParentThing();
    CThingPlayerCreature& player_creature=*GFDebugDynamicCast(&parent_thing,(CThingPlayerCreature*)0);
    CCamera current_cam=GetWorld().GetGameCamera()->GetRenderCamera(1.0f);
    const CTargetingDef& targeting_def=GetTargetingDef();
    NPlayerTargeting::CTargetPoint old_target_point=TargetedPoint;
    bool ztarget_target_select_mode=false;
    const long NUM_FRAMES_CONTROL_HELD_FOR_TARGET_SELECTION=8;
    if(ZLockTargetSelectionTimer>NUM_FRAMES_CONTROL_HELD_FOR_TARGET_SELECTION)
        ztarget_target_select_mode=true;
    bool pc_proj_targ=false;
    bool target_3d=false;
    if(GetParentThing().PeekPlayer().GetCurrentMode()==PLAYER_MODE_PC_PROJECTILE_WEAPON_THIRD_PERSON_AIMING)
        pc_proj_targ=true;
    bool use_camera_position=targeting_def.TargetingBasePosFromCamera;
    bool use_camera_facing=targeting_def.TargetingPCFacingDirFromCamera;
    if(ztarget_target_select_mode)use_camera_facing=true;
    if(pc_proj_targ)use_camera_facing=true;
    C3DVector targeting_base_pos=use_camera_position?current_cam.PeekWorldPos():parent_thing.GetPos();
    C3DVector facing_dir=use_camera_facing?current_cam.PeekViewDirection():parent_thing.GetPhysicsTC()->GetFacingVector();
    C2DBoxI area(targeting_base_pos.ToC2DCoordI(),(long)(MaxTargetingDistance+CTCAIScratchpad::GetGlobalTargetingDistanceOffset()));
    float fov=GetFOVToBeInToBeTargetable();
    const CTCCarrying* ptc_carrying=0;
    const CThing* pcarried_thing=parent_thing.PeekTC(&ptc_carrying)?ptc_carrying->GetThingInPrimarySlot():0;
    bool building_override=false;
    if(!PlayerTargetingBuildingVolumeTest)building_override=true;
    std::vector<CThing*> target_list;
    GetThingSearchTools().GetAllThingsInRoughArea<CThing>(area,
        GFFilter_Bind5(CThingFilter_IsNot(parent_thing),
            CThingFilter_IsTargetable(GetParentThing().GetPlayer().IsZTargeting(),GetParentThing().HasTC(TCI_WILL_LIGHTNING)),
            CThingFilter_IsInTargetingRangeAndFOV(targeting_base_pos,facing_dir,parent_thing,targeting_def,pcarried_thing,fov,target_3d),
            CThingFilter_IsInSameVolumeContextAsThing(parent_thing,building_override),
            CThingFilter_IsValidTargetForPlayerCreature(player_creature,PTargetOnlyThing.Get())),target_list);
    TTargetPointList target_points;
    for(std::vector<CThing*>::iterator it=target_list.begin();it!=target_list.end();++it){
        CThing* pthing=*it;
        CTCTargeted* ptc_targeted;
        pthing->GetTC(&ptc_targeted);
        long num_pnts=ptc_targeted->GetNumTargetPoints();
        for(long c1=0;c1<num_pnts;++c1){
            NPlayerTargeting::CTargetPoint tpoint;
            tpoint.Index=c1;
            tpoint.PThing=pthing;
            tpoint.TargetingPosition=ptc_targeted->GetTargetPoint(c1,1.0f);
            target_points.push_back(tpoint);
        }
        if(num_pnts==0){
            NPlayerTargeting::CTargetPoint tpoint;
            tpoint.Index=-1;
            tpoint.PThing=pthing;
            tpoint.TargetingPosition=ptc_targeted->GetTargetPoint(-1,1.0f);
            target_points.push_back(tpoint);
        }
    }
    if(fov>0.5f){
        {
            TTargetPointList::iterator it=std::remove_if(target_points.begin(),target_points.end(),
            GFFilter_Bind3(CThingFilter_IsInFOV(targeting_base_pos,facing_dir*-1.0f,0.5f,0.0f,false),
                CTargetPointFilter_FurtherFromPosThan(targeting_base_pos,targeting_def.EnemyBehindTargetingRange),
                std::not1(CTargetPointFilter_IsLockedTarget(GetPTargetLockedThing()))));
            target_points.erase(it,target_points.end());
        }
        target_points.erase(std::remove_if(target_points.begin(),target_points.end(),
            GFFilter_Bind2(CThingFilter_IsInFOV(targeting_base_pos,facing_dir*-1.0f,0.5f,0.0f,false),CTargetPointFilter_IsStealableStockItem())),target_points.end());
        C3DVector line_guy=parent_thing.GetPhysicsTC()->GetPosition()-parent_thing.GetPhysicsTC()->GetLastPosition();
        float sqr_speed=line_guy.GetSquaredMagnitude();
        float SPEED_THRESHOLD=DELTA_NOTIONAL_ZERO;
        if(targeting_def.RejectTargetsBehindIfRunning && sqr_speed>GFSqr(SPEED_THRESHOLD))
            target_points.erase(std::remove_if(target_points.begin(),target_points.end(),CThingFilter_IsInFOV(targeting_base_pos,facing_dir*-1.0f,0.5f,0.0f,false)),target_points.end());
        if(targeting_def.AcceptOnlyEnemiesBehind){
            CThing* pthing=GetPTargetedThing();
            if(!GetParentThing().PeekPlayer().IsRightTriggerTargetLocked())pthing=0;
            TTargetPointList::iterator it=std::remove_if(target_points.begin(),target_points.end(),GFFilter_Bind2(
                CThingFilter_IsInFOV(targeting_base_pos,facing_dir*-1.0f,0.5f,0.0f,false),std::not1(CThingFilter_IsTargetableBehind(parent_thing,pthing))));
            target_points.erase(it,target_points.end());
        }
    }
    C3DVector preferred_line=facing_dir;
    if(NGlobalConsole::GTargetingTargetPreferenceIn3D &&
        (GetParentThing().GetPlayer().IsPCProjectileWeaponThirdPersonAimingMode()||GetParentThing().GetPlayer().IsFirstPersonTargeting()))
        preferred_line=current_cam.PeekViewDirection();
    CThingCompare_BestForTargeting2 targeting_compare(GetParentThing(),targeting_base_pos,preferred_line,
        GetTargetingEnemyPreferenceFactor(GetParentThing(),targeting_def),
        GetTargetingAngleFalloffForThing(parent_thing,targeting_def,ztarget_target_select_mode),
        GetTargetingSqrDistanceFalloffForThing(parent_thing,targeting_def,ztarget_target_select_mode),
        GetTargetingShortRangeOverrideFactorForThing(parent_thing,targeting_def),
        GetTargetingShortRangeOverrideFalloffForThing(parent_thing,targeting_def));
    targeting_compare.SetAsTargetCompareInXYPlane();
    std::sort(target_points.begin(),target_points.end(),targeting_compare);
    // Native804AD2..4B14 expands this104-byte genuine copy constructor.
    TTargetPointList new_targets=target_points;
    // Native804B25 calls the same genuine copy constructor at808470.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
    TTargetPointList old_targets=Targets;
#ifdef _MSC_VER
#pragma inline_depth()
#endif
    CGameCameraManager* pcam_man;
    GetWorld().GetGameCamera()->GetCameraManager(&pcam_man);
    bool locked_onto_target=false;
    CThing* ptargeted_thing=TargetedPoint.PThing.Get();
    CThing* pzlocked_thing=GetPTargetLockedThing();
    if(TargetNextThing)locked_onto_target=true;
    else if(GetParentThing().GetPlayer().IsZTargeting()){
        locked_onto_target=true;
        if(ztarget_target_select_mode)locked_onto_target=false;
    }else if(GetParentThing().GetPlayer().IsRightTriggerTargetLocked())locked_onto_target=true;
    bool pick_up_locked_target=false;
    if(ztarget_target_select_mode)pick_up_locked_target=true;
    bool looking_for_target=true;
    do{
        long cur_index=-1;
        TTargetPointList final_targets;
        if(!locked_onto_target){
            final_targets=new_targets;
            if(!final_targets.empty())cur_index=0;
        }else{
            final_targets=old_targets;
            if(!false){
            for(TTargetPointList::iterator it=new_targets.begin();it!=new_targets.end();++it){
                NPlayerTargeting::CTargetPoint& target_point=*it;
                TTargetPointList::iterator it_look_up=std::find(final_targets.begin(),final_targets.end(),target_point);
                if(it_look_up==final_targets.end())final_targets.push_back(target_point);
            }
            }
            if(!false){
            for(TTargetPointList::iterator it=final_targets.begin();it!=final_targets.end();){
                NPlayerTargeting::CTargetPoint& target_point=*it;
                TTargetPointList::iterator it_look_up=std::find(new_targets.begin(),new_targets.end(),target_point);
                if(it_look_up==new_targets.end())it=final_targets.erase(it);
                else ++it;
            }
            }
            if(IS_THING_ALIVE(pzlocked_thing) && CTCEnemy::AreThingsEnemies(GetParentThing(),*pzlocked_thing))
                final_targets.erase(std::remove_if(final_targets.begin(),final_targets.end(),CTargetPointIsNotEnemyOfThing(GetParentThing())),final_targets.end());
            if(!final_targets.empty()){
                cur_index=0;
                if(IS_THING_ALIVE(pzlocked_thing) && ptargeted_thing==pzlocked_thing){
                    TTargetPointList::iterator it=std::find(final_targets.begin(),final_targets.end(),TargetedPoint);
                    if(it!=final_targets.end())cur_index=it-final_targets.begin();
                }
                if(TargetNextThing){
                    pick_up_locked_target=true;
                    if(!GetParentThing().GetPlayer().IsZTargeting()||!GetPTargetedThing()||GetPTargetLockedThing()){
                        if(TargetNextThingMethod==TARGET_NEXT_NEXT){
                            bool back_to_start=(CWorld::GetFrame()>FrameSetAsTargetNextThing+CMainGameComponent::GetConstantFPS()*0.75f);
                            if(back_to_start){
                                cur_index=0;
                                std::sort(final_targets.begin(),final_targets.end(),targeting_compare);
                            }else if(cur_index!=-1){
                                long size_of_list=final_targets.size();
                                std::sort(final_targets.begin()+cur_index,final_targets.end(),targeting_compare);
                                ++cur_index;
                                if(cur_index==size_of_list)cur_index=0;
                            }
                        }else if(TargetNextThingMethod!=TARGET_NEXT_NULL){
                            // Native804FAA also calls808470.
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
                            TTargetPointList shuffle_list=final_targets;
#ifdef _MSC_VER
#pragma inline_depth()
#endif
                            if(cur_index!=-1 && cur_index<shuffle_list.size()){
                                TTargetPointList::iterator it=shuffle_list.begin()+cur_index;
                                NPlayerTargeting::CTargetPoint target_point=*it;
                                C3DVector cur_target_pos=target_point.TargetingPosition;
                                float direction=1.0f;
                                if(TargetNextThingMethod!=TARGET_NEXT_LEFT)direction=-1.0f;
                                shuffle_list.erase(it);
                                const NPlayerTargeting::CTargetPoint* pnew_target=std::for_each(shuffle_list.begin(),shuffle_list.end(),CGetBestTargetInDirection(GetParentThing().GetPos(),cur_target_pos,direction)).GetPBestTargetPoint();
                                if(pnew_target){
                                    TTargetPointList::iterator it2=std::find(final_targets.begin(),final_targets.end(),*pnew_target);
                                    cur_index=it2-final_targets.begin();
                                }
                            }
                        }
                    }
                }
            }
        }
        NPlayerTargeting::CTargetPoint* pnew_target_point=0;
        Targets=final_targets;
        if(cur_index>=0)pnew_target_point=&Targets[cur_index];
        if(pnew_target_point){
            const float TARGETING_UPWARD_DISTANCE_OFFSET=0.5f;
            bool use_los_test=PlayerTargetingLineOfSightTest;
            if(use_los_test){
                const CTCTargeted* ptc_targeted;
                if(pnew_target_point->PThing->GetTC(&ptc_targeted) && ptc_targeted->IsValidTargetWithoutLOS())use_los_test=false;
            }
            const CThing* phit_thing;
            C3DVector int_pos,normal_vec;
            float cam_rad;
            if(use_los_test && !pcam_man->GetCameraTools().IsLineOfSight(GetParentThing().GetTCGraphicAppearance()->GetStaticCameraFocalPos(),pnew_target_point->TargetingPosition+C3DVector(0.0f,0.0f,TARGETING_UPWARD_DISTANCE_OFFSET),int_pos,normal_vec,cam_rad,&phit_thing,NCameraModes::CTools::TEST_ALL_BUT_CREATURES,true) && pnew_target_point->PThing!=phit_thing){
                looking_for_target=true;
                new_targets.erase(std::remove(new_targets.begin(),new_targets.end(),*pnew_target_point),new_targets.end());
                old_targets.erase(std::remove(old_targets.begin(),old_targets.end(),*pnew_target_point),old_targets.end());
            }else{
                looking_for_target=false;
                TargetedPoint=*pnew_target_point;
                Targets=final_targets;
                CThing* pthing_to_target=TargetedPoint.PThing.Get();
                bool target_thing_enemy=false;
                if(IS_THING_ALIVE(pthing_to_target) && CTCEnemy::AreThingsEnemies(GetParentThing(),*pthing_to_target))target_thing_enemy=true;
                const CPlayer& player=GetParentThing().GetPlayer();
                if(pick_up_locked_target){
                    PTargetLockedThing=pthing_to_target;
                    LockedOntoEnemy=target_thing_enemy;
                }else{
                    if(player.IsZTargeting() && player.PeekPlayerInterface()->IsToggleZTarget() && player.GetCurrentMode()!=PLAYER_MODE_LIGHTNING && (target_thing_enemy||!LockedOntoEnemy)){
                        PTargetLockedThing=pthing_to_target;
                        LockedOntoEnemy=target_thing_enemy;
                    }
                }
                SetPTargetedThing(pthing_to_target);
            }
        }else{
            TargetedPoint.Index=-1;
            TargetedPoint.PThing=(CThing*)0;
            TargetedPoint.FrameRejected=-1;
            SetPTargetedThing(0);
            looking_for_target=false;
        }
    }while(looking_for_target);
    TargetNextThing=false;
}