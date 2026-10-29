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