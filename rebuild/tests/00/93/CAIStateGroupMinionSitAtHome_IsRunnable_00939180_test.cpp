#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#define __int64 int __attribute__((mode(DI)))
#endif
#include <string.h>
extern "C" int puts(const char*);
int main();
// 00939180 CAIStateGroup_MinionSitAtHome::IsRunnable -- consult idle scheduler then set sitting deadline
// Leaf: ?IsRunnable@CAIStateGroup_MinionSitAtHome@@MAE_NXZ
// Flags:
#define CClass CClassLayout
#include "engine/CClass.h"
#undef CClass
#define CThing CThingLayout
#define CTCGraphicAppearance CTCGraphicAppearanceLayout
#include "engine/_quarantine/CThing.h"
#undef CThing
#undef CTCGraphicAppearance
#define CThingPhysical CThingPhysicalLayout
#include "engine/CThingPhysical.h"
#undef CThingPhysical
#define CThingGameObject CThingGameObjectLayout
#include "engine/CThingGameObject.h"
#undef CThingGameObject
#define CThingCreatureBase CThingCreatureBaseLayout
#include "engine/CThingCreatureBase.h"
#undef CThingCreatureBase
#define CThingAICreature CThingAICreatureLayout
#include "engine/CThingAICreature.h"
#undef CThingAICreature

#pragma pack(push,1)
class C3DVector {
public:float X,Y,Z;
    void Rotate(float,float);
    C3DVector(){}
    C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
    C3DVector(const C3DVector&v):X(v.X),Y(v.Y),Z(v.Z){}
    const C3DVector operator+(const C3DVector&v)const{return C3DVector(X+v.X,Y+v.Y,Z+v.Z);}
    const C3DVector operator-(const C3DVector&v)const{return C3DVector(X-v.X,Y-v.Y,Z-v.Z);}
};
#pragma pack(pop)
namespace std {
template<class A,class R>struct unary_function {typedef A argument_type;typedef R result_type;};
}
class CBaseClass {public:virtual~CBaseClass();};
class CBaseObject:public CBaseClass {
public:virtual void Validate()const;
protected:virtual~CBaseObject();
private:unsigned char unobservedObjectState[4];
};
#pragma pack(push,1)
class CClass:public CBaseObject {private:unsigned char unobservedClassState[sizeof(CClassLayout)-sizeof(CBaseObject)];};
enum ETCInterfaceType{};class CTCBase;
namespace std{template<class T>class allocator{};template<class K,class V>struct pair{K first;V second;};template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};template<class T,class A>class _Vector_base{protected:T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:T*end(){return this->_M_finish;}};}
template<class T>class CArray:public std::vector<T>{};template<class K,class V>class CKeyPairCompareLess{};
template<class K,class V,class C=CKeyPairCompareLess<K,V> >class CVectorMap:public CArray<std::pair<K,V> >{private:C Compare;bool Dirty;public:std::pair<K,V>*LowerBound(const K&);};
class CThing:public CClass {
public:template<class T>__forceinline bool GetTC(T**out){
 unsigned long*bits=reinterpret_cast<unsigned long*>(reinterpret_cast<CThingLayout*>(this)->UsingComponents);
 if(bits[T::StatGetInterfaceType()>>5]&(1UL<<(T::StatGetInterfaceType()&31))){ETCInterfaceType type=T::StatGetInterfaceType();CVectorMap<ETCInterfaceType,CTCBase*>&map=*reinterpret_cast<CVectorMap<ETCInterfaceType,CTCBase*>*>(reinterpret_cast<CThingLayout*>(this)->Components);std::pair<ETCInterfaceType,CTCBase*>*it=map.LowerBound(type);if(it==map.end()||T::StatGetInterfaceType()<it->first)it=map.end();*out=static_cast<T*>(it->second);return true;}return false;
 }
 const C3DVector&GetPos()const;
    static unsigned long __fastcall StatGetTypeFlags(){return 0;}
    bool IsDead()const{return (reinterpret_cast<const CThingLayout*>(this)->CreatedBySerialisation[0]&1)!=0;}
    unsigned long GetTypeFlags()const{return reinterpret_cast<const CThingLayout*>(this)->ThingTypeFlags;}
private:unsigned char unobservedThingState[sizeof(CThingLayout)-sizeof(CClass)];
};
class CThingPhysical:public CThing {public:const C3DVector&PeekPos()const;
private:unsigned char unobservedPhysicalState[sizeof(CThingPhysicalLayout)-sizeof(CThing)];};
class CThingGameObject:public CThingPhysical {private:unsigned char unobservedGameObjectState[sizeof(CThingGameObjectLayout)-sizeof(CThingPhysical)];};
class CThingCreatureBase:public CThingGameObject {public:bool IsThingInFOV(const CThing&,bool)const;
private:unsigned char unobservedCreatureState[sizeof(CThingCreatureBaseLayout)-sizeof(CThingGameObject)];};
class CThingBuilding:public CThingGameObject {};
class CBaseIntelligentPointer {
public:virtual~CBaseIntelligentPointer();CBaseObject*GetPItem()const;
private:unsigned char unobservedPointerState[4];
};
class CThingAICreature:public CThingCreatureBase {
public:CThingBuilding*GetHomeBuilding()const {
    return static_cast<CThingBuilding*>(reinterpret_cast<const CBaseIntelligentPointer*>(reinterpret_cast<const CThingAICreatureLayout*>(this)->HomeBuilding)->GetPItem());
}
private:unsigned char unobservedAIState[sizeof(CThingAICreatureLayout)-sizeof(CThingCreatureBase)];
};
#pragma pack(pop)
#define CAIBrain CAIBrainLayout
#include "engine/CAIBrain.h"
#undef CAIBrain
#define CAIStateGroupBase CAIStateGroupBaseLayout
#include "engine/CAIStateGroupBase.h"
#undef CAIStateGroupBase
class CCreatureActionBase;template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{private:T*GetPointer()const{return static_cast<T*>(GetPItem());}public:T*Get()const{return GetPointer();}T*operator->()const{return GetPointer();}};
class CAIBrain;class CWorld;class CCharString;class CThingBuilding;
enum EScriptAIPriority {};enum EAIStateGroupType {};enum EAIActionReturn {};enum EAIActionRepeat {};enum EMovementType {};
#pragma pack(push,1)
class CAIStateGroupBase:public CBaseClass {
friend int ::main();
public:
    void Terminate();
    virtual void Initialise(CAIBrain&,CThingAICreature&,long,EScriptAIPriority,const CCharString&);
    virtual void OnActivate();virtual bool InitCreaturePosition();virtual bool AllowPriorityOverlap();
    virtual bool IsStillValid()const;virtual const CCharString*GetDebugText()const;
    virtual bool IsTimingCritical()const;virtual EAIStateGroupType GetStateGroupType()const;
protected:
    virtual void Cleanup();virtual void OnReplace();virtual bool IsRunnable();virtual void Update()=0;
    virtual void InitStateGroup(const CWorld&,CThingAICreature&);
    EAIActionReturn AIAction_MoveToPosProximity(EAIActionRepeat,C3DVector,float,bool&,EMovementType,bool,bool,bool,const CThing*,bool,float);

