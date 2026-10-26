// CAIStateGroup_HoverHornetQueen::KeepOutOfWay() -- choose a safe circle point near the target, otherwise idle; clear timing-critical mode afterward.
// Retail0095b6f0 length297. Queen donor CurrentCirclePoint/CircleMarkers/TimingCritical are four/eight bytes too high.
// PlayAnimation donor sizec0 differs retailb4; use native lifecycle view with a real virtual base destructor.
#include <math.h>
#ifdef _MSC_VER
#include "engine/CAIStateGroup_HoverHornetQueen.h"
#include "engine/CCreatureAction_PlayAnimation.h"
#include "engine/CAIBrain.h"
#include "fable_string.h"
#else
class CAIBrain { public:char prefix[24];void* Creature;};
class CCharString { public:void* storage_;CCharString(const char*,long);~CCharString();};
#endif
class C3DVector { public:float X,Y,Z;};
struct CQueenThing {const C3DVector& GetPosition();}; //004c73d0
struct CQueenPointer {virtual ~CQueenPointer();CQueenThing* item;CQueenThing* GetPItem()const;}; //00a01b50
struct CAnimationSound {long first,last;CAnimationSound():first(0),last(0){}};
struct CActionNative {virtual ~CActionNative();char state[0xa4];}; //00693ef0
struct CPlayAnimationNative:CActionNative {bool UseAnimationMovement,UsePhysics,StayOnLastFrame,Looping;long NumberOfTimesToLoop;bool AnimMayNeedCameraPositionUpdatedInSpeakAndWait;
CPlayAnimationNative(CQueenThing*,const CCharString&,const CAnimationSound&,bool,bool,bool,long,long); //008424f0
};
struct CQueenNative {
virtual ~CQueenNative();CAIBrain* brain;unsigned char baseState[12];CQueenPointer target;
C3DVector StartPos,ArenaCentrePos;bool bOneTime;unsigned char alignment[3];CQueenThing* PEmitter;CQueenThing* PEmitter2;unsigned char Things[12];
long CurrentCirclePoint;CQueenPointer* first;CQueenPointer* last;CQueenPointer* capacity;long allocator;bool TimingCritical;
bool IsStillRunnable();void Terminate(); //0095a420,008fcf40
long AIAction_DoNothing();long GotoSafestCirclePoint(long); //0097e200,0095ad20
long AIAction_PerformActionAndWait(CActionNative*,bool); //0097db70
CQueenThing* GetCreature(){return (CQueenThing*)brain->Creature;}
long KeepOutOfWay();
};
long CQueenNative::KeepOutOfWay(){
if(!IsStillRunnable()){Terminate();return 1;}
TimingCritical=true;
CQueenThing* creature=GetCreature();
const C3DVector& enemy=target.GetPItem()->GetPosition();
const C3DVector& owner=creature->GetPosition();
float dx=owner.X-enemy.X,dy=owner.Y-enemy.Y;
long result;
if((float)sqrt(dx*dx+dy*dy)<15.0f){
if((unsigned long)(last-first)==0)result=AIAction_DoNothing();else result=GotoSafestCirclePoint(9);
if(result==3||!IsStillRunnable()){TimingCritical=false;return result;}
}else{
CCharString animation("ST_IDLE",-1);
result=AIAction_PerformActionAndWait(&CPlayAnimationNative(GetCreature(),animation,CAnimationSound(),false,true,false,0,0),false);
}
if(!IsStillRunnable())Terminate();
TimingCritical=false;return result;
}

#include <cstdio>
#include <cstring>
static int errors,scenario,checks,terminates,idles,paths,actions,strings,actionLive,stringLive,actionDtors;
static CQueenNative* expectedGroup;
static CQueenThing creature,enemy;
static C3DVector ownerPos={3,4,0},enemyPos={0,0,900};
CQueenNative::~CQueenNative(){}
CQueenPointer::~CQueenPointer(){}
CQueenThing* CQueenPointer::GetPItem()const{return item;}
const C3DVector& CQueenThing::GetPosition(){if(this==&creature)return ownerPos;if(this!=&enemy)++errors;return enemyPos;}
bool CQueenNative::IsStillRunnable(){++checks;if(checks>1&&(!TimingCritical||actionLive||stringLive))++errors;return scenario!=0 && !(scenario==3&&checks==2) && !(scenario==4&&checks==3) && !(scenario==6&&checks==2);}
void CQueenNative::Terminate(){++terminates;}
long CQueenNative::AIAction_DoNothing(){++idles;if(!TimingCritical)++errors;return 2;}
long CQueenNative::GotoSafestCirclePoint(long type){++paths;if(!TimingCritical||type!=9)++errors;return scenario==2?3:4;}
CCharString::CCharString(const char* text,long len){++strings;++stringLive;if(std::strcmp(text,"ST_IDLE")||len!=-1||!expectedGroup->TimingCritical)++errors;storage_=0;}
CCharString::~CCharString(){if(actionLive)++errors;--stringLive;}
CPlayAnimationNative::CPlayAnimationNative(CQueenThing* c,const CCharString&,const CAnimationSound& sound,bool a,bool b,bool d,long e,long f){
++actionLive;if(c!=&creature||sound.first||sound.last||a||!b||d||e||f||stringLive!=1||!expectedGroup->TimingCritical)++errors;}
CActionNative::~CActionNative(){if(stringLive!=1)++errors;--actionLive;++actionDtors;}
long CQueenNative::AIAction_PerformActionAndWait(CActionNative* p,bool wait){++actions;if(!p||wait||actionLive!=1||stringLive!=1||!TimingCritical)++errors;return 2;}
int main(){CAIBrain brain;CQueenNative group;CQueenPointer markers[2];
#ifdef _MSC_VER
brain.Creature=(CThingAICreature*)&creature;
#else
brain.Creature=&creature;
#endif
group.brain=&brain;group.target.item=&enemy;expectedGroup=&group;group.first=markers;
for(scenario=0;scenario<8;++scenario){checks=terminates=idles=paths=actions=strings=actionDtors=0;group.TimingCritical=false;group.last=scenario==1?markers:markers+2;ownerPos.X=scenario>=5?15:3;ownerPos.Y=scenario>=5?0:4;
long r=group.KeepOutOfWay();
long expected=scenario==0?1:scenario==2?3:scenario==1||scenario>=5?2:4;
if(r!=expected||group.TimingCritical||stringLive||actionLive)++errors;
if(scenario==0){if(checks!=1||terminates!=1||idles||paths||actions)++errors;}
else if(scenario>=5){if(checks!=2||terminates!=(scenario==6)||idles||paths||actions!=1||strings!=1||actionDtors!=1)++errors;}
else {int wantChecks=scenario==2?1:scenario==3?2:3;if(checks!=wantChecks||terminates!=(scenario==4)||idles!=(scenario==1)||paths!=(scenario!=1)||actions||strings||actionDtors)++errors;}
}
if(errors){std::printf("0095b6f0_TEST FAIL %d\n",errors);return 1;}std::printf("0095b6f0_TEST PASS\n");return 0;}