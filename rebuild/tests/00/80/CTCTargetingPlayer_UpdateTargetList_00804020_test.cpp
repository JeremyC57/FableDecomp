// Entire current authored body and local scoring helpers; controlled callee stubs.
#include <vector>
#include <algorithm>
#include <functional>
#include <cassert>
#include <cstdio>
#include <cmath>
#ifdef _MSC_VER
#define FABLE_FASTCALL __fastcall
#else
#define FABLE_FASTCALL
#endif
enum EPlayerMode{PLAYER_MODE_NULL=0,PLAYER_MODE_LIGHTNING=40,PLAYER_MODE_PC_PROJECTILE_WEAPON_THIRD_PERSON_AIMING=46};
enum ETCInterfaceType{TCI_WILL_LIGHTNING=148};
class C3DVector{ public:float X,Y,Z;C3DVector(){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){} C3DVector operator+(const C3DVector&p)const{return C3DVector(X+p.X,Y+p.Y,Z+p.Z);}C3DVector operator-(const C3DVector&p)const{return C3DVector(X-p.X,Y-p.Y,Z-p.Z);}C3DVector operator*(float n)const{return C3DVector(X*n,Y*n,Z*n);}float GetSquaredMagnitude()const{return X*X+Y*Y+Z*Z;}class C2DCoordI ToC2DCoordI()const;};
class C2DCoordI{ public:long X,Y;C2DCoordI(long x,long y):X(x),Y(y){}};
C2DCoordI C3DVector::ToC2DCoordI()const{return C2DCoordI((long)X,(long)Y);}
class C2DBoxI{ public:long TLX,TLY,BRX,BRY;C2DBoxI(const C2DCoordI&p,long n):TLX(p.X-n),TLY(p.Y-n),BRX(p.X+n),BRY(p.Y+n){}};
class CGamePlayerInterface{ public:bool toggle;bool IsToggleZTarget()const{return toggle;}};
class CPlayer{ public:EPlayerMode mode;bool z,rt,pc,first;CGamePlayerInterface input;EPlayerMode GetCurrentMode()const{return mode;}bool IsPCProjectileWeaponThirdPersonAimingMode()const{return pc;}bool IsFirstPersonTargeting()const{return first;}bool IsZTargeting()const{return z;}bool IsRightTriggerTargetLocked()const{return rt;}const CGamePlayerInterface* PeekPlayerInterface()const{return &input;}};
static CPlayer fixture_player;
class CTCTargeted;class CThing;
class CTCCarrying{ public:CThing* item;CThing* GetThingInPrimarySlot()const{return item;}};
class CTCPhysicsBase{ public:C3DVector pos,old,facing;C3DVector GetFacingVector()const{return facing;}const C3DVector& GetPosition()const{return pos;}const C3DVector& GetLastPosition()const{return old;}};
struct CGraphic{C3DVector focal;C3DVector GetStaticCameraFocalPos()const{return focal;}};
class CBaseObject{ public:};
class CThing:public CBaseObject{ public:int id;bool dead,enemy,behind,far,stealable,acceptBehind,lightning;int priority;C3DVector pos;CTCTargeted* targeted;CTCCarrying* carrying;CTCPhysicsBase phys;CGraphic graphic;mutable int targetQueries;CThing():id(0),dead(false),enemy(false),behind(false),far(false),stealable(false),acceptBehind(false),lightning(false),priority(0),pos(0,0,0),targeted(0),carrying(0),targetQueries(0){phys.pos=phys.old=C3DVector(0,0,0);phys.facing=C3DVector(0,1,0);graphic.focal=C3DVector(0,0,1);}bool IsDead()const{return dead;}const C3DVector& GetPos()const{return pos;}CTCPhysicsBase* GetPhysicsTC(){return &phys;}CGraphic* GetTCGraphicAppearance(){return &graphic;}CPlayer& GetPlayer(){return fixture_player;}const CPlayer& PeekPlayer()const{return fixture_player;}bool HasTC(ETCInterfaceType)const{return lightning;}bool PeekTC(const CTCCarrying**p)const{*p=carrying;return carrying!=0;}bool GetTC(CTCTargeted**p){++targetQueries;*p=targeted;return targeted!=0;}bool GetTC(const CTCTargeted**p){++targetQueries;*p=targeted;return targeted!=0;}};
class CThingPlayerCreature:public CThing{ public:};
template<class T>bool IS_THING_ALIVE(const T& t){if(t!=0){if(!t->IsDead())return true;}return false;}
template<class T>class CIntelligentPointer{ public:T* p;CIntelligentPointer():p(0){}CIntelligentPointer(T*v):p(v){}T* Get()const{return p;}T* operator->()const{return p;}CIntelligentPointer& operator=(T*v){p=v;return *this;}bool operator==(const T*v)const{return p==v;}};
template<class T> bool operator!=(const CIntelligentPointer<T>& p,const CBaseObject* q){return p.Get()!=q;}
namespace NPlayerTargeting{class CTargetPoint{ public:CIntelligentPointer<CThing>PThing;long Index;C3DVector TargetingPosition;long FrameRejected;CTargetPoint():Index(-1),TargetingPosition(0,0,0),FrameRejected(-1){}bool operator==(const CTargetPoint& o)const{return PThing.Get()==o.PThing.Get()&&Index==o.Index;}};}
class CTCTargeted{ public:CThing* owner;long n;bool noLOS;int pointCalls;CTCTargeted():owner(0),n(0),noLOS(false),pointCalls(0){}long GetNumTargetPoints()const{return n;}C3DVector GetTargetPoint(long i,float fraction){assert(fraction==1.0f);++pointCalls;return C3DVector((float)owner->id,(float)i,0);}bool IsValidTargetWithoutLOS()const{return noLOS;}};
class CTargetingDef{ public:bool TargetingBasePosFromCamera,TargetingPCFacingDirFromCamera,RejectTargetsBehindIfRunning,AcceptOnlyEnemiesBehind;float TargetArc,TargetArcPCThirdPersonAimingMode,EnemyBehindTargetingRange,FirstPersonTargetingAngleFalloff,FirstPersonTargetingDistanceFalloff,ThirdPersonTargetingAngleFalloff,ThirdPersonTargetingDistanceFalloff,ZLockTargetSelectAngleFalloff,ZLockTargetSelectDistanceFalloff,FirstPersonTargetingOverrideShortRangeFactor,FirstPersonTargetingOverrideShortRangeFalloff,ThirdPersonTargetingOverrideShortRangeFactor,ThirdPersonTargetingOverrideShortRangeFalloff,FirstPersonTargetingEnemyPreferenceFactor,ThirdPersonTargetingEnemyPreferenceFactor;};
static std::vector<CThing*> fixture_candidates;
static C3DVector observedBase,observedFacing;static const CThing* observedCarry;static bool observedBuilding;static float observedFOV;static C2DBoxI observedArea(C2DCoordI(0,0),0);
class CThingSearchTools{ public:template<class T,class F>void GetAllThingsInRoughArea(const C2DBoxI&a,const F&,std::vector<CThing*>&out){observedArea=a;out=fixture_candidates;}};
class CCamera{ public:C3DVector pos,dir;const C3DVector& PeekWorldPos()const{return pos;}const C3DVector& PeekViewDirection()const{return dir;}};
static CThing* fixtureHit;static unsigned blockedMask;static int losCalls;static bool losSawOffset;
namespace NCameraModes{class CTools{ public:enum ELOSTest{TEST_ALL_BUT_CREATURES=7};bool IsLineOfSight(const C3DVector&,const C3DVector&to,C3DVector&,C3DVector&,float&,const CThing**hit,ELOSTest test,bool b){assert((int)test==7&&b);++losCalls;losSawOffset=to.Z==0.5f;*hit=fixtureHit;return (blockedMask&(1U<<(int)to.X))==0;}};}
class CGameCameraManager{ public:NCameraModes::CTools tools;NCameraModes::CTools& GetCameraTools(){return tools;}};
struct CGameCamera{CCamera cam;CGameCameraManager manager;CCamera GetRenderCamera(float x){assert(x==1);return cam;}bool GetCameraManager(CGameCameraManager**p){*p=&manager;return true;}};
class CWorld{ public:CGameCamera camera;static long frame;CGameCamera* GetGameCamera(){return &camera;}static long GetFrame(){return frame;}};long CWorld::frame=0;
class CMainGameComponent{ public:static long GetConstantFPS(){return 30;}};
class CTCAIScratchpad{ public:static float GetGlobalTargetingDistanceOffset(){return 2;}};
namespace NGlobalConsole{bool GTargetingTargetPreferenceIn3D=false;}
static const float DELTA_NOTIONAL_ZERO=0.0001f;float GFSqr(float x){return x*x;}
template<class A,class B,class C,class D,class E>class CFilter_Bind5{ public:CFilter_Bind5(const A&,const B&,const C&,const D&,const E&) {}};
template<class A,class B,class C,class D,class E>CFilter_Bind5<A,B,C,D,E> GFFilter_Bind5(const A&a,const B&b,const C&c,const D&d,const E&e){return CFilter_Bind5<A,B,C,D,E>(a,b,c,d,e);}
class CThingFilter_IsNot{ public:CThingFilter_IsNot(const CThing&) {}};
class CThingFilter_IsTargetable{ public:CThingFilter_IsTargetable(bool,bool) {}};
struct CThingFilter_IsInTargetingRangeAndFOV{CThingFilter_IsInTargetingRangeAndFOV(const C3DVector&a,const C3DVector&b,const CThing&,const CTargetingDef&,const CThing*c,float f,bool three){observedBase=a;observedFacing=b;observedCarry=c;observedFOV=f;assert(!three);}};
class CThingFilter_IsInSameVolumeContextAsThing{ public:CThingFilter_IsInSameVolumeContextAsThing(const CThing&,bool b){observedBuilding=b;}};
struct CThingFilter_IsValidTargetForPlayerCreature{CThingFilter_IsValidTargetForPlayerCreature(const CThingPlayerCreature&,const CThing*) {}};
template<class A,class B>class CFilter_Bind2:public std::unary_function<NPlayerTargeting::CTargetPoint,bool>{ public:A a;B b;CFilter_Bind2(const A&x,const B&y):a(x),b(y){}bool operator()(const NPlayerTargeting::CTargetPoint&p)const{return a(p)&&b(p);}};
template<class A,class B>CFilter_Bind2<A,B>GFFilter_Bind2(const A&a,const B&b){return CFilter_Bind2<A,B>(a,b);}
template<class A,class B,class C>class CFilter_Bind3:public std::unary_function<NPlayerTargeting::CTargetPoint,bool>{ public:A a;B b;C c;CFilter_Bind3(const A&x,const B&y,const C&z):a(x),b(y),c(z){}bool operator()(const NPlayerTargeting::CTargetPoint&p)const{return a(p)&&b(p)&&c(p);}};
template<class A,class B,class C>CFilter_Bind3<A,B,C>GFFilter_Bind3(const A&a,const B&b,const C&c){return CFilter_Bind3<A,B,C>(a,b,c);}
class CThingFilter_IsInFOV:public std::unary_function<NPlayerTargeting::CTargetPoint,bool>{ public:CThingFilter_IsInFOV(const C3DVector&,const C3DVector&,float,float,bool){}bool operator()(const NPlayerTargeting::CTargetPoint&p)const{return p.PThing->behind;}};
struct CTargetPointFilter_FurtherFromPosThan:std::unary_function<NPlayerTargeting::CTargetPoint,bool>{CTargetPointFilter_FurtherFromPosThan(const C3DVector&,float){}bool operator()(const NPlayerTargeting::CTargetPoint&p)const{return p.PThing->far;}};
struct CTargetPointFilter_IsLockedTarget:std::unary_function<NPlayerTargeting::CTargetPoint,bool>{const CThing*t;CTargetPointFilter_IsLockedTarget(const CThing*p):t(p){}bool operator()(const NPlayerTargeting::CTargetPoint&p)const{return p.PThing.Get()==t;}};
struct CTargetPointFilter_IsStealableStockItem:std::unary_function<NPlayerTargeting::CTargetPoint,bool>{bool operator()(const NPlayerTargeting::CTargetPoint&p)const{return p.PThing->stealable;}};
struct CThingFilter_IsTargetableBehind:std::unary_function<NPlayerTargeting::CTargetPoint,bool>{CThingFilter_IsTargetableBehind(const CThing&,const CThing*){}bool operator()(const NPlayerTargeting::CTargetPoint&p)const{return p.PThing->acceptBehind;}};
struct CTargetPointIsNotEnemyOfThing:std::unary_function<NPlayerTargeting::CTargetPoint,bool>{CTargetPointIsNotEnemyOfThing(const CThing&){}bool operator()(const NPlayerTargeting::CTargetPoint&p)const{return !p.PThing->enemy;}};
class CTCEnemy{ public:static bool AreThingsEnemies(const CThing&,const CThing&t){return t.enemy;}};
static float scoreArgs[5];static C3DVector scoreLine;
class CThingCompare_BestForTargeting2{ public:CThingCompare_BestForTargeting2(const CThing&,const C3DVector&,const C3DVector&line,float e,float a,float d,float f,float o){scoreArgs[0]=e;scoreArgs[1]=a;scoreArgs[2]=d;scoreArgs[3]=f;scoreArgs[4]=o;scoreLine=line;}void SetAsTargetCompareInXYPlane(){}bool operator()(const NPlayerTargeting::CTargetPoint&a,const NPlayerTargeting::CTargetPoint&b)const{if(a.PThing->priority!=b.PThing->priority)return a.PThing->priority<b.PThing->priority;return a.Index<b.Index;}};
struct CGetBestTargetInDirection{float direction;const NPlayerTargeting::CTargetPoint*best;CGetBestTargetInDirection(const C3DVector&,const C3DVector&,float d):direction(d),best(0){}void operator()(const NPlayerTargeting::CTargetPoint&p){if(!best||(direction>0?p.TargetingPosition.X>best->TargetingPosition.X:p.TargetingPosition.X<best->TargetingPosition.X))best=&p;}const NPlayerTargeting::CTargetPoint* GetPBestTargetPoint()const{return best;}};
class CTCTargetingPlayer{ public:static bool PlayerTargetingLineOfSightTest,PlayerTargetingBuildingVolumeTest;enum ETargetNextThingMethod{TARGET_NEXT_NULL=0,TARGET_NEXT_NEXT=1,TARGET_NEXT_LEFT=2,TARGET_NEXT_RIGHT=3};typedef std::vector<NPlayerTargeting::CTargetPoint>TTargetPointList;CThingPlayerCreature* parent;CWorld world;CThingSearchTools search;CTargetingDef def;long ZLockTargetSelectionTimer;bool TargetNextThing;ETargetNextThingMethod TargetNextThingMethod;long FrameSetAsTargetNextThing;float MaxTargetingDistance;NPlayerTargeting::CTargetPoint TargetedPoint;TTargetPointList Targets,RejectedTargetPoints;CIntelligentPointer<CThing>PTargetLockedThing,PTargetOnlyThing;bool LockedOntoEnemy;CThing* published;int publishedCalls;CThing& GetParentThing(){return *parent;}const CThing& GetParentThing()const{return *parent;}CWorld& GetWorld(){return world;}const CTargetingDef& GetTargetingDef()const{return def;}CThingSearchTools& GetThingSearchTools(){return search;}CThing* GetPTargetLockedThing()const{return PTargetLockedThing.Get();}CThing* GetPTargetedThing()const{return published;}void SetPTargetedThing(CThing*p){published=p;++publishedCalls;}float GetFOVToBeInToBeTargetable()const;void UpdateTargetList();};

