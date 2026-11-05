// 008bf1c0 CCreatureAction_UnblockableMidAttack::Initialise
// Flags:
// Leaf: ?Initialise@CCreatureAction_UnblockableMidAttack@@UAEXXZ
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
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
#include "engine/C3DVector.h"
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
public:virtual void SetFacingAngleXY(float);

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
public:const C3DVector&GetPos()const;CTCPhysicsBase*GetPhysicsTC(){return PhysicsTC;}
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
class CBaseIntelligentPointer {
 CBaseObjectPointer*PData;
public:virtual~CBaseIntelligentPointer();CBaseObject*GetPItem()const;
};
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer {
public:T*Get()const{return static_cast<T*>(GetPItem());}T*operator->()const{return Get();}
};
// Layout: CBaseObject8 then protected intelligent pointer8; native total168.
class CCreatureActionBase:public CBaseObject {
protected:CIntelligentPointer<CThingCreatureBase>PCreature;
private:unsigned char unreadAction[0xa8-16];
public:
 virtual void EditUpdate(float);
 virtual void PerformAction();
 virtual void FinishAction();
 virtual bool IsPlayingAnimation()const;
 virtual long GetAnimDelayTime()const;
 virtual long GetAnimationIndex()const;
 virtual long GetAnimationTransitionInTime()const;
 virtual long GetAnimationTransitionOutTime()const;
 virtual unsigned long GetAnimExtraFlags()const;
 virtual CCreatureActionBase* Clone()const;
 virtual void Initialise();
};
// Layout: native combat base276; current row touches only inherited fields.
class CCombatActionBase:public CCreatureActionBase {
 protected:CIntelligentPointer<CThing>PTarget;
private:unsigned char unreadCombat[0x114-0xb0];
public:virtual void Initialise();void SetCombatAnimation(const CCharString&);
};

#define CCreatureAction_UnblockableMidAttack CCreatureAction_UnblockableMidAttackLayout
#include "engine/CCreatureAction_UnblockableMidAttack.h"
#undef CCreatureAction_UnblockableMidAttack
float __fastcall GFGetAngleXYTo(const C3DVector&,const C3DVector&);
template<class T>inline bool __fastcall IS_THING_ALIVE(const T&p){return p.Get()!=0&&!p->IsDead();}
class CCreatureAction_UnblockableMidAttack:public CCombatActionBase{public:virtual void Initialise();};
void CCreatureAction_UnblockableMidAttack::Initialise(){
 {CCharString name("STRIKE_MEDIUM_FRONT_UNBLOCKABLE");SetCombatAnimation(name);}
 CCombatActionBase::Initialise();
 if(IS_THING_ALIVE(PTarget)){
  if(PTarget.Get()!=PCreature.Get())
   PCreature->GetPhysicsTC()->SetFacingAngleXY(GFGetAngleXYTo(PCreature->GetPos(),PTarget->GetPos()));
 }
}