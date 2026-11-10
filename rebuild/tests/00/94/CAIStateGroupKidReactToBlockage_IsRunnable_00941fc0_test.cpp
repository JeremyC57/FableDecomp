#include <stdio.h>
#include <assert.h>
#include <math.h>
const float DELTA_NOTIONAL_ZERO=0.0001f;
class C2DCoordI{public:long X,Y;C2DCoordI(long x,long y):X(x),Y(y){}};
class C2DBoxI{public:long TLX,TLY,BRX,BRY;C2DBoxI(const C2DCoordI&p,long r):TLX(p.X-r),TLY(p.Y-r),BRX(p.X+r),BRY(p.Y+r){}};
class C3DVector{public:float X,Y,Z;C3DVector():X(0),Y(0),Z(0){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}C3DVector(const C3DVector&v):X(v.X),Y(v.Y),Z(v.Z){}const C3DVector operator-(const C3DVector&v)const{return C3DVector(X-v.X,Y-v.Y,Z-v.Z);}float GetSquaredMagnitude()const{return X*X+Z*Z+Y*Y;}bool IsNonZero()const{return Z*Z+Y*Y+X*X>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO;}C2DCoordI ToC2DCoordI()const{return C2DCoordI((long)X,(long)Y);}};
class CTCPhysicsBase{public:C3DVector Position,OldPosition;float Radius;const C3DVector&GetPosition()const{return Position;}const C3DVector&GetLastPosition()const{return OldPosition;}float GetRadius()const{return Radius;}};
class CTCCreatureModeManager{public:unsigned mask;int calls;bool IsModeActive(unsigned long n)const{static const unsigned long modes[]={0x28,8,9,10,11,0x2d,0x27};++const_cast<CTCCreatureModeManager*>(this)->calls;for(int i=0;i<7;++i)if(n==modes[i])return (mask&(1u<<i))!=0;assert(false);return false;}};
class CThing{public:CTCPhysicsBase physics;const C3DVector&GetPos()const{return physics.Position;}CTCPhysicsBase*GetPhysicsTC(){return &physics;}const CTCPhysicsBase*GetPhysicsTC()const{return &physics;}};
class CThingCreatureBase:public CThing{public:const C3DVector&PeekPos()const{return GetPos();}};
class CThingAICreature:public CThingCreatureBase{public:bool mode_present;CTCCreatureModeManager modes;bool GetTC(CTCCreatureModeManager**p){if(!mode_present)return false;*p=&modes;return true;}bool PeekTC(const CTCPhysicsBase**p)const{*p=&physics;return true;}};
static int pointer_live,pointer_assigns;
template<class T>class CIntelligentPointer{T*value;public:CIntelligentPointer():value(0){++pointer_live;}CIntelligentPointer(T*p):value(p){++pointer_live;}CIntelligentPointer(const CIntelligentPointer&p):value(p.value){++pointer_live;}~CIntelligentPointer(){--pointer_live;}CIntelligentPointer&operator=(const CIntelligentPointer&p){value=p.value;++pointer_assigns;return *this;}T*operator->()const{return value;}T*Get()const{return value;}};
class CThingFilter_IsNot{const CThing&Thing;public:CThingFilter_IsNot(const CThing&t):Thing(t){}bool operator()(const CThing&t)const{return &t!=&Thing;}};
class CIsVeryCloseMotionlessCreature{C3DVector MyPos;float MyRadius;public:CIsVeryCloseMotionlessCreature(CThingAICreature&t):MyPos(t.GetPos()),MyRadius(t.GetPhysicsTC()->GetRadius()){}bool operator()(const CThingCreatureBase&t)const{const CTCPhysicsBase*p=t.GetPhysicsTC();if((p->GetPosition()-p->GetLastPosition()).IsNonZero())return false;C3DVector delta=t.PeekPos()-MyPos;return sqrt(delta.X*delta.X+delta.Y*delta.Y)<(p->GetRadius()+MyRadius)*1.4f;}};
template<class F,class G>class CFilter_Bind2{const F&F1;const G&F2;public:CFilter_Bind2(const F&f,const G&g):F1(f),F2(g){}template<class T>bool operator()(const T&t)const{return F1(t)&&F2(t);}};
template<class F,class G>CFilter_Bind2<F,G>GFFilter_Bind2(const F&f,const G&g){return CFilter_Bind2<F,G>(f,g);}
class CThingCompare_Nearest{C3DVector Pos;public:CThingCompare_Nearest(const C3DVector&p):Pos(p){}template<class T>T*operator()(T*first,T*second)const{if(!first)return second;if(!second)return first;return (first->GetPos()-Pos).GetSquaredMagnitude()<(second->GetPos()-Pos).GetSquaredMagnitude()?first:second;}};
class CThingSearchTools{public:CThingCreatureBase*items[3];int count;mutable int calls;template<class F,class C,class T>bool GetPBestThingInRoughArea(const C2DBoxI&a,const F&f,const C&c,T**out)const{++calls;assert(a.TLX==-6&&a.TLY==-5&&a.BRX==14&&a.BRY==15);T*best=0;for(int i=0;i<count;++i)if(f(*items[i]))best=c(best,static_cast<T*>(items[i]));*out=best;return best!=0;}};
class CWorld{public:static long frame;static long GetFrame(){return frame;}};long CWorld::frame;
class CAIStateGroup_KidReactToBlockage{public:long LastEndState,CurrentActivationMinInterval;float our_radius,their_radius;CIntelligentPointer<CThingCreatureBase>PThingCreature;CThingAICreature creature;CThingSearchTools search;CWorld world;const CWorld&PeekWorld()const{return world;}CThingAICreature&GetCreature(){return creature;}const CThingSearchTools&PeekThingSearchTools()const{return search;}bool IsRunnable();};
bool CAIStateGroup_KidReactToBlockage::IsRunnable(){
 if(PeekWorld().GetFrame()-LastEndState<CurrentActivationMinInterval)return false;
 CTCCreatureModeManager* tc_mode_manager;
 if(GetCreature().GetTC(&tc_mode_manager)){
  if(tc_mode_manager->IsModeActive(0x28)||tc_mode_manager->IsModeActive(8)||tc_mode_manager->IsModeActive(9)||tc_mode_manager->IsModeActive(10)||tc_mode_manager->IsModeActive(11)||tc_mode_manager->IsModeActive(0x2d)||tc_mode_manager->IsModeActive(0x27))return false;
 }
 const CTCPhysicsBase* ptc_physics;
 GetCreature().PeekTC(&ptc_physics);
 if(!(ptc_physics->GetPosition()-ptc_physics->GetLastPosition()).IsNonZero()){
  const C3DVector& creature_pos=GetCreature().PeekPos();
  our_radius=GetCreature().GetPhysicsTC()->GetRadius();
  C2DBoxI area(creature_pos.ToC2DCoordI(),(long)(our_radius*10.0f));
  CThingCreatureBase* pcreature;
  if(PeekThingSearchTools().GetPBestThingInRoughArea(area,GFFilter_Bind2(CThingFilter_IsNot(GetCreature()),CIsVeryCloseMotionlessCreature(GetCreature())),CThingCompare_Nearest(GetCreature().PeekPos()),&pcreature)){
   PThingCreature=pcreature;
   their_radius=PThingCreature->GetPhysicsTC()->GetRadius();
   return true;
  }
 }
 return false;
}

