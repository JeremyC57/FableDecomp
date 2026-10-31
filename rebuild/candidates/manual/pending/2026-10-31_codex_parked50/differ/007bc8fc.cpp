// 007bc8fc NThingShot::CTCShotDefinitionTearInGround::InitMovement
// Codex parked-50 best attempt: DIFFER(308v308); residue: 308/308 bytes; LEA of stack temporary and MOV of ParentThing occur in the opposite order.
// Flags:
#pragma optimize("y",off)
#pragma optimize("s",on)
// Leaf: ?InitMovement@CTCShotDefinitionTearInGround@NThingShot@@MAEXXZ
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
class CCharStringData;
class CCharString {CCharStringData*PStringData;public:CCharString(const char*,long=-1);~CCharString();};
#include "engine/CBaseClassNonCopyable.h"
class CThing;class CTCParamsBase;
class C3DVector {public:float X,Y,Z;
 __forceinline C3DVector(){}__forceinline C3DVector(float x,float y,float z):X(x),Y(y),Z(z){}
 __forceinline C3DVector(const C3DVector&v):X(v.X),Y(v.Y),Z(v.Z){}
 __forceinline const C3DVector operator-(const C3DVector&v)const{return C3DVector(X-v.X,Y-v.Y,Z-v.Z);}
 __forceinline const C3DVector operator+(const C3DVector&v)const{return C3DVector(X+v.X,Y+v.Y,Z+v.Z);}
 __forceinline const C3DVector operator*(float n)const{return C3DVector(X*n,Y*n,Z*n);}
 C3DVector&Normalise();
};
class CWorldMap;class C2DVector;class CRightHandedSet;class CWorld;enum ETCInterfaceType{};
class CPersistContext;class CEnginePrimitivePrepareInfo;namespace NTCMessages{class CMessageBase;}
class CThingManager;
class CTCBase:public CBaseClassNonCopyable{
protected:CWorldMap&GetWorldMap();virtual void OnSerialise(CPersistContext&,bool);CWorld&GetWorld()const;CThingManager&GetThingManager();
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

class CThingSearchTools;class CThingPhysicalDebugInfo;class CPhysicsDebugInfo;class CPhysicsCollideResponse;class CThingPhysicsMeshInfo;
#define CTCPhysicsBase CTCPhysicsBaseLayout
#include "engine/CTCPhysicsBase.h"
#undef CTCPhysicsBase
class CTCPhysicsBase:public CTCBase{
    protected:C3DVector Position,OldPosition;unsigned char unobservedVelocity[0x34-0x24];float Radius;unsigned char unobservedPhysics[sizeof(CTCPhysicsBaseLayout)-0x38];
public:const C3DVector&GetPosition()const{return Position;}const C3DVector&GetLastPosition()const{return OldPosition;}
protected:const CThingSearchTools&GetThingSearchTools()const;
protected:virtual C2DVector GetOffsetVector()const;
protected:virtual C3DVector GetOffsetVector3D()const;
public:virtual void SetPosition(const C3DVector&);
public:virtual void ResetPosition(const C3DVector&);
public:virtual void ResetRHSet(const CRightHandedSet&);
public:virtual void SetPositionToNavigateTo(const C3DVector&);
public:virtual void SetForcedToMovePos(const C3DVector&,bool);
public:virtual void ResetMovementState();
public:virtual void EnablePhysics(bool);
public:virtual bool IsPhysicsEnabled()const;
public:virtual void EnableGravity(bool);
public:virtual bool IsGravityEnabled()const;
public:virtual void EnableNavigator(bool);
public:virtual bool IsNavigationEnabled()const;
public:virtual bool HasNavigationLayers()const;
public:virtual void SetInteractionFlags(long);
public:virtual void ReturnToDefaultInteractionFlags();
public:virtual void OnDroppedFromUpdateList();
public:virtual void OnAddedToUpdateList();
public:virtual bool IsAlwaysFacingMovementDirection()const;
public:virtual bool IsAffectedByGravity()const;
public:virtual bool IsAffectedByAir()const;
public:virtual bool IsAffectedByWind()const;
public:virtual bool IsAngledWithGround()const;
public:virtual bool IsImmoveable()const;
public:virtual bool IsOnAnySurface()const;
public:virtual bool IsOnAnyStationarySurface()const;
public:virtual bool IsOnMeshSurface()const;
public:virtual bool IsOnGroundSurface()const;
public:virtual bool CanCollideWithThings()const;
public:virtual bool CanCollideWithThingsOneWay()const;
public:virtual bool CanCollideWithLandscape()const;
public:virtual bool CanInteractWithThings()const;
public:virtual bool IsExcludingCreatures()const;
public:virtual bool IsPhysicsMeshImpassable()const;
public:virtual float GetFacingAngleXY()const;
public:virtual float GetFacingAngleYZ()const;
public:virtual float GetFacingAngleXZ()const;
public:virtual void SetFacingAngleXY(float);void SetVelocity(const C3DVector&);

};

#pragma pack(push,1)
// Layout: CClass private byte Type16, parent CBaseObject8, native prefix17.
class CClass:public CBaseObject {
 unsigned char unreadClass[8];unsigned char Type;
public:unsigned long GetType()const{return Type;}
};
// Layout: shared CThing physical header gives PhysicsTC60 and byte flags91.
class CThing:public CClass {
 unsigned char unreadThing[0x60-sizeof(CClass)];CTCPhysicsBase*PhysicsTC;
 unsigned char unreadThing2[0x91-0x64];
 bool Dead:1;unsigned char unreadFlags:7;
 unsigned char unreadThing3[sizeof(CThingLayout)-0x92];
public:CTCBase*AddTC(const CCharString&,bool,const CTCParamsBase*);const C3DVector&GetPos()const;CTCPhysicsBase*GetPhysicsTC(){return PhysicsTC;}
 bool IsDead()const{return Dead;}
};
class CThingPhysical:public CThing {unsigned char unreadPhysical[sizeof(CThingPhysicalLayout)-sizeof(CThing)];};
class CThingGameObject:public CThingPhysical {unsigned char unreadGameObject[sizeof(CThingGameObjectLayout)-sizeof(CThingPhysical)];};
class CThingCreatureBase:public CThingGameObject {unsigned char unreadCreature[sizeof(CThingCreatureBaseLayout)-sizeof(CThingGameObject)];};
class CThingAICreature:public CThingCreatureBase {
 unsigned char unreadAI[sizeof(CThingAICreatureLayout)-sizeof(CThingCreatureBase)];
public:void SuspendAI(bool,void*);
};
#pragma pack(pop)

#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CCountedPointer.h"
#define CWorldMap CWorldMapLayout
#include "engine/CWorldMap.h"
#undef CWorldMap
// API projection only: world map is never constructed or read directly here.
class CWorldMap{public:float GetGroundSizeZAt(const C3DVector&)const;};
class CBaseIntelligentPointer{CBaseObjectPointer*PData;public:virtual~CBaseIntelligentPointer();CBaseObject*GetPItem()const;};
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{public:T*Get()const{return static_cast<T*>(GetPItem());}};
template<class T>bool __fastcall IS_THING_ALIVE(const T&);
// Layout: parent def57 byte-packed, first own bytes end at60; Speed60 native.
class CThingShotDef:public CParentDefClassBase{unsigned char unreadDef[0x3c-sizeof(CParentDefClassBase)];public:float Speed;};
class CPlayerManager;
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
protected:static CMainGameComponent*PMainGameComponent;static long ConstantFPS;
 CASoundBank*PSampleBank;CCountedPointer<NGameText::CDataBank>PTextBank;boost::scoped_ptr<CPlayerManager>PPlayerManager;
public:static long __fastcall GetConstantFPS(){return ConstantFPS;}
 static CMainGameComponent*__fastcall Get(){return PMainGameComponent;}
 CPlayerManager&GetPlayerManager(){return *PPlayerManager;}
};


