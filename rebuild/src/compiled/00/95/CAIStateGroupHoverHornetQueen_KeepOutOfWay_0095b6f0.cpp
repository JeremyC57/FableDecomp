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
struct CAIBrain {char prefix[24];void* Creature;};
struct CCharString {void* storage_;CCharString(const char*,long);~CCharString();};
#endif
struct C3DVector {float X,Y,Z;};
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