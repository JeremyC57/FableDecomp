// 00939180 CAIStateGroup_MinionSitAtHome::IsRunnable -- consult idle scheduler then set sitting deadline
// Leaf: ?IsRunnable@CAIStateGroup_MinionSitAtHome@@MAE_NXZ
// Plan phase 5 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 4 (2026-11-05), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Flags:
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

#pragma pack(push,1)
#include "engine/C3DVector.h"
#pragma pack(pop)
#include "engine/CArray.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
#pragma pack(push,1)
class CClass:public CBaseObject {private:unsigned char unobservedClassState[sizeof(CClassLayout)-sizeof(CBaseObject)];};
enum ETCInterfaceType{};class CTCBase;
#include "engine/CVectorMap.h"
class CThing:public CClass {
public:template<class T>FABLE_FORCEINLINE bool GetTC(T**out){
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
protected:long FrameToStopSitting;virtual bool IsRunnable();
};
bool CAIStateGroup_MinionSitAtHome::IsRunnable(){
 CTCIdleScheduler*scheduler;if(GetCreature().GetTC(&scheduler)&&!scheduler->IsPreferredIdle(this))return false;
 CWorld&world=GetWorld();unsigned long delay=GFRandom(150,GetPWorldSeed());
 FrameToStopSitting=world.GetFrame()+delay+225;
 return CAIStateGroup_MinionSitOnSeatBase::IsRunnable();
}