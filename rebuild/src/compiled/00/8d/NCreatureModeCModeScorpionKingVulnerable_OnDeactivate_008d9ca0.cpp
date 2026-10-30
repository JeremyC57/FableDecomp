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
class CCharString {protected:CCharStringData*PStringData;public:CCharString(const char*,long=-1);~CCharString();};
class CBaseObjectPointer;class CTCBase;
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
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
class CTCPhysicsBase:public CTCBase {public:
    // Layout: position, old position and velocity precede the observed radius.
    unsigned char unobservedPhysicsState[0x34-12];
protected:float Radius;
public:void SetRadius(float radius){Radius=radius;}
    // Layout: definition pointer and movement flags are unread.
    unsigned char unobservedPhysicsTail[8];
};
#pragma pack(push,1)
class CBaseIntelligentPointer {private:CBaseObjectPointer*PData;public:virtual ~CBaseIntelligentPointer();CBaseObject*GetPItem()const;};
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
class CModeScorpionKingVulnerable:public CModeStandBase {private:
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