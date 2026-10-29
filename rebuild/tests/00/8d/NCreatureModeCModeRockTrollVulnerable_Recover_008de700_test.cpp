#ifndef _MSC_VER
#define __declspec(x)
#define __fastcall
#define __forceinline inline
#define __int64 int __attribute__((mode(DI)))
#endif
#include <string.h>
extern "C" int puts(const char*);
int main();
// 008de700 NCreatureMode::CModeRockTrollVulnerable::Recover -- leave the vulnerable troll mode
// Leaf: ?Recover@CModeRockTrollVulnerable@NCreatureMode@@AAEXXZ
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
#define CCreatureActionBase CCreatureActionBaseLayout
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase
#define CCombatActionBase CCombatActionBaseLayout
#include "engine/_quarantine/CCombatActionBase.h"
#undef CCombatActionBase
class CBaseObjectPointer;class CTCBase;
class CBaseClass {public:virtual ~CBaseClass();};
class CBaseClassNonCopyable:public CBaseClass {protected:virtual ~CBaseClassNonCopyable();};
class CBaseObject:public CBaseClass {private:unsigned char unobservedObjectPointer[4];protected:virtual ~CBaseObject();};
#pragma pack(push,1)
class CClass:public CBaseObject {
friend int ::main();
private:
    // Layout: ClassListPos and TypeListPos are unread.
    unsigned char unobservedClassLists[8];unsigned char Type;
public:unsigned long GetType()const{return Type;}
};
#pragma pack(pop)
enum ETCInterfaceType {};
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
private:C Compare;bool Dirty;public:std::pair<K,V>*LowerBound(const K&);
};
#pragma pack(push,1)
class CThing:public CClass {
public:
    // Layout: all Thing fields are accessed through the imported physical layout only.
    unsigned char unobservedThingState[sizeof(CThingLayout)-sizeof(CClass)];
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
    bool SetCurrentAction(const CCreatureActionBase&);
};
class CThingAICreature:public CThingCreatureBase {public:
    unsigned char unobservedAIState[sizeof(CThingAICreatureLayout)-sizeof(CThingCreatureBase)];
    void SuspendAI(bool,void*);void ForceAIReset();
};
#pragma pack(pop)
class CCreatureActionBase:public CBaseObject {public:
    unsigned char unobservedActionState[sizeof(CCreatureActionBaseLayout)-sizeof(CBaseObject)];virtual ~CCreatureActionBase();
};
class CCombatActionBase:public CCreatureActionBase {public:
    unsigned char unobservedCombatState[sizeof(CCombatActionBaseLayout)-sizeof(CCreatureActionBase)];virtual ~CCombatActionBase();
};
class __declspec(novtable) CCreatureAction_TrollVulnerableOutOf:public CCombatActionBase {public:CCreatureAction_TrollVulnerableOutOf(CThingCreatureBase&);};
class CTCBase:public CBaseClassNonCopyable {public:
    // Layout: ParentThing and component flags are unread.
    unsigned char unobservedTCState[8];
};
class CTCCreatureModeManager:public CTCBase {
public:static ETCInterfaceType __fastcall StatGetInterfaceType(){return (ETCInterfaceType)0x31;}void RemoveMode(unsigned long);
    class CModeBase {
    public:virtual ~CModeBase();
        // Layout: ParentThing, WeaponBasedFilter, removal timer and active flag are unread.
        unsigned char unobservedModeState[24];
    };
};
namespace NCreatureMode {
class CModeRockTrollVulnerable:public CTCCreatureModeManager::CModeBase {
private:long RecoveryCount;
    // Layout: original CAnimFilterOfGroup is unread.
    unsigned char unobservedFilter[12];CThingCreatureBase*PCreature;bool bRecovering;
private:friend int ::main();void Recover();
};
void CModeRockTrollVulnerable::Recover() {
    PCreature->SetCurrentAction(CCreatureAction_TrollVulnerableOutOf(*PCreature));
    CThingCreatureBase*creature=PCreature;
    if(creature->GetType()==1) {
        static_cast<CThingAICreature*>(creature)->SuspendAI(false,0);
        static_cast<CThingAICreature*>(creature)->ForceAIReset();
    }
    CTCCreatureModeManager*modes;PCreature->GetTC(&modes);modes->RemoveMode(0x3a);
}
}
FABLE_STATIC_ASSERT(sizeof(CCreatureAction_TrollVulnerableOutOf)==0x114);
FABLE_STATIC_ASSERT(sizeof(CClass)==sizeof(CClassLayout));
FABLE_STATIC_ASSERT(sizeof(NCreatureMode::CModeRockTrollVulnerable)==52);