int main(){for(int mask=0;mask<128;++mask)for(int present=0;present<2;++present)for(int time=0;time<3;++time)for(int moving=0;moving<2;++moving)for(int found=0;found<2;++found){
assert(pointer_live==0);{
CAIStateGroup_KidReactToBlockage state;CThingCreatureBase near_thing,far_thing;state.LastEndState=10;state.CurrentActivationMinInterval=2;CWorld::frame=11+time;state.our_radius=-1;state.their_radius=-2;state.creature.mode_present=present!=0;state.creature.modes.mask=(unsigned)mask;state.creature.modes.calls=0;state.creature.physics.Position=C3DVector(4,5,6);state.creature.physics.OldPosition=C3DVector(moving?3.0f:4.0f,5,6);state.creature.physics.Radius=1;
near_thing.physics.Position=near_thing.physics.OldPosition=C3DVector(4.25f,5,6);near_thing.physics.Radius=0.5f;far_thing.physics.Position=far_thing.physics.OldPosition=C3DVector(4.5f,5,6);far_thing.physics.Radius=0.25f;state.search.items[0]=&state.creature;state.search.items[1]=&far_thing;state.search.items[2]=&near_thing;state.search.count=found?3:1;state.search.calls=0;pointer_assigns=0;
bool scan=time>=1&&(!present||mask==0)&&!moving;bool expected=scan&&found;bool result=state.IsRunnable();assert(result==expected);assert(state.search.calls==(scan?1:0));assert(state.our_radius==(scan?1.0f:-1.0f));assert(state.their_radius==(expected?0.5f:-2.0f));assert(state.PThingCreature.Get()==(expected?&near_thing:0));assert(pointer_assigns==(expected?1:0));assert(pointer_live==1);
if(time==0||!present)assert(state.creature.modes.calls==0);else if(mask==0)assert(state.creature.modes.calls==7);else{int first=0;while((mask&(1<<first))==0)++first;assert(state.creature.modes.calls==first+1);}
}assert(pointer_live==0);
}
puts("00941fc0_TEST PASS");return 0;}