template<class F,class T> T* FABLE_FASTCALL GFDebugDynamicCast(F* p,T*){return static_cast<T*>(p); }

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

namespace FOVActualFixture {
struct C3DVector {
 float X,Y,Z;
 C3DVector(){}
 C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
 float GetSquaredMagnitude()const{return X*X+Y*Y+Z*Z;}
 const C3DVector operator*(float n)const{return C3DVector(X*n,Y*n,Z*n);}
 const C3DVector& operator/=(float n){float reciprocal=1.0f/n;X*=reciprocal;Y*=reciprocal;Z*=reciprocal;return *this;}
 const C3DVector& operator-=(const C3DVector& p){X-=p.X;Y-=p.Y;Z-=p.Z;return *this;}
};
const float DELTA_NOTIONAL_ZERO=0.0001f;
class CThingFilter_IsInFOV {
public:
 CThingFilter_IsInFOV(const C3DVector&,const C3DVector&,float,float,bool);
 C3DVector FromPos,Direction;
 float CosFOV,FOV;
 bool AngleTest3D;
};
#ifdef _MSC_VER
#pragma intrinsic(cos,tan,sqrt)
#endif
// Original donor01d9e180/01f93a50 use the widened single-precision full turn.
inline double FABLE_FASTCALL GFCos(float n){return cos((double)n*6.28318548f);}
inline double FABLE_FASTCALL GFTan(float n){return tan((double)n*6.28318548f);}
inline float FABLE_FASTCALL GFRoot(float n){return (float)sqrt((double)n);}
CThingFilter_IsInFOV::CThingFilter_IsInFOV(const C3DVector& from_pos,const C3DVector& facing_vector,float fov,float test_point_rad,bool angle_test_3d)
 :FromPos(from_pos),AngleTest3D(angle_test_3d)
{
 FOV=fov;
 float half_angle=fov*0.5f;
 CosFOV=(float)GFCos(half_angle);
 float displacement=0.0f;
 if(half_angle<0.25f)displacement=(float)(test_point_rad/GFTan(half_angle));
 Direction=facing_vector;
 if(!AngleTest3D)Direction.Z=0.0f;
 float sqr_mag=Direction.GetSquaredMagnitude();
 if(sqr_mag>DELTA_NOTIONAL_ZERO)Direction/=GFRoot(sqr_mag);
 FromPos-=Direction*displacement;
}
static bool near(float a,float b){return std::fabs(a-b)<0.00001f;}
static void check(){
 C3DVector from(2,3,4),dir(0,4,3),zero(0,0,0);
 CThingFilter_IsInFOV a(from,dir,0.25f,0.0f,false);
 assert(near(a.Direction.X,0)&&near(a.Direction.Y,1)&&near(a.Direction.Z,0));
 assert(near(a.CosFOV,0.70710678f)&&a.FOV==0.25f&&!a.AngleTest3D);
 assert(near(a.FromPos.X,2)&&near(a.FromPos.Y,3)&&near(a.FromPos.Z,4));
 CThingFilter_IsInFOV b(from,dir,0.25f,0.0f,true);
 assert(near(b.Direction.Y,0.8f)&&near(b.Direction.Z,0.6f)&&b.AngleTest3D);
 CThingFilter_IsInFOV c(from,dir,0.25f,2.0f,false);
 assert(near(c.FromPos.X,2)&&near(c.FromPos.Y,1)&&near(c.FromPos.Z,4));
 CThingFilter_IsInFOV d(from,dir,0.75f,2.0f,false);
 assert(near(d.CosFOV,-0.70710678f)&&near(d.FromPos.Y,3));
 CThingFilter_IsInFOV e(from,zero,0.25f,2.0f,false);
 assert(e.Direction.GetSquaredMagnitude()==0&&near(e.FromPos.Y,3));
 CThingFilter_IsInFOV f(from,dir,1.0f,2.0f,false);
 assert(near(f.CosFOV,-1)&&near(f.FromPos.Y,3));
}
}