enum ECollideType{};enum EHeroExperienceStatCategory{};
namespace NThingShot {
C3DVector __fastcall GetCreatureFireAtPos(const CThing&);
// Layout: original base private target32, def56; preceding vector shrinks4.
class CTCShotDefinitionBase:public CTCBase{
 unsigned char unreadShot[0x20-0xc];CIntelligentPointer<const CThing>PTargetThing;
 unsigned char unreadOwners[0x38-0x28];CDefPointer<const CThingShotDef>PShotDef;
public:
 virtual void OnCollision(ECollideType,const C3DVector&,const C3DVector&,CThing*);
 virtual void OnHitByMelee(CThing&,CThing*,const C3DVector&,const C3DVector&,bool,bool);
 virtual bool GetStatToModifyOnHit(EHeroExperienceStatCategory&);virtual float GetShotDamage()const;virtual long GetWeaponTrailGraphic()const;
 const CThing*GetPTargetThing()const{return PTargetThing.Get();}
protected:virtual void InitMovement();virtual void InitAttachedEffects();virtual void UpdateMovement();
 const CThingShotDef&GetShotDef()const{return *PShotDef;}
};
class CTCShotDefinitionTearInGround:public CTCShotDefinitionBase{protected:virtual void InitMovement();};
void CTCShotDefinitionTearInGround::InitMovement(){
 if(!IS_THING_ALIVE(GetPTargetThing()))return;
 CTCPhysicsBase*physics=ParentThing.GetPhysicsTC();
 C3DVector direction=GetCreatureFireAtPos(*GetPTargetThing())-physics->GetPosition();
 direction.Z=0.0f;direction.Normalise();
 C3DVector ground=physics->GetPosition();ground.Z=GetWorldMap().GetGroundSizeZAt(ground);
 physics->ResetPosition(direction*2.0f+ground);
 physics->SetVelocity(direction*(GetShotDef().Speed/CMainGameComponent::GetConstantFPS()));
 physics->SetInteractionFlags(0x14);
 {CCharString componentName("CTCHitOnCollision");ParentThing.AddTC(componentName,false,0);}
 UpdateMovement();
}
}
