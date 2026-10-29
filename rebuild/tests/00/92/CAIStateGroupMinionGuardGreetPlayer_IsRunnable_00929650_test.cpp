#ifndef _MSC_VER
#define __int64 int __attribute__((mode(DI)))
#define __cdecl
#define __fastcall
#define __forceinline inline
#endif
#include <stddef.h>
#include <string.h>
extern "C" int puts(const char*);
// 00929650 CAIStateGroup_MinionGuardGreetPlayer::IsRunnable()
// Leaf: ?IsRunnable@CAIStateGroup_MinionGuardGreetPlayer@@UAE_NXZ
// Flags:
extern "C" double __cdecl sqrt(double);
#define CIVCountedPointeeBase CIVCountedPointeeBaseLayout
#include "engine/CIVCountedPointeeBase.h"
#undef CIVCountedPointeeBase
#define CWorldMap CWorldMapLayout
#include "engine/CWorldMap.h"
#undef CWorldMap
#define CPlayer CPlayerLayout
#include "engine/CPlayer.h"
#undef CPlayer
#define CWorld CWorldLayout
#include "engine/CWorld.h"
#undef CWorld

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

#define CAIBrain CAIBrainLayout
#include "engine/CAIBrain.h"
#undef CAIBrain
#define CAIStateGroupBase CAIStateGroupBaseLayout
#include "engine/CAIStateGroupBase.h"
#undef CAIStateGroupBase
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
enum ETCInterfaceType {};
class CTCBase;
namespace std {
template<class T>class allocator {};
template<class K,class V>struct pair{K first;V second;};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
template<class T,class A>class _Vector_base{protected:T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:T*end(){return this->_M_finish;}};
}
template<class T>class CArray:public std::vector<T>{};
template<class K,class V>class CKeyPairCompareLess{};
template<class K,class V,class C=CKeyPairCompareLess<K,V> >class CVectorMap:public CArray<std::pair<K,V> > {
private:C Compare;bool Dirty;
public:std::pair<K,V>*LowerBound(const K&);
};
class C3DVector;
class CThing:public CClass {
public:const C3DVector&GetPos()const;

    bool HasTC(ETCInterfaceType type)const {
        const unsigned long*bits=reinterpret_cast<const unsigned long*>(reinterpret_cast<const CThingLayout*>(this)->UsingComponents);
        if(bits[(unsigned)type>>5]&(1UL<<((unsigned)type&31)))return true;
        return false;
    }
    __forceinline CTCBase&GetTC(ETCInterfaceType type) {
        ETCInterfaceType key=type;
        CVectorMap<ETCInterfaceType,CTCBase*>&map=*reinterpret_cast<CVectorMap<ETCInterfaceType,CTCBase*>*>(reinterpret_cast<CThingLayout*>(this)->Components);
        std::pair<ETCInterfaceType,CTCBase*>*it=map.LowerBound(key);
        std::pair<ETCInterfaceType,CTCBase*>*last=map.end();
        return *(it==last||type<it->first?last:it)->second;
    }
    bool IsAlive()const{return ((unsigned char)~reinterpret_cast<const CThingLayout*>(this)->CreatedBySerialisation[0]&1)!=0;}
    bool IsDead()const{return (reinterpret_cast<const CThingLayout*>(this)->CreatedBySerialisation[0]&1)!=0;}
    unsigned long GetTypeFlags()const{return reinterpret_cast<const CThingLayout*>(this)->ThingTypeFlags;}
private:unsigned char unobservedThingState[sizeof(CThingLayout)-sizeof(CClass)];
};
class CThingPhysical:public CThing {private:unsigned char unobservedPhysicalState[sizeof(CThingPhysicalLayout)-sizeof(CThing)];};
class CThingGameObject:public CThingPhysical {private:unsigned char unobservedGameObjectState[sizeof(CThingGameObjectLayout)-sizeof(CThingPhysical)];};
class CThingCreatureBase:public CThingGameObject {
public:const C3DVector&PeekInitialPos()const{return *reinterpret_cast<const C3DVector*>(&reinterpret_cast<const CThingCreatureBaseLayout*>(this)->InitialPos_X);}
private:unsigned char unobservedCreatureState[sizeof(CThingCreatureBaseLayout)-sizeof(CThingGameObject)];};
class CThingAICreature:public CThingCreatureBase {
public:static unsigned long __fastcall StatGetTypeFlags(){return 31;}
private:unsigned char unobservedAIState[sizeof(CThingAICreatureLayout)-sizeof(CThingCreatureBase)];
};
#pragma pack(pop)


