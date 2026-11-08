// 0090c670 CAIStateGroup_MinionGuardFollowPlayer::OnReplace()
// Leaf: ?OnReplace@CAIStateGroup_MinionGuardFollowPlayer@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Flags:
// Layout: existing shared flattened headers supply observed offsets; no new packing.
#include "fable_compiler.h"
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
#define CAIStateGroup_Gaze CAIStateGroup_GazeLayout
#include "engine/CAIStateGroup_Gaze.h"
#undef CAIStateGroup_Gaze
#define CAIStateGroup_GazeAtWork CAIStateGroup_GazeAtWorkLayout
#include "engine/CAIStateGroup_GazeAtWork.h"
#undef CAIStateGroup_GazeAtWork
#include "engine/CVectorMap.h"
#include "engine/CBaseClass.h"
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
#include "engine/CBaseClassNonCopyable.h"
class CClass:public CBaseObject {private:unsigned char unobservedClassState[sizeof(CClassLayout)-sizeof(CBaseObject)];};
enum ETCInterfaceType {};
class CTCBase;
class CThing:public CClass {
public:

    bool HasTC(ETCInterfaceType type)const {
        const unsigned long*bits=reinterpret_cast<const unsigned long*>(reinterpret_cast<const CThingLayout*>(this)->UsingComponents);
        if(bits[(unsigned)type>>5]&(1UL<<((unsigned)type&31)))return true;
        return false;
    }
    template<class T>FABLE_FORCEINLINE bool GetTC(T**out) {
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
    bool IsDead()const{return (reinterpret_cast<const unsigned char*>(this)[0x91]&1)!=0;}
    unsigned long GetTypeFlags()const{return reinterpret_cast<const CThingLayout*>(this)->ThingTypeFlags;}
private:unsigned char unobservedThingState[sizeof(CThingLayout)-sizeof(CClass)];
};
class CThingPhysical:public CThing {private:unsigned char unobservedPhysicalState[sizeof(CThingPhysicalLayout)-sizeof(CThing)];};
class CThingGameObject:public CThingPhysical {private:unsigned char unobservedGameObjectState[sizeof(CThingGameObjectLayout)-sizeof(CThingPhysical)];};
class CThingCreatureBase:public CThingGameObject {private:unsigned char unobservedCreatureState[sizeof(CThingCreatureBaseLayout)-sizeof(CThingGameObject)];};
class CThingAICreature:public CThingCreatureBase {
public:static unsigned long __fastcall StatGetTypeFlags(){return 31;}
private:unsigned char unobservedAIState[sizeof(CThingAICreatureLayout)-sizeof(CThingCreatureBase)];
};

class CAIBrain;class CWorld;class CCharString;class CThingBuilding;
enum EScriptAIPriority {};enum EAIStateGroupType {};
class CAIStateGroupBase:public CBaseClass {
public:
    virtual void Initialise(CAIBrain&,CThingAICreature&,long,EScriptAIPriority,const CCharString&);
    virtual void OnActivate();virtual bool InitCreaturePosition();virtual bool AllowPriorityOverlap();
    virtual bool IsStillValid()const;virtual const CCharString*GetDebugText()const;
    virtual bool IsTimingCritical()const;virtual EAIStateGroupType GetStateGroupType()const;
protected:
    virtual void Cleanup();virtual void OnReplace();virtual bool IsRunnable();virtual void Update()=0;
    virtual void InitStateGroup(const CWorld&,CThingAICreature&);
    CAIBrain*PBrain;
    unsigned char unobservedGroupState[sizeof(CAIStateGroupBaseLayout)-8];
    CWorld&GetWorld()const;
    CThingAICreature&GetCreature()const{return *reinterpret_cast<const CAIBrainLayout*>(PBrain)->Creature;}
};

#define CIVCountedPointeeBase CIVCountedPointeeBaseLayout
#include "engine/CIVCountedPointeeBase.h"
#undef CIVCountedPointeeBase
#define CResource CResourceLayout
#include "engine/CResource.h"
#undef CResource
#define CDefClassBase CDefClassBaseLayout
#include "engine/CDefClassBase.h"
#undef CDefClassBase
#define CVillageDef CVillageDefLayout
#include "engine/_quarantine/CVillageDef.h"
#undef CVillageDef
#define CTCVillage CTCVillageLayout
#include "engine/CTCVillage.h"
#undef CTCVillage

class CBaseObjectPointer;
class CBaseIntelligentPointer {
public:CBaseIntelligentPointer();virtual~CBaseIntelligentPointer();CBaseObject*GetPItem()const;void SetPItem(CBaseObject*);
private:CBaseObjectPointer*PData;
};
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer {
public:CIntelligentPointer(T*item){SetPItem(static_cast<CBaseObject*>(item));}
    CIntelligentPointer&operator=(const CIntelligentPointer&p){SetPItem(p.GetPItem());return *this;}
};
#define CTCBase CTCBaseLayout
#include "engine/CTCBase.h"
#undef CTCBase
class CTCBase:public CBaseClassNonCopyable {private:unsigned char unobservedTCState[sizeof(CTCBaseLayout)-sizeof(CBaseClassNonCopyable)];};
class CIVCountedPointeeBase {
public:virtual~CIVCountedPointeeBase();virtual void Delete()const;
private:mutable unsigned long IVRefCount;public:
    void AddRef()const{++IVRefCount;}
    long Release()const{return --IVRefCount;}
};
class CResource:public CIVCountedPointeeBase {private:unsigned char unobservedResourceState[sizeof(CResourceLayout)-sizeof(CIVCountedPointeeBase)];};
class CDefPointeeBase:public CResource {};
class CDefClassBase:public CDefPointeeBase {private:unsigned char unobservedDefinitionState[sizeof(CDefClassBaseLayout)-sizeof(CDefPointeeBase)];};
class CSubDefClassBase:public CDefClassBase {};
// Layout: observed definition prefix only; actual original header supplies corrected field positions.
class CVillageDef:public CSubDefClassBase {
private:unsigned char unobservedVillageDefinitionState[0xfc-sizeof(CSubDefClassBase)];
public:long FramesBetweenGuardSnoop;long FramesBetweenGuardSnoopNightShops;
};
template<class T>class CDefPointer {
private:CDefPointeeBase*Object;public:
    CDefPointer(const CDefPointer&p):Object(p.Object){if(Object)Object->AddRef();}
    ~CDefPointer(){if(Object&&Object->Release()==0)Object->Delete();}
    T*operator->()const{return static_cast<T*>(Object);}
};
#define CWorld CWorldLayout
#include "engine/CWorld.h"
#undef CWorld
class CWorld:public CWorldLayout {public:static long __fastcall GetFrame();};
class CTCVillage:public CTCBase {
public:
    void SetGuardFollowingPlayer(CThingAICreature*creature){
        *reinterpret_cast<CIntelligentPointer<CThingAICreature>*>(reinterpret_cast<CTCVillageLayout*>(this)->GuardFollowingPlayer)=CIntelligentPointer<CThingAICreature>(creature);
    }
    CDefPointer<const CVillageDef>GetPDef()const{return *reinterpret_cast<const CDefPointer<const CVillageDef>*>(&reinterpret_cast<const CTCVillageLayout*>(this)->PDef_Object);}
    void SetNextFrameToFollowPlayer(long frame){reinterpret_cast<CTCVillageLayout*>(this)->NextFrameToFollowPlayer=frame;}
    bool IsNightTime()const;
private:unsigned char unobservedVillageState[sizeof(CTCVillageLayout)-sizeof(CTCBase)];
};
class CTCVillageMember:public CTCBase {
public:static FABLE_FORCEINLINE ETCInterfaceType __fastcall StatGetInterfaceType(){return (ETCInterfaceType)0x23;}
    CTCVillage*GetPVillage()const;
};
class CAIStateGroup_MinionGuardFollowPlayer:public CAIStateGroupBase {
public:virtual void OnReplace();
};
void CAIStateGroup_MinionGuardFollowPlayer::OnReplace()
{
    CTCVillageMember*member;
    CTCVillage*village;
 if(GetCreature().GetTC(&member))village=member->GetPVillage();else village=member->GetPVillage();
    village->SetGuardFollowingPlayer(0);
    if(village->IsNightTime()) {
        CWorld&world=GetWorld();
        CDefPointer<const CVillageDef>definition=village->GetPDef();
        village->SetNextFrameToFollowPlayer(world.GetFrame()+definition->FramesBetweenGuardSnoopNightShops);
    }else {
        CWorld&world=GetWorld();
        CDefPointer<const CVillageDef>definition=village->GetPDef();
        village->SetNextFrameToFollowPlayer(world.GetFrame()+definition->FramesBetweenGuardSnoop);
    }
}