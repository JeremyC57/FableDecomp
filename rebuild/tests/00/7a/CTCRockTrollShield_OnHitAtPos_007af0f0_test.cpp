// Self-contained behavioral fixture. The target body below is unchanged.
// Unused interfaces/layouts reduced; original native dispatch/offsets audited separately.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
#define __declspec(x)
#endif
#define CHECK(x)do{if(!(x)){fprintf(stderr,"CHECK line %d\n",__LINE__);abort();}}while(0)
static int positionCalls,facingCalls,subdefCalls,effectCalls,creates,stringCtors,stringDtors,deletes;
extern unsigned long GFastOneOverSqrtTable[128];
extern "C" double __cdecl cos(double);
#pragma intrinsic(cos)
__forceinline float __fastcall GFFastOneOverSqrt(float value){unsigned int bits;memcpy(&bits,&value,4);unsigned int estimate=(((0x17c-((bits>>23)&0xff))>>1)<<23)|(GFastOneOverSqrtTable[(bits>>17)&0x7f]<<15);float r;memcpy(&r,&estimate,4);return (3.0f-r*r*value)*r*0.5f;}
class C3DVector{public:float X,Y,Z;C3DVector(){}C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}C3DVector(const C3DVector&v):X(v.X),Y(v.Y),Z(v.Z){}
__forceinline const C3DVector&operator*=(float scale){X*=scale;Y*=scale;Z*=scale;return *this;}
__forceinline C3DVector&Normalise(){float factor=GFFastOneOverSqrt(X*X+Y*Y+Z*Z);X*=factor;Y*=factor;Z*=factor;return *this;}
__forceinline const float Dot(const C3DVector&b)const{float r=Y*b.Y;r+=Z*b.Z;r+=X*b.X;return r;}
};class CRightHandedSet{protected:C3DVector Up,Forward;};

unsigned long GFastOneOverSqrtTable[128]={106,103,100,98,95,92,90,87,85,83,80,78,76,74,72,70,68,66,64,62,60,58,56,54,52,51,49,47,46,44,42,41,39,38,36,35,33,32,31,29,28,26,25,24,22,21,20,19,17,16,15,14,13,12,10,9,8,7,6,5,4,3,2,1,255,252,248,244,240,237,233,230,226,223,220,217,214,211,208,205,202,199,196,193,191,188,185,183,180,178,176,173,171,168,166,164,162,160,158,155,153,151,149,147,145,143,142,140,138,136,134,133,131,129,127,126,124,122,121,119,118,116,115,113,112,110,109,107};

class CCharString{public:const char*Text;CCharString(const char*p,long n=-1):Text(p){CHECK(n==-1);++stringCtors;}~CCharString(){++stringDtors;}};
class CDefPointeeBase{public:unsigned long RefCount;CDefPointeeBase():RefCount(0){}virtual~CDefPointeeBase(){}virtual void Delete()const{CHECK(RefCount==0);++deletes;}void AddRef(){++RefCount;}unsigned long Release(){return --RefCount;}unsigned long GetRefCount()const{return RefCount;}};
enum ENullDef{NULL_DEF};
template<class T>class CDefPointer{public:CDefPointeeBase*Object;CDefPointer(ENullDef=NULL_DEF):Object(0){}~CDefPointer(){Release();}T*Get()const{return static_cast<T*>(Object);}T*operator->()const{return Get();}void Release(){if(Object){CDefPointeeBase*object=Object;if(object->Release()==0)object->Delete();Object=0;}}};
class CSpecialEffectsDef:public CDefPointeeBase{public:bool Present;bool GetEffectFromString(const CCharString&name,long&out)const{CHECK(strcmp(name.Text,"GET_HIT_BY_ARROW")==0);++effectCalls;if(!Present)return false;out=73;return true;}};
static CSpecialEffectsDef*effectDef;
class CThingBaseDef{public:float MeshHeight;template<class T>bool GetSubDef(CDefPointer<const T>*out)const{++subdefCalls;CHECK(!out->Object);if(!effectDef)return false;out->Object=effectDef;out->Object->AddRef();return true;}};
class CTCPhysicsBase{public:C3DVector Facing;virtual~CTCPhysicsBase(){}virtual C3DVector GetFacingVector()const{++facingCalls;return Facing;}};
enum ETCInterfaceType{TCI_SHOT_DEFINITION=119};
class CThing{public:bool Shot;C3DVector Position;CThingBaseDef*BaseDef;CTCPhysicsBase*PhysicsTC;const C3DVector&GetPos()const{++positionCalls;return Position;}const CThingBaseDef&PeekBaseDef()const{return *BaseDef;}bool HasTC(ETCInterfaceType k)const{CHECK(k==TCI_SHOT_DEFINITION);return Shot;}};
class CTCBase{protected:CThing&ParentThing;public:CTCBase(CThing&t):ParentThing(t){}virtual~CTCBase(){}};
class CTCRockTrollShield:public CTCBase{public:long health,rechargeCounter;CTCRockTrollShield(CThing&t):CTCBase(t),health(0),rechargeCounter(0){}void OnHitAtPos(CThing*,const C3DVector&,const C3DVector&,long,bool&);};
static C3DVector createdAt;static const C3DVector*expectedPosition;
class CTCDParticleEmitter{public:static CThing*__fastcall Create(long id,const C3DVector&pos,bool flag){CHECK(id==73&&!flag&&&pos==expectedPosition);createdAt=pos;++creates;return 0;}};
void CTCRockTrollShield::OnHitAtPos(CThing*attacker,const C3DVector&pos,const C3DVector&direction,long damage,bool&blocked){
 if(health==0)return;
 if(!attacker||!attacker->HasTC(static_cast<ETCInterfaceType>(119))){blocked=true;return;}
 const C3DVector&parentPos=ParentThing.GetPos();C3DVector hitPos(pos);hitPos.X-=parentPos.X;hitPos.Y-=parentPos.Y;hitPos.Z-=parentPos.Z;float hitHeightRatio=hitPos.Z/ParentThing.PeekBaseDef().MeshHeight;
 if(hitHeightRatio>0.5f){
  C3DVector toward(direction.X,direction.Y,0.0f);toward.Normalise();toward*=-1.0f;
  if(toward.Dot(ParentThing.PhysicsTC->GetFacingVector())>cos(1.0471976115464443)){
   health+=damage;CDefPointer<const CSpecialEffectsDef>effects;long effect=-1;
   bool found=ParentThing.PeekBaseDef().GetSubDef(&effects)&&effects->GetEffectFromString("GET_HIT_BY_ARROW",effect);
   if(found)CTCDParticleEmitter::Create(effect,pos,false);
  }
 }
 if(health<=0){health=5;rechargeCounter=0;}else blocked=true;
}

