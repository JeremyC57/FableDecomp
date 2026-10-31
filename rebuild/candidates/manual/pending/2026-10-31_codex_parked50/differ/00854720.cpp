// 00854720 CCreatureAction_SpiritReturnToHero::PerformAction
// Codex parked-50 best attempt: DIFFER(104v104); residue: 104/104 bytes; controlled-player/target work uses swapped ESI/EDI roles.
// Flags:
// Leaf: ?PerformAction@CCreatureAction_SpiritReturnToHero@@UAEXXZ
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

#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
#define CCreatureActionBase CCreatureActionBaseFields
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase
#define CCombatActionBase CCombatActionBaseFields
#include "engine/_quarantine/CCombatActionBase.h"
#undef CCombatActionBase
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CCountedPointer.h"
#include "engine/CPlayerManager.h"
#define CTCBase CTCBaseLayout
#include "engine/CTCBase.h"
#undef CTCBase
class CCharString;class CThing;class CThingCreatureBase;class CTCParamsBase;
#pragma pack(push,1)
class C3DVector{public:float X,Y,Z;C3DVector(const C3DVector&v):X(v.X),Y(v.Y),Z(v.Z){}};
#pragma pack(pop)
class C2DVector;class CRightHandedSet;class CWorld;enum ETCInterfaceType{};
class CPersistContext;class CEnginePrimitivePrepareInfo;namespace NTCMessages{class CMessageBase;}
class CThingManager;
class CTCBase:public CBaseClassNonCopyable{
protected:virtual void OnSerialise(CPersistContext&,bool);CWorld&GetWorld()const;CThingManager&GetThingManager();
public:
    virtual void OnRemove();
    virtual void ConstructFromParams(const CTCParamsBase&);
    virtual void OnCreate();virtual void InitialActivate();virtual void OnInitialActivate();
    virtual void RenderUpdate(float);virtual bool HasFrameUpdate()const;virtual void FrameUpdate();virtual void BulletTimeFrameUpdate();
    virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&);
    virtual void Copy(const CTCBase&);virtual void OnDie();virtual void OnKill();virtual void OnLevelUnload();virtual void OnSetInLimbo(bool);
    virtual bool Validate(CCharString&)const;virtual bool IsFreeForDeletion()const;virtual bool IsSerialiseCreatable()const;virtual bool IsSerialisableForHeroState()const;
    virtual ETCInterfaceType GetInterfaceType()const=0;virtual CCharString GetClassTypeName()const=0;
    virtual bool IsComponentSerialisable()const;virtual bool IsDriver()const;virtual bool IsCopyable()const;virtual void GlobalUpdate(bool);
    virtual void HandleMessage(const NTCMessages::CMessageBase&);
protected:CThing&ParentThing;
private:bool ToBeRemoved,IsInGlobalUpdateTCs;unsigned char unobservedAlignment[2];
};

// Layout: CTCPhysicsBase extends the28-slot native CTCBase prefix.
class CTCPhysicsBase:public CTCBase{
protected:virtual C2DVector GetOffsetVector()const;virtual C3DVector GetOffsetVector3D()const;
public:virtual void SetPosition(const C3DVector&);virtual void ResetPosition(const C3DVector&);
};
#pragma pack(push,1)
class CClass:public CBaseObject{unsigned char unreadClass[sizeof(CClassLayout)-sizeof(CBaseObject)];};
// Layout: actual PhysicsTC at60; no other own fields observed here.
class CThing:public CClass{
 unsigned char unreadThing[0x60-sizeof(CClass)];CTCPhysicsBase*PhysicsTC;
 unsigned char unreadThingTail[sizeof(CThingLayout)-0x64];
public:const C3DVector&GetPos()const;CTCPhysicsBase*GetPhysicsTC(){return PhysicsTC;}
};
class CThingPhysical:public CThing{unsigned char unreadPhysical[sizeof(CThingPhysicalLayout)-sizeof(CThing)];};
class CThingGameObject:public CThingPhysical{unsigned char unreadGameObject[sizeof(CThingGameObjectLayout)-sizeof(CThingPhysical)];};
class CThingCreatureBase:public CThingGameObject{unsigned char unreadCreature[sizeof(CThingCreatureBaseLayout)-sizeof(CThingGameObject)];};
class CThingPlayerCreature:public CThingCreatureBase{};
#pragma pack(pop)
class CPlayer{public:CThingPlayerCreature*GetPControlledCreature()const;};
class CDeviceResetCallback{public:virtual void OnPreDeviceReset()=0;virtual bool OnPostDeviceReset()=0;};
class CGame;
class CGameComponent:public CBaseClassNonCopyable,public CDeviceResetCallback{
public:virtual~CGameComponent();
protected:bool Quit,Running;CGame*Game;
};
namespace boost{template<class T>class scoped_ptr{T*px;public:T&operator*()const{return *px;}};}
class CASoundBank;namespace NGameText{class CDataBank;}
// Layout: original game base16, PSampleBank16, PTextBank20/24, PPlayerManager28.
class CMainGameComponent:public CGameComponent{
protected:static CMainGameComponent*PMainGameComponent;
 CASoundBank*PSampleBank;CCountedPointer<NGameText::CDataBank>PTextBank;boost::scoped_ptr<CPlayerManager>PPlayerManager;
public:static CMainGameComponent*__fastcall Get(){return PMainGameComponent;}
 CPlayerManager&GetPlayerManager(){return *PPlayerManager;}
};
class CBaseIntelligentPointer{CBaseObjectPointer*PData;public:virtual~CBaseIntelligentPointer();CBaseObject*GetPItem()const;};
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{public:T*Get()const{return static_cast<T*>(GetPItem());}};
class CCreatureActionBase:public CBaseObject{
protected:CIntelligentPointer<CThingCreatureBase>PCreature;
 unsigned char unreadAction[0xa8-16];
 CPlayer&GetPlayer(){return CMainGameComponent::Get()->GetPlayerManager().GetMainPlayer();}
public:virtual void EditUpdate(float);virtual void PerformAction();
 const CThingCreatureBase*GetPCreature(){return PCreature.Get();}
};
class CCreatureAction_SpiritReturnToHero:public CCreatureActionBase{public:virtual void PerformAction();};
void CCreatureAction_SpiritReturnToHero::PerformAction(){
 CPlayer&player=GetPlayer();
 C3DVector target=(player.GetPControlledCreature()?player.GetPControlledCreature():GetPCreature())->GetPos();
 const_cast<CThingCreatureBase*>(GetPCreature())->GetPhysicsTC()->ResetPosition(target);
}