class CBaseIntelligentPointer {
public:CBaseIntelligentPointer();virtual~CBaseIntelligentPointer();CBaseObject*GetPItem()const;void SetPItem(CBaseObject*);
private:unsigned char unobservedPointerState[4];
};
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer {
public:CIntelligentPointer(){} CIntelligentPointer(T*item){SetPItem(const_cast<CBaseObject*>(static_cast<const CBaseObject*>(item)));}
};
#pragma pack(push,1)
class C3DVector {
public:float X,Y,Z;
    C3DVector(){}
    C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
    C3DVector(const C3DVector&v):X(v.X),Y(v.Y),Z(v.Z){}
    const C3DVector operator+(const C3DVector&v)const{return C3DVector(X+v.X,Y+v.Y,Z+v.Z);}
    const C3DVector operator-(const C3DVector&v)const{return C3DVector(X-v.X,Y-v.Y,Z-v.Z);}
};
#pragma pack(pop)

class CWorldMap;
class CCreatureActionBase;
class CAIBrain;class CWorld;class CCharString;class CThingBuilding;
enum EScriptAIPriority {};enum EAIStateGroupType {};enum EAIActionReturn {};enum EAIActionRepeat {};enum EMovementType {};
#pragma pack(push,1)
class CAIStateGroupBase:public CBaseClass {
public:
    void Terminate();
    virtual void Initialise(CAIBrain&,CThingAICreature&,long,EScriptAIPriority,const CCharString&);
    virtual void OnActivate();virtual bool InitCreaturePosition();virtual bool AllowPriorityOverlap();
    virtual bool IsStillValid()const;virtual const CCharString*GetDebugText()const;
    virtual bool IsTimingCritical()const;virtual EAIStateGroupType GetStateGroupType()const;
protected:
    virtual void Cleanup();virtual void OnReplace();virtual bool IsRunnable();virtual void Update()=0;
    virtual void InitStateGroup(const CWorld&,CThingAICreature&);
    const CWorldMap&PeekWorldMap()const;
    CAIBrain*PBrain;
    unsigned char unobservedGroupState[sizeof(CAIStateGroupBaseLayout)-8];
    CThingAICreature&GetCreature()const{return *reinterpret_cast<const CAIBrainLayout*>(PBrain)->Creature;}
};
#pragma pack(pop)
class CBaseClassNonCopyable:public CBaseClass {protected:virtual~CBaseClassNonCopyable();};
class CTCBase:public CBaseClassNonCopyable {private:unsigned char unobservedTCState[8];};
#define CResource CResourceLayout
#include "engine/CResource.h"
#undef CResource
#define CDefClassBase CDefClassBaseLayout
#include "engine/CDefClassBase.h"
#undef CDefClassBase
#define CParentDefClassBase CParentDefClassBaseLayout
#include "engine/CParentDefClassBase.h"
#undef CParentDefClassBase
#pragma pack(push,1)
class CIVCountedPointeeBase {
public:virtual~CIVCountedPointeeBase();virtual void Delete()const;
 void AddRef()const{++IVRefCount;}
 long Release()const{if(--IVRefCount==0){Delete();return 0;}return IVRefCount;}
private:mutable unsigned long IVRefCount;
};
typedef char VerifyCountedLayout[(sizeof(CIVCountedPointeeBase)==sizeof(CIVCountedPointeeBaseLayout))?1:-1];
class CResource:public CIVCountedPointeeBase {private:unsigned char unobservedResourceState[sizeof(CResourceLayout)-sizeof(CIVCountedPointeeBase)];};
class CDefPointeeBase:public CResource {};
class CDefClassBase:public CDefPointeeBase {private:unsigned char unobservedDefinitionState[sizeof(CDefClassBaseLayout)-sizeof(CDefPointeeBase)];};
class CParentDefClassBase:public CDefClassBase {private:unsigned char unobservedParentState[sizeof(CParentDefClassBaseLayout)-sizeof(CDefClassBase)];};
class CRegionDef:public CParentDefClassBase {
private:unsigned char unobservedRegionDefinition[0x9d-sizeof(CParentDefClassBase)];
public:bool UseCrimes;
};
#pragma pack(pop)
template<class T>class CDefPointer {
private:CDefPointeeBase*Object;
public:CDefPointer(const CDefPointer&p):Object(p.Object){if(Object)Object->AddRef();}
 explicit CDefPointer(T*p):Object(const_cast<CDefPointeeBase*>(static_cast<const CDefPointeeBase*>(p))){if(Object)Object->AddRef();}
 ~CDefPointer(){if(Object)Object->Release();}
 T*operator->()const{return static_cast<T*>(Object);}
 T*Get()const{return static_cast<T*>(Object);}
};
#define CRegion CRegionLayout
#include "engine/CRegion.h"
#undef CRegion
class CRegion {
public:CDefPointer<const CRegionDef>GetPRegionDef()const{return*reinterpret_cast<const CDefPointer<const CRegionDef>*>(&reinterpret_cast<const CRegionLayout*>(this)->PDef_Object);}
};
class CWorldMap {public:long GetLoadedRegion()const;const CRegion&GetRegion(long)const;};
#define CTCVillage CTCVillageLayout
#include "engine/CTCVillage.h"
#undef CTCVillage
class CTCVillage:public CTCBase {
public:float GetFrameForNextGuardGreeting()const{return(float)reinterpret_cast<const CTCVillageLayout*>(this)->FrameForNextGuardGreeting;}
};
class CTCVillageMember:public CTCBase {public:CTCVillage*GetPVillage()const;};
class CTCPerceiveThing:public CTCBase {public:const CThing*GetPerceivedThing()const;bool IsAwareOfThing()const;};
class CThingPlayerCreature:public CThingCreatureBase {};
class CPlayer {public:CThingPlayerCreature*GetPControlledCreature()const;};
class CWorld {public:static long __fastcall GetFrame();const CPlayer*PeekMyPlayer()const;};
namespace boost{template<class T>class scoped_ptr{private:T*ptr;public:T&operator*()const{return *ptr;}};}
class CDeviceResetCallback{public:virtual void OnPreDeviceReset()=0;virtual bool OnPostDeviceReset()=0;};
class CGame;
class CGameComponent:public CBaseClassNonCopyable,public CDeviceResetCallback{
private:unsigned char unobservedGameComponent[16-sizeof(CBaseClassNonCopyable)-sizeof(CDeviceResetCallback)];
public:explicit CGameComponent(CGame&);virtual ~CGameComponent();virtual void Init();virtual bool Run(CGameComponent**)=0;virtual void ChangeTextureColourDepth(long)=0;
protected:virtual void SetQuit();
};
class CMainGameComponent:public CGameComponent{
friend int main();
private:unsigned char unobservedBeforeWorld[0x24-sizeof(CGameComponent)];
protected:static CMainGameComponent*PMainGameComponent;boost::scoped_ptr<CWorld>PWorld;
 virtual void Update();virtual void GetInputs();virtual void Render();virtual void PostInit();
public:virtual ~CMainGameComponent();virtual void Init();virtual bool Run(CGameComponent**);virtual void ChangeTextureColourDepth(long);virtual void SetQuit();virtual bool IsEditorActive();virtual void OnPreDeviceReset();virtual bool OnPostDeviceReset();
 static CMainGameComponent*__fastcall Get(){return PMainGameComponent;}
 CWorld&GetWorld(){return *PWorld;}
};