    unsigned long*GetPWorldSeed()const;CWorld&GetWorld()const;
    EAIActionReturn AIAction_MinionIdle();
    CAIBrain*PBrain;
    unsigned char unobservedGroupState[sizeof(CAIStateGroupBaseLayout)-8];
    CThingAICreature&GetCreature()const{return *reinterpret_cast<const CAIBrainLayout*>(PBrain)->Creature;}
};
#pragma pack(pop)
class CBaseClassNonCopyable:public CBaseClass{protected:virtual~CBaseClassNonCopyable();};
class CTCBase:public CBaseClassNonCopyable{private:unsigned char unobservedTC[8];};
class CTCIdleScheduler:public CTCBase{public:static ETCInterfaceType __fastcall StatGetInterfaceType(){return(ETCInterfaceType)211;}bool IsPreferredIdle(CAIStateGroupBase*);};
#define CWorld CWorldLayout
#include "engine/CWorld.h"
#undef CWorld
// Only the original static API is observed here; physical state is supplied by the imported layout.
class CWorld{public:static long __fastcall GetFrame();};
long __fastcall GFROR13(unsigned long);
inline unsigned long __fastcall GFRandom(unsigned long range,unsigned long*seed){*seed=*seed*0x24a1UL+0x24dfUL;*seed=GFROR13(*seed);if(range)return *seed%range;return 0;}
#define CAIStateGroup_MinionSitOnSeatBase CAIStateGroup_MinionSitOnSeatBaseLayout
#include "engine/CAIStateGroup_MinionSitOnSeatBase.h"
#undef CAIStateGroup_MinionSitOnSeatBase
#define CAIStateGroup_MinionSitAtHome CAIStateGroup_MinionSitAtHomeLayout
#include "engine/_quarantine/CAIStateGroup_MinionSitAtHome.h"
#undef CAIStateGroup_MinionSitAtHome
class CAIStateGroup_UsesSeat:public CAIStateGroupBase{private:unsigned char unobservedUsesSeat[52-sizeof(CAIStateGroupBase)];};
class CAIStateGroup_MinionSitOnSeatBase:public CAIStateGroup_UsesSeat{
private:unsigned char unobservedSeatBase[sizeof(CAIStateGroup_MinionSitOnSeatBaseLayout)-sizeof(CAIStateGroup_UsesSeat)];
protected:virtual bool IsRunnable();
};
class CAIStateGroup_MinionSitAtHome:public CAIStateGroup_MinionSitOnSeatBase{
friend int ::main();
protected:long FrameToStopSitting;virtual bool IsRunnable();
};
bool CAIStateGroup_MinionSitAtHome::IsRunnable(){
 CTCIdleScheduler*scheduler;if(GetCreature().GetTC(&scheduler)&&!scheduler->IsPreferredIdle(this))return false;
 CWorld&world=GetWorld();unsigned long delay=GFRandom(150,GetPWorldSeed());
 FrameToStopSitting=world.GetFrame()+delay+225;
 return CAIStateGroup_MinionSitOnSeatBase::IsRunnable();
}

