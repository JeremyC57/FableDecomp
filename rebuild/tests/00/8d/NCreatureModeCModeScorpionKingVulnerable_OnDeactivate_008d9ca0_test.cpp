#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#define __int64 int __attribute__((mode(DI)))
#endif
#include <string.h>
#include <new>
extern "C" int puts(const char*);
int main();
// 008d9ca0 NCreatureMode::CModeScorpionKingVulnerable::OnDeactivate -- restore hit locations and release effect things
// Leaf: ?OnDeactivate@CModeScorpionKingVulnerable@NCreatureMode@@UAEXXZ
// Flags:
#define CClass CClassLayout
#include "engine/CClass.h"
#undef CClass
#define CThing CThingLayout
#define CTCGraphicAppearance CTCGraphicAppearanceLayout
#include "engine/_quarantine/CThing.h"
#undef CTCGraphicAppearance
#undef CThing
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
#define CModeStandBase CModeStandBaseLayout
#include "engine/CModeStandBase.h"
#undef CModeStandBase
#define CTCPhysicsBase CTCPhysicsBaseLayout
#include "engine/CTCPhysicsBase.h"
#undef CTCPhysicsBase
#define CCharString CCharStringLayout
#define CCharStringData CCharStringDataLayout
#include "fable_string.h"
#undef CCharStringData
#undef CCharString
class CCharStringData;
class CCharString {friend class CTCHitLocations;protected:CCharStringData*PStringData;public:CCharString(const char*,long=-1);~CCharString();};
class CBaseObjectPointer;class CTCBase;
class CBaseClass {public:virtual ~CBaseClass();};
class CBaseClassNonCopyable:public CBaseClass {protected:virtual ~CBaseClassNonCopyable();};
class CBaseObject:public CBaseClass {private:unsigned char unobservedObjectPointer[4];protected:virtual ~CBaseObject();};
#pragma pack(push,1)
class CClass:public CBaseObject {
public:
    // Layout: ClassListPos and TypeListPos are unread.
    unsigned char unobservedClassState[9];
};
#pragma pack(pop)
enum ETCInterfaceType {};
namespace std {
template<class T>class allocator {};
template<class K,class V>struct pair{K first;V second;};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
template<class T,class A>class _Vector_base{protected:T*_M_start;T*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:T*begin(){return this->_M_start;}T*end(){return this->_M_finish;}unsigned int size()const{return (unsigned int)(this->_M_finish-this->_M_start);}T&operator[](unsigned int n){return *(begin()+n);}};
}
template<class T>class CArray:public std::vector<T>{};
template<class K,class V>class CKeyPairCompareLess{};
template<class K,class V,class C=CKeyPairCompareLess<K,V> >class CVectorMap:public CArray<std::pair<K,V> > {
private:C Compare;bool Dirty;public:std::pair<K,V>*LowerBound(const K&);
};
#pragma pack(push,1)
class CThing:public CClass {
public:
    // Layout: all Thing fields are accessed through the imported physical layout only.
    unsigned char unobservedThingState[sizeof(CThingLayout)-sizeof(CClass)];
    bool IsDead()const{return (reinterpret_cast<const CThingLayout*>(this)->CreatedBySerialisation[0]&1)!=0;}
    void Kill(bool);
    template<class T>__forceinline bool GetTC(T**out) {
        const unsigned long*bits=reinterpret_cast<const unsigned long*>(reinterpret_cast<const CThingLayout*>(this)->UsingComponents);
        if(bits[T::StatGetInterfaceType()>>5]&(1UL<<(T::StatGetInterfaceType()&31))) {
            ETCInterfaceType type=T::StatGetInterfaceType();
            CVectorMap<ETCInterfaceType,CTCBase*>&map=*reinterpret_cast<CVectorMap<ETCInterfaceType,CTCBase*>*>(reinterpret_cast<CThingLayout*>(this)->Components);
            std::pair<ETCInterfaceType,CTCBase*>*it=map.LowerBound(type);
            if(it==map.end()||T::StatGetInterfaceType()<it->first)it=map.end();
            *out=(T*)it->second;return true;
        }
        return false;
    }
};
class CThingPhysical:public CThing {public:unsigned char unobservedPhysicalState[sizeof(CThingPhysicalLayout)-sizeof(CThing)];};
class CThingGameObject:public CThingPhysical {public:unsigned char unobservedGameObjectState[sizeof(CThingGameObjectLayout)-sizeof(CThingPhysical)];};
class CCreatureActionBase;
class CThingCreatureBase:public CThingGameObject {public:
    unsigned char unobservedCreatureState[sizeof(CThingCreatureBaseLayout)-sizeof(CThingGameObject)];
};
class CThingAICreature:public CThingCreatureBase {public:
    unsigned char unobservedAIState[sizeof(CThingAICreatureLayout)-sizeof(CThingCreatureBase)];
    void SuspendAI(bool,void*);void ForceAIReset();
};
#pragma pack(pop)
class CTCBase:public CBaseClassNonCopyable {public:
    // Layout: ParentThing and component flags are unread.
    unsigned char unobservedTCState[8];
};
class CTCCreatureModeManager:public CTCBase {
public:static ETCInterfaceType __fastcall StatGetInterfaceType(){return (ETCInterfaceType)0x31;}void RemoveMode(unsigned long);
    class CModeBase {
friend int ::main();
    public:virtual ~CModeBase();
        // Layout: ParentThing, WeaponBasedFilter, removal timer and active flag are unread.
        // Layout: original reference/filter/timer prefix is unread; only protected Active at24 is used.
    private:unsigned char unobservedModePrefix[20];
    protected:bool Active;
    public:
        virtual void OnDeactivate(){Active=false;}
    };
};