static unsigned int fixtureTrace=0,fixtureErrors=0,fixtureActions=0,fixtureDestroy=0,fixtureLookup=0;
static CThingCreatureBase*fixtureCreature=0;static CTCCreatureModeManager*fixtureManager=0;static const void*fixtureMap=0;
CBaseClass::~CBaseClass(){} CBaseClassNonCopyable::~CBaseClassNonCopyable(){} CBaseObject::~CBaseObject(){}
CCreatureActionBase::~CCreatureActionBase(){}
CCombatActionBase::~CCombatActionBase(){fixtureTrace=fixtureTrace*10+3;++fixtureDestroy;}
CTCCreatureModeManager::CModeBase::~CModeBase(){}
CCreatureAction_TrollVulnerableOutOf::CCreatureAction_TrollVulnerableOutOf(CThingCreatureBase&thing){fixtureTrace=fixtureTrace*10+1;++fixtureActions;if(&thing!=fixtureCreature)++fixtureErrors;}
bool CThingCreatureBase::SetCurrentAction(const CCreatureActionBase&){fixtureTrace=fixtureTrace*10+2;if(this!=fixtureCreature)++fixtureErrors;return true;}
void CThingAICreature::SuspendAI(bool suspend,void*who){fixtureTrace=fixtureTrace*10+4;if(this!=fixtureCreature||suspend||who)++fixtureErrors;}
void CThingAICreature::ForceAIReset(){fixtureTrace=fixtureTrace*10+5;if(this!=fixtureCreature)++fixtureErrors;}
void CTCCreatureModeManager::RemoveMode(unsigned long mode){fixtureTrace=fixtureTrace*10+7;if(this!=fixtureManager||mode!=0x3a)++fixtureErrors;}
template<class K,class V,class C>std::pair<K,V>*CVectorMap<K,V,C>::LowerBound(const K&key){fixtureTrace=fixtureTrace*10+6;++fixtureLookup;if(this!=fixtureMap||(int)key!=0x31)++fixtureErrors;std::pair<K,V>*first=this->_M_start;std::pair<K,V>*last=this->_M_finish;while(first!=last&&first->first<key)++first;return first;}
struct FixtureGuardedMode{unsigned int before;NCreatureMode::CModeRockTrollVulnerable mode;unsigned int after;};
int main(){
 unsigned int cases=0;
 for(int n=1;n<=8;++n)for(int selected=0;selected<n;++selected)for(int type=0;type<4;++type)for(int seed=0;seed<16;++seed){
  CThingAICreature creature;CTCCreatureModeManager manager;FixtureGuardedMode guarded;fixtureCreature=&creature;fixtureManager=&manager;creature.Type=(unsigned char)type;
  CThingLayout*physical=reinterpret_cast<CThingLayout*>(&creature);memset(physical->UsingComponents,0,sizeof(physical->UsingComponents));reinterpret_cast<unsigned long*>(physical->UsingComponents)[1]=0x20000UL;
  std::pair<ETCInterfaceType,CTCBase*>entries[9];for(int i=0;i<n;++i){entries[i].first=(ETCInterfaceType)(0x31+i-selected);entries[i].second=&manager;}entries[n].first=(ETCInterfaceType)0x7fffffff;entries[n].second=&manager;
  std::pair<ETCInterfaceType,CTCBase*>*words[3]={entries,entries+n,entries+9};memcpy(physical->Components,words,sizeof(words));fixtureMap=physical->Components;guarded.mode.PCreature=&creature;guarded.mode.RecoveryCount=seed;guarded.mode.bRecovering=(seed&1)!=0;guarded.before=0x12345678U;guarded.after=0xfedcba98U;
  unsigned char beforeCreature[sizeof(creature)],beforeMode[sizeof(guarded.mode)],beforeEntries[sizeof(entries)];memcpy(beforeCreature,&creature,sizeof(creature));memcpy(beforeMode,&guarded.mode,sizeof(guarded.mode));memcpy(beforeEntries,entries,sizeof(entries));
  fixtureTrace=fixtureErrors=fixtureActions=fixtureDestroy=fixtureLookup=0;guarded.mode.Recover();
  if(fixtureErrors||fixtureTrace!=(type==1?1234567U:12367U)||fixtureActions!=1||fixtureDestroy!=1||fixtureLookup!=1)return 1;
  if(memcmp(beforeCreature,&creature,sizeof(creature))||memcmp(beforeMode,&guarded.mode,sizeof(guarded.mode))||memcmp(beforeEntries,entries,sizeof(entries))||guarded.before!=0x12345678U||guarded.after!=0xfedcba98U)return 2;
  ++cases;
 }
 if(cases!=2304)return 3;puts("008de700_TEST PASS");return 0;
}