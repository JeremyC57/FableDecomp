// Controlled dependency fixture; the production Update body is pasted unchanged below.
// ABI, helper implementation and relocated operands require separate compiler review.
#include <cstdio>
#include <cstring>
#include <cmath>
#include <cassert>
#include <string>
#include <vector>
// Actual selected scalar helpers; normalized-turn behavior is tested below.
const float DELTA_NOTIONAL_ZERO=0.0001f;
inline float GFRoot(float n){return sqrt(n);}
inline float GFArcTan(double opposite,double adjacent){return (float)atan2(opposite,adjacent)/6.2831855f;}
inline float GET_ROUNDED_ANGLE(float angle) {
 if(angle<0.0f || angle>=1.0f) {
  angle=fmod(angle,1.0f);
  if(angle<0.0f) angle+=1.0f;
 }
 return angle;
}
struct C3DVector {
 float X,Y,Z;C3DVector():X(0),Y(0),Z(0){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
 C3DVector operator+(const C3DVector& b)const{return C3DVector(X+b.X,Y+b.Y,Z+b.Z);}
 C3DVector operator-(const C3DVector& b)const{return C3DVector(X-b.X,Y-b.Y,Z-b.Z);}
 C3DVector operator*(float f)const{return C3DVector(X*f,Y*f,Z*f);}
 bool IsNonZero()const{const float& x=X;const float& y=Y;const float& z=Z;return y*y+x*x+z*z>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO;}
 float GetMagnitude()const{return GFRoot(X*X+Y*Y+Z*Z);}
 C3DVector& Normalise(){float n=GetMagnitude();assert(n>0);X/=n;Y/=n;Z/=n;return *this;}
 float GetAngleXY()const{return GET_ROUNDED_ANGLE(GFArcTan(X,Y));}
};
enum EAIActionReturn {AI_ACTION_NULL,AI_ACTION_TERMINATE_PROCESS,AI_ACTION_FINISHED,AI_ACTION_FAILED,AI_ACTION_IN_PROGRESS};
enum EAIActionRepeat{ACTION_REPEAT_UNTIL_FINISHED=1,ACTION_REPEAT_CALLBACK=2};
enum EMovementType{AI_MOVEMENT_WALKING=3};
namespace NSpeechGainManager {enum EDialogueLayer {LAYER_DEFAULT=0};}
static int Handles,Defs;
struct CaseState {
 int Tick,ReadyAfter,StopQuery,RunnableChecks,StopAction,ActionsPending;
 int Creates,Summons,PowerUps,Terminate,Move,Look,Rush,Melee,Nothing,Near,Far,Leap,Boast,Speech,Phase,Obstructions;
 int DefinitionReads,Frame,FrameStep,Theme,ShotSets,LastShot,LastSpeech,Kill,Resets,Positions,Facing,ShieldOn,ShieldOff,NavOn,NavOff,Solid;
 bool Runnable,ExistingShield,CreateOK,ShieldTC,NavTC,ObstructionOK,Knocked,LOS,Talking,ActionFinished,MoveFailed;
 float Distance,ShotInterval,LastFacing;
 CaseState():Tick(0),ReadyAfter(3),StopQuery(1000),RunnableChecks(0),StopAction(0),ActionsPending(0),Creates(0),Summons(0),PowerUps(0),Terminate(0),Move(0),Look(0),Rush(0),Melee(0),Nothing(0),Near(0),Far(0),Leap(0),Boast(0),Speech(0),Phase(0),Obstructions(0),DefinitionReads(0),Frame(0),FrameStep(100),Theme(0),ShotSets(0),LastShot(0),LastSpeech(0),Kill(0),Resets(0),Positions(0),Facing(0),ShieldOn(0),ShieldOff(0),NavOn(0),NavOff(0),Solid(0),Runnable(true),ExistingShield(false),CreateOK(true),ShieldTC(true),NavTC(true),ObstructionOK(true),Knocked(false),LOS(true),Talking(false),ActionFinished(true),MoveFailed(false),Distance(8),ShotInterval(1),LastFacing(0){}
 void Step(int kind){++Tick;if(StopAction==kind)Runnable=false;assert(Tick<30);}
};
static CaseState* C;
class CThing;class CThingCreatureBase;class CThingObject;
struct CJackOfBladesBattleDef {float IdealShootDistance,RushTriggerDistance,TimeBetweenShots,RushToDistance;};
static CJackOfBladesBattleDef Battle;
template<class T> class CDefPointer {public:T* P;CDefPointer():P(0){++Defs;}~CDefPointer(){--Defs;}T* operator->()const{return P;}};
struct CThingBaseDef {template<class T> bool GetSubDef(CDefPointer<const T>* p)const{p->P=&Battle;return true;}};
template<class T>class CIntelligentPointer {
 T* P;
public:
 CIntelligentPointer():P(0){++Handles;}CIntelligentPointer(T* p):P(p){++Handles;}
 CIntelligentPointer(const CIntelligentPointer& x):P(x.P){++Handles;}
 template<class U>CIntelligentPointer(CIntelligentPointer<U>& x):P(x.Get()){++Handles;}
 ~CIntelligentPointer(){--Handles;}
 CIntelligentPointer& operator=(const CIntelligentPointer& x){P=x.P;return *this;}
 void Set(T* p){P=p;}T* Get()const{return P;}T* operator->()const{assert(P);return P;}T& operator*()const{assert(P);return *P;}
};
struct CTCPhysicsBase {C3DVector* Position;void ResetPosition(const C3DVector& p){++C->Resets;*Position=p;}void SetPosition(const C3DVector& p){++C->Positions;*Position=p;}void SetFacingAngleXY(float angle){assert(angle>=0.0f&&angle<1.0f);C->LastFacing=angle;++C->Facing;}};
struct CTCBoss {bool IsReadyForPhaseChange()const{return C->Tick>=C->ReadyAfter;}void SetNextBossPhase(int n,bool b){assert(n==1&&!b);++C->Phase;}};
struct CTCCombat {void SetShotTypeOverride(long n){++C->ShotSets;C->LastShot=(int)n;}};
struct CTCJackShield {void EnableShield(bool b){if(b)++C->ShieldOn;else ++C->ShieldOff;}};
struct CTCSwitchableNavigation {void SetAsBlockingNavigation(bool b){if(b)++C->NavOn;else ++C->NavOff;}};
struct CTCSolidToThing {void AddThingNotSolidTo(const CThing*){++C->Solid;}};
namespace NHitLocations {struct CHitLocation {bool Enabled;void SetEnabled(bool e){Enabled=e;}};}
static NHitLocations::CHitLocation Hit;
struct CTCHitLocations {NHitLocations::CHitLocation* GetPHitLocationWithDefName(const char* s){assert(!std::strcmp(s,"HIT_LOCATION_JACK_OF_BLADES"));return &Hit;}};
struct CTCTargeted {bool Targetable;void SetTargetable(bool b){Targetable=b;}};
struct CTCPerceiveThing {bool IsThingLineOfSight()const{return C->LOS;}};
struct CTCTalk {
 bool IsTalking()const{return C->Talking;}
 int PlayAISpeech(const char* s,bool a,NSpeechGainManager::EDialogueLayer layer,bool b,bool c){assert(a&&!b&&c&&layer==0);++C->Speech;const char* p=std::strrchr(s,'_');assert(p);C->LastSpeech=(p[1]-'0')*10;return 0;}
};
struct CTCDPhysicalObstruction {
 static CThing* Create(const C3DVector&,const C3DVector&,float);
 void SetLifeTimeSeconds(float n){assert(n==5.0f);}
 void SetThingToIgnoreObstruction(CThing*){}
};
static CTCBoss Boss;static CTCCombat Combat;static CTCJackShield Shield;static CTCSwitchableNavigation Nav;static CTCSolidToThing Solid;
static CTCHitLocations Locations;static CTCTargeted Targeted;static CTCPerceiveThing Perceive;static CTCTalk Talk;static CTCDPhysicalObstruction Obstruction;
template<class T>T* Component();
template<> CTCBoss* Component<CTCBoss>(){return &Boss;}
template<> CTCCombat* Component<CTCCombat>(){return &Combat;}
template<> CTCJackShield* Component<CTCJackShield>(){return C->ShieldTC?&Shield:0;}
template<> CTCSwitchableNavigation* Component<CTCSwitchableNavigation>(){return C->NavTC?&Nav:0;}
template<> CTCHitLocations* Component<CTCHitLocations>(){return &Locations;}
template<> CTCTargeted* Component<CTCTargeted>(){return &Targeted;}
template<> const CTCPerceiveThing* Component<const CTCPerceiveThing>(){return &Perceive;}
template<> CTCTalk* Component<CTCTalk>(){return &Talk;}
template<> CTCDPhysicalObstruction* Component<CTCDPhysicalObstruction>(){return &Obstruction;}
class CThing {
public:C3DVector Pos;CTCPhysicsBase Physics;bool Alive;CThingBaseDef Def;
 CThing():Alive(true){Physics.Position=&Pos;}
 const C3DVector& GetPos()const{return Pos;}CTCPhysicsBase* GetPhysicsTC(){return &Physics;}
 unsigned long GetPlayerNumber()const{return 7;}
 template<class T>bool GetTC(T** out){T* p=Component<T>();if(p)*out=p;return p!=0;}
 template<class T>bool PeekTC(T** out)const{T* p=Component<T>();if(p)*out=p;return p!=0;}
 template<class T>bool GetTCDriver(T** out){return GetTC(out);}
 CTCSolidToThing* AddTC(const char* s,bool b,int n){assert(!std::strcmp(s,"CTCSolidToThing")&&!b&&n==0);return &Solid;}
 void Kill(bool b){assert(b);Alive=false;++C->Kill;}
 const CThingBaseDef& PeekBaseDef()const{return Def;}
};
struct CCombatTypeDef {float FrontMiddleLimit;};static CCombatTypeDef CombatDef;
class CThingCreatureBase:public CThing {
public:const CCombatTypeDef& GetCombatTypeDef()const{return CombatDef;}
 bool IsPerformingAnyAction(){if(C->ActionsPending){--C->ActionsPending;return true;}return false;}
};
class CThingObject:public CThing {public:static CThingObject* Create(long,const C3DVector&,unsigned long,int,int,const char*);};
static CThingCreatureBase Creature,Target;static CThing Arena,Existing,Obstacle;static CThingObject Created;
CThingObject* CThingObject::Create(long n,const C3DVector& p,unsigned long owner,int a,int b,const char* s){assert(n==6&&owner==7&&a==0&&b==0&&!*s);++C->Creates;Created.Pos=p;Created.Alive=true;return C->CreateOK?&Created:0;}
CThing* CTCDPhysicalObstruction::Create(const C3DVector& a,const C3DVector& b,float r){assert(a.X==b.X&&a.Y==b.Y&&r==2.5f);++C->Obstructions;return C->ObstructionOK?&Obstacle:0;}
template<class T>bool IS_THING_ALIVE(const CIntelligentPointer<T>& p){return p.Get()!=0&&p->Alive;}
struct CTCWillResponse {
 static bool ForcePushable,Unwillable;
 static void SetThingIsForcePushable(CThing*,bool b){ForcePushable=b;}
 static void SetThingAsNotBloodyWillableAtAll(CThing*,bool b){Unwillable=b;}
};bool CTCWillResponse::ForcePushable;bool CTCWillResponse::Unwillable;
struct CGameDefinitionManager {
 static CGameDefinitionManager& Get(){static CGameDefinitionManager m;return m;}
 long GetDefGlobalIndexFromName(const char* s)const{++C->DefinitionReads;const char* names[]={"JACK_BATTLE_PHASE_1_MINIONS_FAMILY","ENVIRONMENT_JACK_OF_BLADES_FADE","FIREBALL_JACK_OF_BLADES","FIREBALL_JACK_OF_BLADES2","FIREBALL_JACK_OF_BLADES3","OBJECT_JACK_OF_BLADES_SHIELD"};for(int i=0;i<6;++i)if(!std::strcmp(s,names[i]))return i+1;assert(false);return -1;}
};
struct CEnvironment {void TransitionToThemeAllChannels(int channel,long def,float time){assert(channel==3&&def==2&&time==2.0f);++C->Theme;}};
struct CWorld {CEnvironment E;CEnvironment& GetEnvironment(){return E;}long GetFrame(){C->Frame+=C->FrameStep;return C->Frame;}};static CWorld World;
struct CMainGameComponent {static long GetConstantFPS(){return 25;}};
struct CCreatureActionBase {int Kind;CCreatureActionBase(int k):Kind(k){}};
struct CCreatureAction_LeapBack:CCreatureActionBase {CCreatureAction_LeapBack(CThingCreatureBase&,CThingCreatureBase&):CCreatureActionBase(3){}};
struct CCreatureAction_JackShotNear:CCreatureActionBase {CCreatureAction_JackShotNear(CThingCreatureBase&,CThingCreatureBase&):CCreatureActionBase(4){}};
struct CCreatureAction_JackShotFar:CCreatureActionBase {CCreatureAction_JackShotFar(CThingCreatureBase&,CThingCreatureBase&):CCreatureActionBase(5){}};
struct CCreatureAction_PlayCombatAnimation:CCreatureActionBase {CCreatureAction_PlayCombatAnimation(CThingCreatureBase&,CThingCreatureBase&,const char* n,bool b):CCreatureActionBase(6){assert(!std::strcmp(n,"SPECIAL_BOAST")&&b);}};
class CAIStateGroup_JackOfBladesStage1 {
public:CIntelligentPointer<CThing> PArenaCentre,PShieldThing;CIntelligentPointer<CThingCreatureBase> PTargetThing;long SpeechCounter;
 CAIStateGroup_JackOfBladesStage1():SpeechCounter(0){PArenaCentre.Set(&Arena);PTargetThing.Set(&Target);if(C->ExistingShield)PShieldThing.Set(&Existing);}
 CThingCreatureBase& GetCreature(){return Creature;}CWorld& GetWorld(){return World;}
 bool IsStillRunnable(){++C->RunnableChecks;assert(C->RunnableChecks<1001);return C->Runnable&&C->RunnableChecks<C->StopQuery;}
 void Terminate(){++C->Terminate;}
 void Summon(long family,bool b){assert(family==1&&!b);++C->Summons;}
 void PowerUpSword(){++C->PowerUps;}
 bool HasEnemyBeenKnockedDown(){return C->Knocked;}
 EAIActionReturn RushToPos(const C3DVector&){++C->Rush;C->Step(7);return AI_ACTION_FINISHED;}
 EAIActionReturn AIAction_MoveToThingProximity(EAIActionRepeat r,CIntelligentPointer<const CThing>,float distance,bool& reached,EMovementType m,bool a,bool b,bool c,bool d,float speed){assert(r==ACTION_REPEAT_CALLBACK&&m==3&&a&&!b&&!c&&!d&&speed==0);assert(distance==10||distance==4);reached=!C->MoveFailed;++C->Move;C->Step(1);return C->MoveFailed?AI_ACTION_FAILED:AI_ACTION_FINISHED;}
 EAIActionReturn AIAction_LookAtThing(EAIActionRepeat r,CIntelligentPointer<const CThing>,long frames,float speed){assert(r==ACTION_REPEAT_UNTIL_FINISHED&&frames==35&&speed>0);++C->Look;C->Step(2);return AI_ACTION_FINISHED;}
 EAIActionReturn AIAction_PerformActionAndWait(const CCreatureActionBase& a,bool b){assert(!b);if(a.Kind==3)++C->Leap;else if(a.Kind==4)++C->Near;else if(a.Kind==5)++C->Far;else if(a.Kind==6)++C->Boast;else assert(false);C->Step(a.Kind);return C->ActionFinished?AI_ACTION_FINISHED:AI_ACTION_FAILED;}
 EAIActionReturn MeleeCombat(){++C->Melee;C->ActionsPending=2;C->Step(8);return AI_ACTION_FINISHED;}
 EAIActionReturn AIAction_DoNothing(){++C->Nothing;C->Step(9);return AI_ACTION_FINISHED;}
 void Update();
};

void CAIStateGroup_JackOfBladesStage1::Update()
{
    long minion_family_index=CGameDefinitionManager::Get().GetDefGlobalIndexFromName("JACK_BATTLE_PHASE_1_MINIONS_FAMILY");
    long glow_def_num=CGameDefinitionManager::Get().GetDefGlobalIndexFromName("ENVIRONMENT_JACK_OF_BLADES_FADE");
    GetWorld().GetEnvironment().TransitionToThemeAllChannels(3,glow_def_num,2.0f);
    CDefPointer<const CJackOfBladesBattleDef> p_jack_def;
    GetCreature().PeekBaseDef().GetSubDef(&p_jack_def);
    CTCBoss* ptc_boss;
    GetCreature().GetTC(&ptc_boss);
    CTCCombat* ptc_combat;
    GetCreature().GetTC(&ptc_combat);
    CTCWillResponse::SetThingIsForcePushable(&GetCreature(),false);
    EAIActionReturn ret=AI_ACTION_FINISHED;
    long num_shot_types=3;
    long normal_shot_type=CGameDefinitionManager::Get().GetDefGlobalIndexFromName("FIREBALL_JACK_OF_BLADES");
    long split_shot_type1=CGameDefinitionManager::Get().GetDefGlobalIndexFromName("FIREBALL_JACK_OF_BLADES2");
    long split_shot_type2=CGameDefinitionManager::Get().GetDefGlobalIndexFromName("FIREBALL_JACK_OF_BLADES3");
    if(!IsStillRunnable()){Terminate();return;}
    GetCreature().GetPhysicsTC()->ResetPosition(PArenaCentre->GetPos());
    if(!IsStillRunnable()){Terminate();return;}
    // Original unused local: FableWin PDB -0x34 and donor02BE777D.
    float forcefield_radius=3.0f;
    CIntelligentPointer<CThingObject> pshield_object;
    if(IS_THING_ALIVE(PShieldThing)) {
        CTCJackShield* ptc_jack_shield;
        if(PShieldThing->GetTC(&ptc_jack_shield)) {
            PShieldThing->GetPhysicsTC()->SetPosition(PArenaCentre->GetPos());
            ptc_jack_shield->EnableShield(true);
        }
    } else {
        long shield_object_index=CGameDefinitionManager::Get().GetDefGlobalIndexFromName("OBJECT_JACK_OF_BLADES_SHIELD");
        CThingObject* pshield_object_temp=CThingObject::Create(shield_object_index,PArenaCentre->GetPos(),GetCreature().GetPlayerNumber(),0,0,"");
        if(pshield_object_temp) {
            pshield_object=CIntelligentPointer<CThingObject>(pshield_object_temp);
            CTCSolidToThing* ptc_solid_to_thing=static_cast<CTCSolidToThing*>(pshield_object->AddTC("CTCSolidToThing",false,0));
            ptc_solid_to_thing->AddThingNotSolidTo(&GetCreature());
            pshield_object->GetPhysicsTC()->SetPosition(PArenaCentre->GetPos());
            CTCSwitchableNavigation* ptc_switch_nav;
            if(pshield_object->GetTC(&ptc_switch_nav))ptc_switch_nav->SetAsBlockingNavigation(true);
            CTCJackShield* ptc_jack_shield;
            if(pshield_object->GetTC(&ptc_jack_shield))ptc_jack_shield->EnableShield(true);
        }
    }
    CTCHitLocations* p_TC_hit_locations;
    GetCreature().GetTC(&p_TC_hit_locations);
    NHitLocations::CHitLocation* p_hit_location_final=p_TC_hit_locations->GetPHitLocationWithDefName("HIT_LOCATION_JACK_OF_BLADES");
    p_hit_location_final->SetEnabled(false);
    CTCTargeted* ptc_targeted;
    GetCreature().GetTC(&ptc_targeted);
    ptc_targeted->SetTargetable(false);
    GetCreature().GetPhysicsTC()->ResetPosition(PArenaCentre->GetPos()+C3DVector(0,0,0));
    {
        C3DVector vector_to_target=PTargetThing->GetPos()-GetCreature().GetPos();
        if(vector_to_target.IsNonZero()) {
            vector_to_target=vector_to_target.Normalise();
            GetCreature().GetPhysicsTC()->SetFacingAngleXY(vector_to_target.GetAngleXY());
        }
    }
    if(!IsStillRunnable()){Terminate();return;}
    Summon(minion_family_index,false);
    PowerUpSword();
    if(IS_THING_ALIVE(PShieldThing)) {
        CTCJackShield* ptc_jack_shield;
        if(PShieldThing->GetTC(&ptc_jack_shield))ptc_jack_shield->EnableShield(false);
    } else if(IS_THING_ALIVE(pshield_object)) {
        CTCSwitchableNavigation* ptc_switch_nav;
        if(pshield_object->GetTC(&ptc_switch_nav))ptc_switch_nav->SetAsBlockingNavigation(false);
        CTCJackShield* ptc_jack_shield;
        if(pshield_object->GetTC(&ptc_jack_shield))ptc_jack_shield->EnableShield(false);
        pshield_object->Kill(true);
    }
    p_hit_location_final->SetEnabled(true);
    ptc_targeted->SetTargetable(true);
    CTCWillResponse::SetThingIsForcePushable(&GetCreature(),true);
    float ideal_distance_for_shot=p_jack_def->IdealShootDistance;
    float max_distance_for_shot=p_jack_def->RushTriggerDistance;
    float time_between_shots=p_jack_def->TimeBetweenShots;
    long frame_of_last_shot=GetWorld().GetFrame();
    long shot_counter=0;
    while(IsStillRunnable()&&!ptc_boss->IsReadyForPhaseChange()) {
        C3DVector line_to_target=PTargetThing->GetPos()-GetCreature().GetPos();
        float dis_to_target=line_to_target.GetMagnitude();
        bool moved=false;
        if(dis_to_target>max_distance_for_shot) {
            C3DVector dir_to_target=line_to_target;
            dir_to_target=dir_to_target.Normalise();
            C3DVector rush_to_pos=GetCreature().GetPos()+dir_to_target*p_jack_def->RushToDistance;
            RushToPos(rush_to_pos);
            if(!IsStillRunnable()){Terminate();return;}
        } else if(dis_to_target>ideal_distance_for_shot+3.0f) {
            bool nav;
            ret=AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK,PTargetThing,ideal_distance_for_shot,nav,(EMovementType)3,true,false,false,false,0.0f);
            if(!IsStillRunnable()){Terminate();return;}
            if(ret!=AI_ACTION_FAILED)moved=true;
        }
        if(ptc_boss->IsReadyForPhaseChange())break;
        long current_frame=GetWorld().GetFrame();
        float time_since_last_shot=(float)(current_frame-frame_of_last_shot)/CMainGameComponent::GetConstantFPS();
        dis_to_target=(GetCreature().GetPos()-PTargetThing->GetPos()).GetMagnitude();
        if(dis_to_target<GetCreature().GetCombatTypeDef().FrontMiddleLimit) {
            if(HasEnemyBeenKnockedDown()) {
                AIAction_PerformActionAndWait(CCreatureAction_LeapBack(GetCreature(),*PTargetThing),false);
            } else {
                MeleeCombat();
                while(GetCreature().IsPerformingAnyAction()) {
                    AIAction_DoNothing();
                    if(!IsStillRunnable()){Terminate();return;}
                }
            }
        } else if(time_since_last_shot>time_between_shots) {
            const CTCPerceiveThing* ptc_perceive_thing;
            GetCreature().PeekTC(&ptc_perceive_thing);
            if(ptc_perceive_thing->IsThingLineOfSight()) {
                AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED,PTargetThing,35,0.1f);
                if(!IsStillRunnable()){Terminate();return;}
                if(!ptc_boss->IsReadyForPhaseChange()) {
                    switch(shot_counter%num_shot_types) {
                    case 0:ptc_combat->SetShotTypeOverride(normal_shot_type);break;
                    case 1:ptc_combat->SetShotTypeOverride(split_shot_type1);break;
                    case 2:ptc_combat->SetShotTypeOverride(split_shot_type2);break;
                    }
                    if(dis_to_target<ideal_distance_for_shot+3.0f) {
                        if(AIAction_PerformActionAndWait(CCreatureAction_JackShotNear(GetCreature(),*PTargetThing),false)==AI_ACTION_FINISHED) {
                            frame_of_last_shot=GetWorld().GetFrame();++shot_counter;
                        }
                    } else if(dis_to_target<max_distance_for_shot) {
                        if(AIAction_PerformActionAndWait(CCreatureAction_JackShotFar(GetCreature(),*PTargetThing),false)==AI_ACTION_FINISHED) {
                            frame_of_last_shot=GetWorld().GetFrame();++shot_counter;
                        }
                    }
                }
            } else {
                bool nav;
                ret=AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK,PTargetThing,GetCreature().GetCombatTypeDef().FrontMiddleLimit,nav,(EMovementType)3,true,false,false,false,0.0f);
                if(!IsStillRunnable()){Terminate();return;}
                if(ret!=AI_ACTION_FAILED)moved=true;
            }
        } else if(HasEnemyBeenKnockedDown()) {
            CTCTalk* ptc_talk;
            if(GetCreature().GetTC(&ptc_talk)&&!ptc_talk->IsTalking()) {
                long random_int=SpeechCounter%6;
                switch(random_int) {
                case 0:ptc_talk->PlayAISpeech("TEXT_QST_039_JACK_BOSS_FIGHT_TAUNT_10",true,(NSpeechGainManager::EDialogueLayer)0,false,true);break;
                case 1:ptc_talk->PlayAISpeech("TEXT_QST_039_JACK_BOSS_FIGHT_TAUNT_20",true,(NSpeechGainManager::EDialogueLayer)0,false,true);break;
                case 2:ptc_talk->PlayAISpeech("TEXT_QST_039_JACK_BOSS_FIGHT_TAUNT_30",true,(NSpeechGainManager::EDialogueLayer)0,false,true);break;
                case 3:ptc_talk->PlayAISpeech("TEXT_QST_039_JACK_BOSS_FIGHT_TAUNT_40",true,(NSpeechGainManager::EDialogueLayer)0,false,true);break;
                }
                ++SpeechCounter;
            }
            AIAction_PerformActionAndWait(CCreatureAction_PlayCombatAnimation(GetCreature(),*PTargetThing,"SPECIAL_BOAST",true),false);
        } else if(!moved) {
            AIAction_LookAtThing(ACTION_REPEAT_UNTIL_FINISHED,PTargetThing,35,0.1f);
        }
    }
    if(ptc_boss->IsReadyForPhaseChange()) {
        if(IsStillRunnable()) {
            CTCWillResponse::SetThingAsNotBloodyWillableAtAll(&GetCreature(),true);
            CThing* p_obstruction_thing=CTCDPhysicalObstruction::Create(PArenaCentre->GetPos(),PArenaCentre->GetPos(),2.5f);
            if(p_obstruction_thing) {
                CTCDPhysicalObstruction* ptcd_obstruction;
                p_obstruction_thing->GetTCDriver(&ptcd_obstruction);
                ptcd_obstruction->SetLifeTimeSeconds(5.0f);
                ptcd_obstruction->SetThingToIgnoreObstruction(&GetCreature());
            }
            RushToPos(PArenaCentre->GetPos());
            if(IsStillRunnable()) {
                C3DVector vector_to_target=PTargetThing->GetPos()-GetCreature().GetPos();
                if(vector_to_target.IsNonZero())GetCreature().GetPhysicsTC()->SetFacingAngleXY(vector_to_target.GetAngleXY());
            }
        }
        ptc_boss->SetNextBossPhase(1,false);
    }
    Terminate();
}