class CModeStandBase:public CTCCreatureModeManager::CModeBase {public:
    // Layout: all stand movement counters, speeds and animation pointers are unread.
    unsigned char unobservedStandState[0x68-28];
};
class CTCPhysicsBase:public CTCBase {
friend int ::main();public:
    // Layout: position, old position and velocity precede the observed radius.
    unsigned char unobservedPhysicsState[0x34-12];
protected:float Radius;
public:void SetRadius(float radius){Radius=radius;}
    // Layout: definition pointer and movement flags are unread.
    unsigned char unobservedPhysicsTail[8];
};
#pragma pack(push,1)
class CBaseIntelligentPointer {
friend int ::main();private:CBaseObjectPointer*PData;public:virtual ~CBaseIntelligentPointer();CBaseObject*GetPItem()const;};
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer {public:
    T*Get()const{return (T*)CBaseIntelligentPointer::GetPItem();}
    T*operator->()const{return Get();}
};
#pragma pack(pop)
template<class T>__forceinline bool __fastcall IS_THING_ALIVE(const T&p){return p.Get()!=0&&!p.Get()->IsDead();}
namespace NHitLocations {class CHitLocation {public:void SetEnabled(bool);};}
class CTCHitLocations:public CTCBase {public:
    static ETCInterfaceType __fastcall StatGetInterfaceType(){return (ETCInterfaceType)0x7f;}
    NHitLocations::CHitLocation*GetPHitLocationWithDefName(const CCharString&);
};
namespace NCreatureMode {
class CModeScorpionKingVulnerable:public CModeStandBase {
friend int ::main();private:
    long RecoveryCount;
    // Layout: the original twelve-byte animation filter is unread.
    unsigned char unobservedModeFilter[12];CThingCreatureBase*PCreature;bool bRecovering;CIntelligentPointer<CThingCreatureBase>PTarget;
    std::vector<CIntelligentPointer<CThing> >Obstructions;CIntelligentPointer<CThing>PEyeEmitter,PStingEmitter;
    long FrameCount;bool EffectsDone;long TailEffectIndex;float CollisionRadius;bool TailCurlFinished;
public:
    virtual void OnDeactivate();
};
void CModeScorpionKingVulnerable::OnDeactivate(){
    CTCHitLocations*hitLocations=0;PCreature->GetTC(&hitLocations);
    NHitLocations::CHitLocation*belly=hitLocations->GetPHitLocationWithDefName("HIT_LOCATION_SCORPION_KING_BELLY");
    NHitLocations::CHitLocation*body=hitLocations->GetPHitLocationWithDefName("HIT_LOCATION_SCORPION_KING");
    belly->SetEnabled(false);body->SetEnabled(true);
    reinterpret_cast<CThingLayout*>(PCreature)->PhysicsTC->SetRadius(CollisionRadius);
    long count=(long)Obstructions.size();
    for(long i=0;i<count;++i)if(IS_THING_ALIVE(Obstructions[i]))Obstructions[i]->Kill(true);
    if(IS_THING_ALIVE(PEyeEmitter))PEyeEmitter->Kill(true);
    if(IS_THING_ALIVE(PStingEmitter))PStingEmitter->Kill(true);
    CTCCreatureModeManager::CModeBase::OnDeactivate();
}
}
FABLE_STATIC_ASSERT(sizeof(CModeStandBase)==sizeof(CModeStandBaseLayout));
FABLE_STATIC_ASSERT(sizeof(CTCPhysicsBase)==sizeof(CTCPhysicsBaseLayout));
FABLE_STATIC_ASSERT(sizeof(NCreatureMode::CModeScorpionKingVulnerable)==184);