static int scenarios;
static void run(int mode){
 CThingPlayerCreature hero;CThing a,b,c,obstacle;CTCTargeted ta,tb,tc;CTCCarrying carry;
 a.id=1;b.id=2;c.id=3;a.priority=1;b.priority=2;c.priority=3;a.targeted=&ta;b.targeted=&tb;c.targeted=&tc;ta.owner=&a;tb.owner=&b;tc.owner=&c;
 fixture_player=CPlayer();fixture_player.mode=PLAYER_MODE_NULL;fixture_player.input.toggle=false;
 fixture_candidates.clear();fixture_candidates.push_back(&a);fixture_candidates.push_back(&b);fixture_candidates.push_back(&c);
 blockedMask=0;fixtureHit=&obstacle;losCalls=0;losSawOffset=false;NGlobalConsole::GTargetingTargetPreferenceIn3D=false;observedCarry=0;
 CTCTargetingPlayer x;x.parent=&hero;x.ZLockTargetSelectionTimer=0;x.TargetNextThing=false;x.TargetNextThingMethod=CTCTargetingPlayer::TARGET_NEXT_NULL;x.FrameSetAsTargetNextThing=0;x.MaxTargetingDistance=10;x.LockedOntoEnemy=false;x.published=0;x.publishedCalls=0;x.world.camera.cam.pos=C3DVector(100,200,3);x.world.camera.cam.dir=C3DVector(1,0,0);x.PlayerTargetingLineOfSightTest=true;x.PlayerTargetingBuildingVolumeTest=true;CWorld::frame=1;
 x.def=CTargetingDef();x.def.TargetArc=180;x.def.TargetArcPCThirdPersonAimingMode=90;x.def.EnemyBehindTargetingRange=2;x.def.FirstPersonTargetingAngleFalloff=11;x.def.FirstPersonTargetingDistanceFalloff=12;x.def.ThirdPersonTargetingAngleFalloff=21;x.def.ThirdPersonTargetingDistanceFalloff=22;x.def.ZLockTargetSelectAngleFalloff=31;x.def.ZLockTargetSelectDistanceFalloff=32;x.def.FirstPersonTargetingOverrideShortRangeFactor=13;x.def.FirstPersonTargetingOverrideShortRangeFalloff=14;x.def.ThirdPersonTargetingOverrideShortRangeFactor=23;x.def.ThirdPersonTargetingOverrideShortRangeFalloff=24;x.def.FirstPersonTargetingEnemyPreferenceFactor=15;x.def.ThirdPersonTargetingEnemyPreferenceFactor=25;
 int expected=1;int expectedLOS=1;
 switch(mode){
 case 0:fixture_candidates.clear();expected=0;expectedLOS=0;break;
 case 1:break;
 case 2:blockedMask=2;expected=2;expectedLOS=2;break;
 case 3:blockedMask=14;expected=0;expectedLOS=3;break;
 case 4:blockedMask=2;fixtureHit=&a;break;
 case 5:x.PlayerTargetingLineOfSightTest=false;expectedLOS=0;break;
 case 6:ta.noLOS=true;expectedLOS=0;break;
 case 7:ta.n=2;break;
 case 8:x.def.TargetArc=360;a.behind=true;a.far=true;expected=2;break;
 case 9:x.def.TargetArc=360;a.behind=true;a.stealable=true;expected=2;break;
 case 10:x.def.TargetArc=360;a.behind=true;x.def.RejectTargetsBehindIfRunning=true;hero.phys.pos.X=1;expected=2;break;
 case 11:x.def.TargetArc=360;a.behind=true;x.def.AcceptOnlyEnemiesBehind=true;expected=2;break;
 case 12:x.def.TargetArc=360;a.behind=true;x.PTargetLockedThing=&a;break;
 case 13:fixture_player.first=true;break;
 case 14:fixture_player.pc=true;break;
 case 15:x.ZLockTargetSelectionTimer=9;break;
 case 16:x.ZLockTargetSelectionTimer=8;break;
 case 17:x.def.TargetingBasePosFromCamera=true;break;
 case 18:x.def.TargetingPCFacingDirFromCamera=true;break;
 case 19:fixture_player.mode=PLAYER_MODE_PC_PROJECTILE_WEAPON_THIRD_PERSON_AIMING;break;
 case 20:carry.item=&c;hero.carrying=&carry;break;
 case 21:x.PlayerTargetingBuildingVolumeTest=false;break;
 case 22:NGlobalConsole::GTargetingTargetPreferenceIn3D=true;fixture_player.pc=true;break;
 case 23:fixture_player.z=true;x.PTargetLockedThing=&c;c.enemy=true;b.enemy=true;expected=2;break;
 case 24:fixture_player.z=true;x.PTargetLockedThing=&c;break;
 case 25:fixture_player.z=true;fixture_player.input.toggle=true;a.enemy=true;break;
 case 26:fixture_player.z=true;fixture_player.input.toggle=true;x.PTargetLockedThing=&c;x.LockedOntoEnemy=true;break;
 case 27:fixture_player.z=true;fixture_player.input.toggle=true;a.enemy=true;fixture_player.mode=PLAYER_MODE_LIGHTNING;break;
 default:{
   NPlayerTargeting::CTargetPoint pa,pb,pc;pa.PThing=&a;pa.Index=-1;pa.TargetingPosition=C3DVector(1,-1,0);pb=pa;pb.PThing=&b;pb.TargetingPosition.X=2;pc=pa;pc.PThing=&c;pc.TargetingPosition.X=3;x.Targets.push_back(pa);x.Targets.push_back(pb);x.Targets.push_back(pc);x.TargetedPoint=pa;x.PTargetLockedThing=&a;x.published=&a;x.TargetNextThing=true;
   x.TargetNextThingMethod=mode==28||mode==29?CTCTargetingPlayer::TARGET_NEXT_NEXT:mode==30?CTCTargetingPlayer::TARGET_NEXT_LEFT:mode==31?CTCTargetingPlayer::TARGET_NEXT_RIGHT:CTCTargetingPlayer::TARGET_NEXT_NULL;
   expected=mode==28?2:mode==30?3:mode==31?2:1;
   if(mode==29)CWorld::frame=100;
   if(mode==33){fixture_player.z=true;x.PTargetLockedThing=(CThing*)0;expected=1;}
   break;
 }
 }
 x.UpdateTargetList();
 assert(x.published==(expected==0?(CThing*)0:expected==1?&a:expected==2?&b:&c));assert(!x.TargetNextThing);assert(x.publishedCalls==1);assert(losCalls==expectedLOS);if(expectedLOS)assert(losSawOffset);
 if(expected==0){assert(x.TargetedPoint.Index==-1&&x.TargetedPoint.FrameRejected==-1&&!x.TargetedPoint.PThing.Get());}
 else assert(x.TargetedPoint.PThing.Get()==x.published);
 if(mode==2)assert(x.Targets.size()==2);
 if(mode==3)assert(x.Targets.empty());
 if(mode==5)assert(a.targetQueries==1);
 if(mode==6)assert(a.targetQueries==2);
 if(mode==7)assert(ta.pointCalls==2&&x.TargetedPoint.Index==0);
 if(mode==13)assert(scoreArgs[0]==15&&scoreArgs[1]==11&&scoreArgs[2]==12&&scoreArgs[3]==13&&scoreArgs[4]==14);
 if(mode==14)assert(observedFOV==0.25f);
 if(mode==15)assert(x.PTargetLockedThing.Get()==&a&&scoreArgs[1]==31&&scoreArgs[2]==32&&observedFacing.X==1);
 if(mode==16)assert(!x.PTargetLockedThing.Get());
 if(mode==17)assert(observedBase.X==100&&observedBase.Y==200&&observedArea.TLX==88&&observedArea.BRY==212);
 if(mode==18||mode==19)assert(observedFacing.X==1&&observedFacing.Y==0);
 if(mode==20)assert(observedCarry==&c);
 if(mode==21)assert(observedBuilding);
 if(mode==22)assert(scoreLine.X==1&&scoreLine.Y==0);
 if(mode==25)assert(x.PTargetLockedThing.Get()==&a&&x.LockedOntoEnemy);
 if(mode==26)assert(x.PTargetLockedThing.Get()==&c&&x.LockedOntoEnemy);
 if(mode==27)assert(!x.PTargetLockedThing.Get());
 if(mode>=28)assert(x.PTargetLockedThing.Get()==x.published);
 ++scenarios;
}
bool CTCTargetingPlayer::PlayerTargetingLineOfSightTest=true;
bool CTCTargetingPlayer::PlayerTargetingBuildingVolumeTest=true;
int main(){FOVActualFixture::check();for(int i=0;i<34;++i)run(i);assert(scenarios==34);std::puts("00804020_TEST PASS");return 0;}