static int Cases;
static void Run(CaseState& state,long speech=0)
{
 C=&state;Creature.Pos=C3DVector();Arena.Pos=C3DVector();Target.Pos=C3DVector(state.Distance,0,0);Existing.Alive=true;Created.Alive=true;
 Battle.IdealShootDistance=10;Battle.RushTriggerDistance=15;Battle.RushToDistance=8;Battle.TimeBetweenShots=state.ShotInterval;CombatDef.FrontMiddleLimit=4;
 Hit.Enabled=true;Targeted.Targetable=true;CTCWillResponse::ForcePushable=true;CTCWillResponse::Unwillable=false;
 assert(Handles==0&&Defs==0);
 {CAIStateGroup_JackOfBladesStage1 ai;ai.SpeechCounter=speech;int before=Handles;ai.Update();assert(Handles==before&&Defs==0);}
 assert(Handles==0&&Defs==0);assert(state.Terminate==1&&state.Theme==1);++Cases;
}
static void CheckMathHelpers()
{
 assert(C3DVector(0.0f,1.0f,0.0f).GetAngleXY()==0.0f);
 assert(C3DVector(1.0f,0.0f,0.0f).GetAngleXY()==0.25f);
 assert(C3DVector(0.0f,-1.0f,0.0f).GetAngleXY()==0.5f);
 assert(C3DVector(-1.0f,0.0f,0.0f).GetAngleXY()==0.75f);
 assert(GET_ROUNDED_ANGLE(-1.25f)==0.75f);
 assert(GET_ROUNDED_ANGLE(1.25f)==0.25f);
 assert(GET_ROUNDED_ANGLE(1.0f)==0.0f);
 assert(C3DVector(3.0f,4.0f,0.0f).GetMagnitude()==5.0f);
 assert(C3DVector(0.0f,0.0f,8.0f).GetMagnitude()==8.0f);
 assert(!C3DVector(0.0f,0.0f,0.0f).IsNonZero());
 assert(!C3DVector(1.0f/16384.0f,0.0f,0.0f).IsNonZero());
 assert(C3DVector(1.0f/8192.0f,0.0f,0.0f).IsNonZero());
}
int main()
{
 CheckMathHelpers();
 {CaseState s;Run(s);assert(s.Near==1&&s.Far==0&&s.Creates==1&&s.Kill==1&&s.Phase==1&&s.ShieldOn==1&&s.ShieldOff==1&&s.NavOn==1&&s.NavOff==1&&s.Solid==1);assert(Hit.Enabled&&Targeted.Targetable&&CTCWillResponse::ForcePushable&&CTCWillResponse::Unwillable);assert(s.LastFacing==0.25f);}
 {CaseState s;s.ExistingShield=true;Run(s);assert(s.Creates==0&&s.Kill==0&&s.ShieldOn==1&&s.ShieldOff==1&&s.NavOn==0);}
 {CaseState s;s.CreateOK=false;Run(s);assert(s.Creates==1&&s.Kill==0&&s.ShieldOn==0&&s.NavOn==0);}
 {CaseState s;s.ShieldTC=false;Run(s);assert(s.ShieldOn==0&&s.ShieldOff==0&&s.NavOn==1&&s.Kill==1);}
 {CaseState s;s.ExistingShield=true;s.ShieldTC=false;Run(s);assert(s.Creates==0&&s.ShieldOn==0&&s.ShieldOff==0);}
 {CaseState s;s.NavTC=false;Run(s);assert(s.NavOn==0&&s.NavOff==0&&s.ShieldOn==1&&s.ShieldOff==1);}
 {CaseState s;s.NavTC=false;s.ShieldTC=false;Run(s);assert(s.Kill==1&&s.ShieldOn==0&&s.NavOn==0);}
 {CaseState s;s.ObstructionOK=false;Run(s);assert(s.Obstructions==1&&s.Phase==1);}
 {CaseState s;s.ReadyAfter=0;Run(s);assert(s.Near==0&&s.Melee==0&&s.Move==0&&s.Phase==1&&s.Obstructions==1&&s.Rush==1);}
 {CaseState s;s.Distance=2;Run(s);assert(s.Melee==1&&s.Nothing==2&&s.Near==0&&s.Phase==1);}
 {CaseState s;s.Distance=2;s.Knocked=true;Run(s);assert(s.Leap==3&&s.Melee==0&&s.Speech==0&&s.Phase==1);}
 {CaseState s;s.Distance=20;Run(s);assert(s.Rush>=3&&s.Move==0&&s.Near==0&&s.Far==0);}
 {CaseState s;s.Distance=14;Run(s);assert(s.Move==1&&s.Far==1&&s.Near==0);}
 {CaseState s;s.Distance=14;s.MoveFailed=true;Run(s);assert(s.Move==1&&s.Far==1);}
 {CaseState s;s.LOS=false;Run(s);assert(s.Move==3&&s.Look==0&&s.Near==0);}
 {CaseState s;s.LOS=false;s.MoveFailed=true;Run(s);assert(s.Move==3&&s.Look==0);}
 {CaseState s;s.ShotInterval=10000;Run(s);assert(s.Look==3&&s.Near==0&&s.ShotSets==0);}
 {CaseState s;s.Distance=14;s.ShotInterval=10000;Run(s);assert(s.Move==3&&s.Look==0);}
 {CaseState s;s.Distance=14;s.ShotInterval=10000;s.MoveFailed=true;Run(s);assert(s.Move==2&&s.Look==1);}
 for(int speech=0;speech<6;++speech){CaseState s;s.ReadyAfter=1;s.ShotInterval=10000;s.Knocked=true;Run(s,speech);assert(s.Boast==1&&s.Leap==0&&s.Speech==(speech<4?1:0));if(speech<4)assert(s.LastSpeech==(speech+1)*10);}
 {CaseState s;s.ShotInterval=10000;s.Knocked=true;s.Talking=true;Run(s);assert(s.Speech==0&&s.Boast==3);}
 {CaseState s;s.ReadyAfter=7;Run(s);assert(s.Near==3&&s.ShotSets==3&&s.LastShot==5);}
 {CaseState s;s.ReadyAfter=9;Run(s);assert(s.Near==4&&s.LastShot==3);}
 {CaseState s;s.ReadyAfter=7;s.ActionFinished=false;Run(s);assert(s.Near==3&&s.LastShot==3);}
 {CaseState s;s.Distance=13;Run(s);assert(s.Move==0&&s.Far==1&&s.Near==0);}
 {CaseState s;s.Distance=15;Run(s);assert(s.Move==2&&s.Look==1&&s.Near==0&&s.Far==0);}
 {CaseState s;s.Distance=4;Run(s);assert(s.Melee==0&&s.Near==1);}
 {CaseState s;s.Distance=0;Run(s);assert(s.Facing==0&&s.Melee==1&&s.Nothing==2);}
 for(int query=1;query<=3;++query){CaseState s;s.StopQuery=query;Run(s);assert(s.Phase==0&&s.Summons==0);if(query<3)assert(s.Creates==0);else assert(!Hit.Enabled&&!Targeted.Targetable);}
 {CaseState s;s.Distance=14;s.ReadyAfter=100;s.StopAction=1;Run(s);assert(s.Move==1&&s.Far==0&&s.Phase==0);}
 {CaseState s;s.ReadyAfter=100;s.StopAction=2;Run(s);assert(s.Look==1&&s.Near==0&&s.Phase==0);}
 {CaseState s;s.Distance=20;s.ReadyAfter=100;s.StopAction=7;Run(s);assert(s.Rush==1&&s.Phase==0);}
 {CaseState s;s.Distance=2;s.ReadyAfter=100;s.StopAction=9;Run(s);assert(s.Melee==1&&s.Nothing==1&&s.Phase==0);}
 {CaseState s;s.ReadyAfter=0;s.StopAction=7;Run(s);assert(s.Phase==1&&s.Rush==1&&s.Facing==1);}
 {CaseState s;s.ReadyAfter=0;s.StopQuery=5;Run(s);assert(s.Phase==1&&s.Obstructions==0);}
 {CaseState s;s.ReadyAfter=0;s.StopQuery=6;Run(s);assert(s.Phase==1&&s.Obstructions==1&&s.Facing==1);}
 {CaseState s;s.ReadyAfter=100;s.StopAction=4;Run(s);assert(s.Near==1&&s.Phase==0);}
 assert(Cases==44);
 std::puts("00975bb0_TEST PASS");return 0;
}