struct FixtureStringData{unsigned long fixtureRefs;};
static FixtureStringData fixtureNames[2];static const char*fixtureText[2]={"HIT_LOCATION_SCORPION_KING_BELLY","HIT_LOCATION_SCORPION_KING"};
static unsigned int fixtureErrors=0,fixtureConstruct=0,fixtureDestroy=0,fixtureLookups=0,fixtureEnabled=0,fixtureMapCalls=0,fixtureKills=0;
static NHitLocations::CHitLocation fixtureLocations[2];static NHitLocations::CHitLocation*fixtureBody=0;static bool fixtureLocationState[2];
static CThing*fixtureThings=0;static unsigned int fixtureKillOrder[8];static const void*fixtureMap=0;static CTCHitLocations*fixtureComponent=0;
CBaseClass::~CBaseClass(){}CBaseClassNonCopyable::~CBaseClassNonCopyable(){}CBaseObject::~CBaseObject(){}CBaseIntelligentPointer::~CBaseIntelligentPointer(){}CTCCreatureModeManager::CModeBase::~CModeBase(){}
CBaseObject*CBaseIntelligentPointer::GetPItem()const{return reinterpret_cast<CBaseObject*>(PData);}
CCharString::CCharString(const char*p,long n){int index=fixtureConstruct<2?(int)fixtureConstruct:0;if(n!=-1||strcmp(p,fixtureText[index]))++fixtureErrors;PStringData=reinterpret_cast<CCharStringData*>(fixtureNames+index);++reinterpret_cast<FixtureStringData*>(PStringData)->fixtureRefs;++fixtureConstruct;}
CCharString::~CCharString(){--reinterpret_cast<FixtureStringData*>(PStringData)->fixtureRefs;++fixtureDestroy;}
NHitLocations::CHitLocation*CTCHitLocations::GetPHitLocationWithDefName(const CCharString&name){unsigned int index=fixtureLookups++;if(this!=fixtureComponent||index>1||name.PStringData!=reinterpret_cast<CCharStringData*>(fixtureNames+index)||fixtureConstruct!=index+1||fixtureDestroy!=index)++fixtureErrors;return index?fixtureBody:fixtureLocations;}
void NHitLocations::CHitLocation::SetEnabled(bool enabled){if(fixtureLookups!=2||fixtureDestroy!=2||enabled!=(fixtureEnabled==1))++fixtureErrors;if(this==fixtureLocations)fixtureLocationState[0]=enabled;else if(this==fixtureLocations+1)fixtureLocationState[1]=enabled;else ++fixtureErrors;++fixtureEnabled;}
template<class K,class V,class C>std::pair<K,V>*CVectorMap<K,V,C>::LowerBound(const K&key){++fixtureMapCalls;if(this!=fixtureMap||(int)key!=0x7f)++fixtureErrors;std::pair<K,V>*it=this->_M_start;while(it!=this->_M_finish&&it->first<key)++it;return it;}
void CThing::Kill(bool immediate){int id=-1;for(int i=0;i<8;++i)if(this==fixtureThings+i)id=i;if(id<0||!immediate||fixtureEnabled!=2||fixtureKills>=8){++fixtureErrors;return;}fixtureKillOrder[fixtureKills++]=(unsigned int)id;reinterpret_cast<CThingLayout*>(this)->CreatedBySerialisation[0]|=1;}
struct FixtureGuardedMode{unsigned int before;NCreatureMode::CModeScorpionKingVulnerable mode;unsigned int after;};
int main(){
 for(int i=0;i<2;++i)fixtureNames[i].fixtureRefs=0;
 unsigned int cases=0;
 for(int n=0;n<=6;++n){unsigned int configurations=1;for(int i=0;i<n;++i)configurations*=3;
  for(unsigned int config=0;config<configurations;++config)for(int eye=0;eye<3;++eye)for(int sting=0;sting<3;++sting)for(int hitAlias=0;hitAlias<2;++hitAlias){
   CThingCreatureBase creature;CTCPhysicsBase physics;CTCHitLocations component;CThing things[8];fixtureThings=things;CIntelligentPointer<CThing>obstructions[6];FixtureGuardedMode guarded;fixtureComponent=&component;fixtureBody=fixtureLocations+(!hitAlias);fixtureLocationState[0]=true;fixtureLocationState[1]=false;
   unsigned int pattern=config;int pointerIDs[6];bool dead[8];for(int i=0;i<8;++i){memset(things[i].unobservedThingState,0x5a,sizeof(things[i].unobservedThingState));dead[i]=false;}
   for(int i=0;i<n;++i){int state=pattern%3;pattern/=3;pointerIDs[i]=state?i:-1;dead[i]=state==1;obstructions[i].PData=reinterpret_cast<CBaseObjectPointer*>(state?things+i:0);}
   dead[6]=eye==1;dead[7]=sting==1;int eyeID=eye==0?-1:eye==1?6:n?(int)(config%(unsigned int)n):6;int stingID=sting==0?-1:sting==1?7:n?n-1:7;
   for(int i=0;i<8;++i)if(dead[i])reinterpret_cast<CThingLayout*>(things+i)->CreatedBySerialisation[0]|=1;
   CThingLayout*physical=reinterpret_cast<CThingLayout*>(&creature);memset(physical->UsingComponents,0,sizeof(physical->UsingComponents));reinterpret_cast<unsigned long*>(physical->UsingComponents)[3]=0x80000000UL;
   std::pair<ETCInterfaceType,CTCBase*>entries[3];for(int i=0;i<3;++i){entries[i].first=(ETCInterfaceType)(0x7e + i);entries[i].second=&component;}std::pair<ETCInterfaceType,CTCBase*>*words[3]={entries,entries+3,entries+3};memcpy(physical->Components,words,sizeof(words));fixtureMap=physical->Components;physical->PhysicsTC=&physics;physics.Radius=-3.0f;
   guarded.before=0x12345678U;guarded.after=0xfedcba98U;guarded.mode.PCreature=&creature;guarded.mode.Active=true;guarded.mode.CollisionRadius=(float)(config%17+1)/4.0f;
   CIntelligentPointer<CThing>*vectorWords[3]={obstructions,obstructions+n,obstructions+6};memcpy(&guarded.mode.Obstructions,vectorWords,sizeof(vectorWords));guarded.mode.PTarget.PData=0;guarded.mode.PEyeEmitter.PData=reinterpret_cast<CBaseObjectPointer*>(eyeID<0?0:things+eyeID);guarded.mode.PStingEmitter.PData=reinterpret_cast<CBaseObjectPointer*>(stingID<0?0:things+stingID);
   unsigned int expectedOrder[8],expectedKills=0;bool afterDead[8];memcpy(afterDead,dead,sizeof(dead));for(int i=0;i<n+2;++i){int id=i<n?pointerIDs[i]:i==n?eyeID:stingID;if(id>=0&&!afterDead[id]){expectedOrder[expectedKills++]=(unsigned int)id;afterDead[id]=true;}}
   unsigned char expectedThings[sizeof(things)],expectedMode[sizeof(guarded.mode)],expectedPhysics[sizeof(physics)],beforeCreature[sizeof(creature)];memcpy(expectedThings,things,sizeof(things));memcpy(expectedMode,&guarded.mode,sizeof(guarded.mode));memcpy(expectedPhysics,&physics,sizeof(physics));memcpy(beforeCreature,&creature,sizeof(creature));
   for(int i=0;i<8;++i)if(afterDead[i])reinterpret_cast<CThingLayout*>(expectedThings+i*sizeof(CThing))->CreatedBySerialisation[0]|=1;
   unsigned int activeOffset=(unsigned int)(reinterpret_cast<unsigned char*>(&guarded.mode.Active)-reinterpret_cast<unsigned char*>(&guarded.mode));expectedMode[activeOffset]=0;unsigned int radiusOffset=(unsigned int)(reinterpret_cast<unsigned char*>(&physics.Radius)-reinterpret_cast<unsigned char*>(&physics));memcpy(expectedPhysics+radiusOffset,&guarded.mode.CollisionRadius,sizeof(float));
   fixtureErrors=fixtureConstruct=fixtureDestroy=fixtureLookups=fixtureEnabled=fixtureMapCalls=fixtureKills=0;guarded.mode.NCreatureMode::CModeScorpionKingVulnerable::OnDeactivate();
   if(fixtureErrors||fixtureConstruct!=2||fixtureDestroy!=2||fixtureLookups!=2||fixtureEnabled!=2||fixtureMapCalls!=1||fixtureKills!=expectedKills||memcmp(fixtureKillOrder,expectedOrder,expectedKills*sizeof(unsigned int)))return 1;
   if(fixtureLocationState[0]!=(hitAlias?true:false)||fixtureLocationState[1]!=(hitAlias?false:true)||fixtureNames[0].fixtureRefs||fixtureNames[1].fixtureRefs)return 2;
   if(memcmp(expectedThings,things,sizeof(things))||memcmp(expectedMode,&guarded.mode,sizeof(guarded.mode))||memcmp(expectedPhysics,&physics,sizeof(physics))||memcmp(beforeCreature,&creature,sizeof(creature))||guarded.before!=0x12345678U||guarded.after!=0xfedcba98U)return 3;
   ++cases;
  }
 }
 if(cases!=19674)return 4;puts("008d9ca0_TEST PASS");return 0;
}