class CAIStateGroup_MinionGuardGreetPlayer:public CAIStateGroupBase {public:virtual bool IsRunnable();};
bool CAIStateGroup_MinionGuardGreetPlayer::IsRunnable()
{
    CDefPointer<const CRegionDef>definition=PeekWorldMap().GetRegion(PeekWorldMap().GetLoadedRegion()).GetPRegionDef();
    if(definition.Get()!=0&&!definition->UseCrimes)return false;
    long nextGreeting=0x7fffffff;
    CThingAICreature&creature=GetCreature();
    if(creature.HasTC((ETCInterfaceType)35)){
        CTCVillageMember&member=static_cast<CTCVillageMember&>(creature.GetTC((ETCInterfaceType)35));
        if(member.GetPVillage()!=0)nextGreeting=(long)member.GetPVillage()->GetFrameForNextGuardGreeting();
    }
    if(CWorld::GetFrame()>nextGreeting){
        CThingPlayerCreature*hero=CMainGameComponent::Get()->GetWorld().PeekMyPlayer()->GetPControlledCreature();
        if(hero!=0&&!hero->IsDead()){
            const C3DVector&me=GetCreature().GetPos();
            const C3DVector&him=hero->GetPos();
            float dx=him.X-me.X,dy=him.Y-me.Y;
            if((float)sqrt(dx*dx+dy*dy)>5.0f)return false;
            CThingAICreature&self=GetCreature();
            if(self.HasTC((ETCInterfaceType)52)){
                CTCPerceiveThing&perceive=static_cast<CTCPerceiveThing&>(self.GetTC((ETCInterfaceType)52));
                const CThing*perceived=perceive.GetPerceivedThing();
                if(perceived!=0&&!perceived->IsDead()&&perceive.IsAwareOfThing())return true;
            }
        }
    }
    return false;
}