int main(){int cases=0;const long healthValues[]={0,2,-2};const long damageValues[]={-4,0,3};const float heights[]={0.0f,0.5f,0.75f};
 for(int h=0;h<3;++h)for(int weapon=0;weapon<3;++weapon)for(int height=0;height<3;++height)for(int direction=0;direction<4;++direction)for(int damage=0;damage<3;++damage)for(int effect=0;effect<3;++effect)for(int initial=0;initial<2;++initial){
  CThing owner,attacker;CThingBaseDef base;CTCPhysicsBase physics;CSpecialEffectsDef def;CTCRockTrollShield shield(owner);
  owner.Position=C3DVector(8,-4,2);owner.BaseDef=&base;owner.PhysicsTC=&physics;base.MeshHeight=4;
  physics.Facing=C3DVector(-1,0,0);attacker.Shot=weapon==2;
  C3DVector pos(16,8,owner.Position.Z+heights[height]*base.MeshHeight);
  C3DVector dir=direction==0?C3DVector(1,0,8):(direction==1?C3DVector(-1,0,4):(direction==2?C3DVector(0,1,-4):C3DVector(2,0,-8)));
  shield.health=healthValues[h];shield.rechargeCounter=19;bool blocked=initial!=0;
  effectDef=effect?&def:0;def.Present=effect==2;expectedPosition=&pos;
  positionCalls=facingCalls=subdefCalls=effectCalls=creates=stringCtors=stringDtors=deletes=0;
  shield.OnHitAtPos(weapon?&attacker:0,pos,dir,damageValues[damage],blocked);
  bool active=h!=0,valid=weapon==2,high=height==2,hit=direction==0||direction==3;
  long expectedHealth=healthValues[h],expectedRecharge=19;bool expectedBlocked=initial!=0;
  if(active){if(!valid)expectedBlocked=true;else{if(high&&hit)expectedHealth+=damageValues[damage];if(expectedHealth<=0){expectedHealth=5;expectedRecharge=0;}else expectedBlocked=true;}}
  bool effectReached=active&&valid&&high&&hit;
  CHECK(shield.health==expectedHealth&&shield.rechargeCounter==expectedRecharge&&blocked==expectedBlocked);
  CHECK(positionCalls==(active&&valid?1:0));CHECK(facingCalls==(active&&valid&&high?1:0));CHECK(subdefCalls==(effectReached?1:0));
  CHECK(effectCalls==(effectReached&&effect?1:0));CHECK(stringCtors==effectCalls&&stringDtors==effectCalls);CHECK(deletes==effectCalls);CHECK(def.RefCount==0);
  CHECK(creates==(effectReached&&effect==2?1:0));if(creates)CHECK(createdAt.X==pos.X&&createdAt.Y==pos.Y&&createdAt.Z==pos.Z);
  ++cases;
 }CHECK(cases==1944);puts("007af0f0_TEST PASS");return 0;
}