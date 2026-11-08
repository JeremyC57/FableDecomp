// 00929650 CAIStateGroup_MinionGuardGreetPlayer::IsRunnable()
// Leaf: ?IsRunnable@CAIStateGroup_MinionGuardGreetPlayer@@UAE_NXZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Flags:
#include "fable_compiler.h"
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
#include "engine/CVectorMap.h"
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
#pragma pack(push,1)
class CClass:public CBaseObject {private:unsigned char unobservedClassState[sizeof(CClassLayout)-sizeof(CBaseObject)];};
enum ETCInterfaceType {};
class CTCBase;
#include "engine/C3DVector.h"
class CThing:public CClass {
public:const C3DVector&GetPos()const;

    bool HasTC(ETCInterfaceType type)const {
        const unsigned long*bits=reinterpret_cast<const unsigned long*>(reinterpret_cast<const CThingLayout*>(this)->UsingComponents);
        if(bits[(unsigned)type>>5]&(1UL<<((unsigned)type&31)))return true;
        return false;
    }
    FABLE_FORCEINLINE CTCBase&GetTC(ETCInterfaceType type) {
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


#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
#pragma pack(push,1)
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