struct FixtureVectorAlignment{char marker;C3DVector value;};
typedef char VerifyVectorByteAlignment[(offsetof(FixtureVectorAlignment,value)==1)?1:-1];
CBaseClass::~CBaseClass(){}
CBaseObject::~CBaseObject(){}
void CAIStateGroupBase::Initialise(CAIBrain&,CThingAICreature&,long,EScriptAIPriority,const CCharString&){}
void CAIStateGroupBase::OnActivate(){}
bool CAIStateGroupBase::InitCreaturePosition(){return false;}
bool CAIStateGroupBase::AllowPriorityOverlap(){return false;}
bool CAIStateGroupBase::IsStillValid()const{return false;}
const CCharString*CAIStateGroupBase::GetDebugText()const{return 0;}
bool CAIStateGroupBase::IsTimingCritical()const{return false;}
EAIStateGroupType CAIStateGroupBase::GetStateGroupType()const{return EAIStateGroupType(0);}
void CAIStateGroupBase::Cleanup(){}
void CAIStateGroupBase::OnReplace(){}
bool CAIStateGroupBase::IsRunnable(){return false;}
void CAIStateGroupBase::InitStateGroup(const CWorld&,CThingAICreature&){}

void CBaseObject::Validate()const{}


static int bad,mapCalls,loadedCalls,regionCalls,frameCalls,playerCalls,heroCalls,lookupCalls,villageCalls,posCalls,perceivedCalls,awareCalls,deleteCalls;
static bool releaseExternal,awareAnswer;static long frameValue;
static CWorldMap*worldMap;static CRegion*region;static CRegionDef*definition;static CWorld*world;static CPlayer*player;static CThingPlayerCreature*heroPtr;
static CThingAICreature*creature;static CTCVillageMember*member;static CTCVillage*village;static CTCPerceiveThing*perception;static const CThing*perceivedPtr;static CAIStateGroupBase*group;
static C3DVector actorPosition,heroPosition;
static unsigned long refs(const CIVCountedPointeeBase*p){unsigned long value;memcpy(&value,reinterpret_cast<const unsigned char*>(p)+4,4);return value;}
static void setRefs(CIVCountedPointeeBase*p,unsigned long value){memcpy(reinterpret_cast<unsigned char*>(p)+4,&value,4);}
CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CIVCountedPointeeBase::~CIVCountedPointeeBase(){}
void CIVCountedPointeeBase::Delete()const{if(this!=definition||refs(this)!=0)bad=1;++deleteCalls;}
CMainGameComponent*CMainGameComponent::PMainGameComponent=0;
const CWorldMap&CAIStateGroupBase::PeekWorldMap()const{if(this!=group)bad=2;++mapCalls;return*worldMap;}
long CWorldMap::GetLoadedRegion()const{if(this!=worldMap)bad=3;++loadedCalls;return 7;}
const CRegion&CWorldMap::GetRegion(long index)const{if(this!=worldMap||index!=7||loadedCalls!=1)bad=4;++regionCalls;return*region;}
long __fastcall CWorld::GetFrame(){
 if(mapCalls!=2||regionCalls!=1||(definition&&refs(definition)!=2))bad=5;++frameCalls;
 if(releaseExternal&&definition)definition->Release();return frameValue;
}
const CPlayer*CWorld::PeekMyPlayer()const{if(this!=world||frameCalls!=1)bad=6;++playerCalls;return player;}
CThingPlayerCreature*CPlayer::GetPControlledCreature()const{if(this!=player||playerCalls!=1)bad=7;++heroCalls;return heroPtr;}
template<class K,class V,class C>std::pair<K,V>*CVectorMap<K,V,C>::LowerBound(const K&key){
 if(this!=reinterpret_cast<void*>(reinterpret_cast<CThingLayout*>(creature)->Components)||((int)key!=35&&(int)key!=52))bad=8;
 if((int)key==35&&lookupCalls)bad=9;if((int)key==52&&(posCalls!=2||frameCalls!=1))bad=10;
 ++lookupCalls;std::pair<K,V>*it=this->_M_start;while(it!=this->_M_finish&&it->first<key)++it;return it;
}
CTCVillage*CTCVillageMember::GetPVillage()const{if(this!=member||frameCalls)bad=11;++villageCalls;return village;}
const C3DVector&CThing::GetPos()const{++posCalls;if(posCalls==1){if(this!=creature)bad=12;return actorPosition;}if(this!=heroPtr||posCalls!=2)bad=13;return heroPosition;}
const CThing*CTCPerceiveThing::GetPerceivedThing()const{if(this!=perception||posCalls!=2)bad=14;++perceivedCalls;return perceivedPtr;}
bool CTCPerceiveThing::IsAwareOfThing()const{if(this!=perception||perceivedCalls!=1)bad=15;++awareCalls;return awareAnswer;}
class FixtureGroup:public CAIStateGroup_MinionGuardGreetPlayer {public:void attach(CAIBrainLayout&b){PBrain=reinterpret_cast<CAIBrain*>(&b);}virtual void Update(){}};
struct FixtureMapWords{std::pair<ETCInterfaceType,CTCBase*>*first,*last,*capacity;unsigned char tail[4];};
int main(){
 int cases=0;
 for(int defMode=0;defMode!=3;++defMode)for(int hasMember=0;hasMember!=2;++hasMember)for(int hasVillage=0;hasVillage!=2;++hasVillage)for(int time=0;time!=4;++time)for(int hero=0;hero!=3;++hero)for(int distance=0;distance!=3;++distance)for(int sensed=0;sensed!=4;++sensed)for(int aware=0;aware!=2;++aware)for(int drop=0;drop!=2;++drop){
  CThingAICreature actor;CThingPlayerCreature pc;CThing other;CTCVillageMember mem;CTCPerceiveThing perc;CWorldMap wm;CRegionLayout regionStorage;CRegionDef def;CWorld wrld;CPlayer pl;CTCVillageLayout villageStorage;CAIBrainLayout brain;brain.Creature=&actor;FixtureGroup candidate;candidate.attach(brain);
  union{double align;unsigned char bytes[0x28];}mainStorage;memset(mainStorage.bytes,0,sizeof mainStorage.bytes);
  world=&wrld;player=&pl;heroPtr=hero?&pc:0;creature=&actor;member=&mem;village=hasVillage?reinterpret_cast<CTCVillage*>(&villageStorage):0;perception=&perc;perceivedPtr=sensed>=2?&other:0;group=&candidate;worldMap=&wm;region=reinterpret_cast<CRegion*>(&regionStorage);definition=defMode?&def:0;
  CMainGameComponent::PMainGameComponent=reinterpret_cast<CMainGameComponent*>(mainStorage.bytes);memcpy(mainStorage.bytes+0x24,&world,4);
  def.UseCrimes=defMode==2;setRefs(&def,1);regionStorage.PDef_Object=definition;villageStorage.FrameForNextGuardGreeting=time==3?16777216:20;frameValue=time==3?16777217:19+time;
  CThingLayout*actorLayout=reinterpret_cast<CThingLayout*>(&actor);memset(actorLayout->UsingComponents,0,36);unsigned long mask=(hasMember?8UL:0)|(sensed?0x100000UL:0);reinterpret_cast<unsigned long*>(actorLayout->UsingComponents)[1]=mask;
  reinterpret_cast<CThingLayout*>(&pc)->CreatedBySerialisation[0]=hero==1?1:0;reinterpret_cast<CThingLayout*>(&other)->CreatedBySerialisation[0]=sensed==2?1:0;
  std::pair<ETCInterfaceType,CTCBase*>pairs[2];pairs[0].first=(ETCInterfaceType)35;pairs[0].second=&mem;pairs[1].first=(ETCInterfaceType)52;pairs[1].second=&perc;FixtureMapWords map;map.first=pairs;map.last=pairs+2;map.capacity=pairs+2;memset(map.tail,0,4);memcpy(actorLayout->Components,&map,16);
  actorPosition=C3DVector(0,0,-200);heroPosition=distance==0?C3DVector(0,4,900):(distance==1?C3DVector(3,4,900):C3DVector(6,8,900));awareAnswer=aware!=0;releaseExternal=drop!=0;
  bad=mapCalls=loadedCalls=regionCalls=frameCalls=playerCalls=heroCalls=lookupCalls=villageCalls=posCalls=perceivedCalls=awareCalls=deleteCalls=0;
  bool actual=candidate.IsRunnable();
  bool allowed=defMode!=1;bool ready=allowed&&hasMember&&hasVillage&&(time==2||time==3);bool live=ready&&hero==2;bool near=live&&distance<2;bool sensing=near&&sensed!=0;bool hasAlive=sensing&&sensed==3;bool expected=hasAlive&&aware;
  int released=defMode==2&&drop?1:0;
  if(bad||actual!=expected||mapCalls!=2||loadedCalls!=1||regionCalls!=1||frameCalls!=(int)allowed||playerCalls!=(int)ready||heroCalls!=(int)ready||lookupCalls!=((allowed&&hasMember)?1:0)+(sensing?1:0)||villageCalls!=((allowed&&hasMember)?(hasVillage?2:1):0)||posCalls!=(live?2:0)||perceivedCalls!=(int)sensing||awareCalls!=(int)hasAlive||deleteCalls!=released||refs(&def)!=(unsigned long)(1-released)||def.UseCrimes!=(defMode==2))return 1;
  ++cases;
 }
 puts("00929650_TEST PASS");return cases==6912?0:2;
}