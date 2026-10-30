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
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CBaseClassNonCopyable:public CBaseClass {protected:virtual ~CBaseClassNonCopyable();};
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
#pragma pack(push,1)
class CClass:public CBaseObject {
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
private:void Recover();
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