static int events[12],count,errors;static bool preferred,baseResult;static CTCIdleScheduler*expectedScheduler;static const CAIStateGroupBase*group;
static CWorld worldObject;static unsigned long*seedPointer;static unsigned long expectedLCG,expectedRotated,expectedDeadline;static long frame;
static void record(int n){if(count>=12){++errors;return;}events[count++]=n;}
CBaseClass::~CBaseClass(){}CBaseClassNonCopyable::~CBaseClassNonCopyable(){}CBaseObject::~CBaseObject(){}void CBaseObject::Validate()const{}CBaseIntelligentPointer::~CBaseIntelligentPointer(){}
template<class K,class V,class C>std::pair<K,V>*CVectorMap<K,V,C>::LowerBound(const K&key){record(1);if(key!=(K)211)++errors;std::pair<K,V>*it=this->_M_start;while(it!=this->_M_finish&&it->first<key)++it;return it;}
bool CTCIdleScheduler::IsPreferredIdle(CAIStateGroupBase*p){record(2);if(this!=expectedScheduler||p!=group)++errors;return preferred;}
CWorld&CAIStateGroupBase::GetWorld()const{record(3);if(this!=group)++errors;return worldObject;}
unsigned long*CAIStateGroupBase::GetPWorldSeed()const{record(4);if(this!=group)++errors;return seedPointer;}
long __fastcall GFROR13(unsigned long value){record(5);if(value!=expectedLCG||*seedPointer!=expectedLCG)++errors;unsigned long result=0;for(int bit=0;bit<32;++bit)if(value&(1UL<<bit))result|=1UL<<((bit+19)%32);if(result!=expectedRotated)++errors;return (long)result;}
long __fastcall CWorld::GetFrame(){record(6);if(*seedPointer!=expectedRotated)++errors;return frame;}
bool CAIStateGroup_MinionSitOnSeatBase::IsRunnable(){record(7);unsigned long deadline;memcpy(&deadline,(const char*)this+60,4);if(this!=group||deadline!=expectedDeadline)++errors;return baseResult;}
void CAIStateGroupBase::Initialise(CAIBrain&,CThingAICreature&,long,EScriptAIPriority,const CCharString&){++errors;}
void CAIStateGroupBase::OnActivate(){++errors;}bool CAIStateGroupBase::InitCreaturePosition(){++errors;return false;}
bool CAIStateGroupBase::AllowPriorityOverlap(){++errors;return false;}bool CAIStateGroupBase::IsStillValid()const{++errors;return false;}
const CCharString*CAIStateGroupBase::GetDebugText()const{++errors;return 0;}bool CAIStateGroupBase::IsTimingCritical()const{++errors;return false;}
EAIStateGroupType CAIStateGroupBase::GetStateGroupType()const{++errors;return(EAIStateGroupType)0;}
void CAIStateGroupBase::Cleanup(){++errors;}void CAIStateGroupBase::OnReplace(){++errors;}bool CAIStateGroupBase::IsRunnable(){++errors;return false;}
void CAIStateGroupBase::InitStateGroup(const CWorld&,CThingAICreature&){++errors;}
struct Fixture:CAIStateGroup_MinionSitAtHome{virtual void Update(){++errors;}};
struct Guarded{unsigned long pre;Fixture item;unsigned long post;};
int main(){
 const long frames[8]={-1000,-1,0,1,0x7ffffe00L,0x7fffffffL,-2147483647L,100000};
 for(int present=0;present<2;++present)for(int pref=0;pref<2;++pref)for(int result=0;result<2;++result)for(int sample=0;sample<32;++sample)for(int fi=0;fi<8;++fi){
  Guarded guard;Fixture&o=guard.item;guard.pre=0x12345678UL;guard.post=0xabcdef98UL;CThingAICreature creature;CTCIdleScheduler scheduler;CAIBrainLayout brain;
  memset(&brain,0x35,sizeof(brain));brain.Creature=&creature;o.PBrain=reinterpret_cast<CAIBrain*>(&brain);o.FrameToStopSitting=1234;
  CThingLayout*t=reinterpret_cast<CThingLayout*>(&creature);unsigned long*bits=(unsigned long*)t->UsingComponents;memset(bits,0,36);if(present)bits[6]|=1UL<<19;
  std::pair<ETCInterfaceType,CTCBase*>entries[4];entries[0].first=(ETCInterfaceType)210;entries[0].second=0;entries[1].first=(ETCInterfaceType)211;entries[1].second=&scheduler;entries[2].first=(ETCInterfaceType)212;entries[2].second=0;entries[3]=entries[2];void*map[3]={entries,entries+3,entries+4};memcpy(t->Components,map,12);
  unsigned long seedGuard[3]={0x12345678UL,0,0xabcdef98UL};unsigned long initial=sample==0?0:sample==1?1:sample==2?0xffffffffUL:sample==3?0x80000000UL:(unsigned long)sample*0x7654321UL;seedGuard[1]=initial;seedPointer=seedGuard+1;
  expectedLCG=(unsigned long)((unsigned __int64)initial*9377+9439);expectedRotated=(expectedLCG>>13)|(expectedLCG<<19);frame=frames[fi];expectedDeadline=(unsigned long)frame+(expectedRotated%150)+225;
  expectedScheduler=&scheduler;group=&o;preferred=pref!=0;baseResult=result!=0;bool allowed=!present||pref;
  unsigned char savedMode[sizeof(o)],want[sizeof(o)],savedThing[sizeof(creature)],savedBrain[sizeof(brain)],savedEntries[sizeof(entries)];memcpy(savedMode,&o,sizeof(o));memcpy(want,&o,sizeof(o));memcpy(savedThing,&creature,sizeof(creature));memcpy(savedBrain,&brain,sizeof(brain));memcpy(savedEntries,entries,sizeof(entries));if(allowed)memcpy(want+60,&expectedDeadline,4);
  count=errors=0;bool got=o.IsRunnable();int expected[7],n=0;if(present){expected[n++]=1;expected[n++]=2;}if(allowed){expected[n++]=3;expected[n++]=4;expected[n++]=5;expected[n++]=6;expected[n++]=7;}
  if(errors||got!=(allowed&&result)||count!=n||memcmp(events,expected,n*sizeof(int)))return 1;
  if(seedGuard[1]!=(allowed?expectedRotated:initial)||seedGuard[0]!=0x12345678UL||seedGuard[2]!=0xabcdef98UL)return 2;
  if(memcmp(want,&o,sizeof(o))||memcmp(savedThing,&creature,sizeof(creature))||memcmp(savedBrain,&brain,sizeof(brain))||memcmp(savedEntries,entries,sizeof(entries))||guard.pre!=0x12345678UL||guard.post!=0xabcdef98UL)return 3;
 }
 const unsigned long ranges[5]={0,1,149,150,0xffffffffUL};
 for(int sample=0;sample<32;++sample)for(int ri=0;ri<5;++ri){
  unsigned long seed=(unsigned long)sample*0x89abcdefUL;expectedLCG=(unsigned long)((unsigned __int64)seed*9377+9439);expectedRotated=(expectedLCG>>13)|(expectedLCG<<19);seedPointer=&seed;
  count=errors=0;unsigned long got=GFRandom(ranges[ri],&seed);unsigned long want=ranges[ri]?expectedRotated%ranges[ri]:0;
  if(errors||count!=1||events[0]!=5||seed!=expectedRotated||got!=want)return 4;
 }
 puts("00939180_TEST PASS");return 0;
}