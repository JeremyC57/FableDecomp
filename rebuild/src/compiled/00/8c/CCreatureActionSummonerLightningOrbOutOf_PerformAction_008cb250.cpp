// 008cb250 CCreatureAction_SummonerLightningOrbOutOf::PerformAction
// Leaf: ?PerformAction@CCreatureAction_SummonerLightningOrbOutOf@@UAEXXZ
// Flags:
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CDefPointer.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CParentDefClassBase.h"
#define CThing CThingFields
#include "engine/_quarantine/CThing.h"
#undef CThing
#define CThingPhysical CThingPhysicalFields
#include "engine/CThingPhysical.h"
#undef CThingPhysical
#define CThingGameObject CThingGameObjectFields
#include "engine/CThingGameObject.h"
#undef CThingGameObject
#define CThingCreatureBase CThingCreatureBaseFields
#include "engine/CThingCreatureBase.h"
#undef CThingCreatureBase
#define CTCBase CTCBaseFields
#include "engine/CTCBase.h"
#undef CTCBase
#define CTCPhysicsBase CTCPhysicsBaseFields
#include "engine/CTCPhysicsBase.h"
#undef CTCPhysicsBase
#define CTCGraphicAppearance CTCGraphicAppearanceFields
#include "engine/_quarantine/CTCGraphicAppearance.h"
#undef CTCGraphicAppearance
#define CCreatureActionBase CCreatureActionBaseFields
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase
#define CCombatActionBase CCombatActionBaseFields
#include "engine/_quarantine/CCombatActionBase.h"
#undef CCombatActionBase
#define CSummonerDef CSummonerDefFields
#include "engine/CSummonerDef.h"
#undef CSummonerDef
#define CDefinitionManager CDefinitionManagerFields
#include "engine/CDefinitionManager.h"
#undef CDefinitionManager
class C3DVector{public:float X,Y,Z;};class CRightHandedSet{protected:C3DVector Up,Forward;};
class CCharStringData;class CCharString{protected:CCharStringData*PStringData;public:CCharString(const char*,long=-1);~CCharString();};
class CDefString{private:long TablePos;public:bool operator==(const char*)const;};
class CBaseObjectPointer;
class CBaseIntelligentPointer{private:CBaseObjectPointer*PData;public:virtual~CBaseIntelligentPointer();CBaseObject*GetPItem()const;};
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{public:T*Get()const{return reinterpret_cast<T*>(GetPItem());}T*operator->()const{return Get();}};
enum ETCInterfaceType{};class C2DVector;class CTCParamsBase;class CEnginePrimitivePrepareInfo;class CPersistContext;namespace NTCMessages{class CMessageBase;}
// Layout: shared CTCBase prefix; native table omits donor editor-only property transfer pair.
class CTCBase:public CBaseClassNonCopyable{protected:CThing&ParentThing;private:bool ToBeRemoved,IsInGlobalUpdateTCs;
protected:virtual void OnSerialise(CPersistContext&,bool);
public:virtual void OnRemove();
public:virtual void ConstructFromParams(const CTCParamsBase&);
public:virtual void OnCreate();
public:virtual void InitialActivate();
public:virtual void OnInitialActivate();
public:virtual void RenderUpdate(float);
public:virtual bool HasFrameUpdate()const;
public:virtual void FrameUpdate();
public:virtual void BulletTimeFrameUpdate();
public:virtual void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&);
public:virtual void Copy(const CTCBase&);
public:virtual void OnDie();
public:virtual void OnKill();
public:virtual void OnLevelUnload();
public:virtual void OnSetInLimbo(bool);
public:virtual bool Validate(CCharString&)const;
public:virtual bool IsFreeForDeletion()const;
public:virtual bool IsSerialiseCreatable()const;
public:virtual bool IsSerialisableForHeroState()const;
public:virtual ETCInterfaceType GetInterfaceType()const;
public:virtual CCharString GetClassTypeName()const;
public:virtual bool IsComponentSerialisable()const;
public:virtual bool IsDriver()const;
public:virtual bool IsCopyable()const;
public:virtual void GlobalUpdate(bool);
public:virtual void HandleMessage(const NTCMessages::CMessageBase&);
};
// Layout: shared physics base64, actual virtual prefix and unread member storage after12.
class CTCPhysicsBase:public CTCBase{unsigned char unreadPhysics[0x40-0xc];
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
public:virtual void SetFacingAngleYZ(float);
public:virtual void SetFacingAngleXZ(float);
public:virtual C3DVector GetFacingVector()const;
public:virtual C3DVector GetFacingVector(float)const;
public:virtual const CRightHandedSet& GetRHSet()const;
public:virtual CRightHandedSet GetRHSet(float)const;
};

namespace NGraphicAppearance{class CTCGraphicAppearance:public CTCGraphicAppearanceFields{public:bool GetDummyObject(const CCharString&,long,unsigned long,C3DVector&,CRightHandedSet&,float)const;};}
namespace std{template<class T>class allocator{};template<class A,class B>struct pair{A first;B second;};template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};template<class T,class A>class _Vector_base{protected:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:T*end(){return this->_M_finish;}};}
template<class T>class CArray:public std::vector<T>{};template<class K,class V>class CKeyPairCompareLess{};template<class K,class V,class C=CKeyPairCompareLess<K,V> >class CVectorMap:public CArray<std::pair<K,V> >{C Compare;bool Dirty;public:std::pair<K,V>*LowerBound(const K&);};
class CThingBaseDef:public CParentDefClassBase{};
// Layout: shared native CThing fields: component bits20/map44, physics60/graphics64, base def70, player90/dead bit91.
class CThing:public CThingFields{public:
 const C3DVector&GetPos()const;CDefString GetDefName()const;
 CDefPointer<const CThingBaseDef>GetPBaseDef()const{return reinterpret_cast<const CDefPointer<const CThingBaseDef>&>(PBaseDef_Object);}
 unsigned long GetPlayerNumber()const{return PlayerNumber;}
 bool IsDead()const{return (reinterpret_cast<const unsigned char*>(this)[0x91]&1)!=0;}
 CTCPhysicsBase*GetPhysicsTC(){return PhysicsTC;}
 NGraphicAppearance::CTCGraphicAppearance*GetTCGraphicAppearance(){return reinterpret_cast<NGraphicAppearance::CTCGraphicAppearance*>(PTCGraphicAppearance);}
 template<class T>__forceinline bool GetTC(T**out){ETCInterfaceType kind=T::StatGetInterfaceType();if(reinterpret_cast<const unsigned long*>(UsingComponents)[kind>>5]&(1UL<<(kind&31))){ETCInterfaceType key=kind;CVectorMap<ETCInterfaceType,CTCBase*>&map=reinterpret_cast<CVectorMap<ETCInterfaceType,CTCBase*>&>(Components);std::pair<ETCInterfaceType,CTCBase*>*it=map.LowerBound(key);*out=reinterpret_cast<T*>((it==map.end()||kind<it->first?map.end():it)->second);return true;}return false;}
};
class CThingPhysical:public CThing{};class CThingGameObject:public CThingPhysical{};class CThingCreatureBase:public CThingGameObject{};
class CThingObject:public CThingGameObject{public:static CThingObject*__fastcall Create(long,const C3DVector&,long,const CRightHandedSet*,unsigned long,const CCharString&);};
class CTCCreatureModeManager:public CTCBase{public:static ETCInterfaceType __fastcall StatGetInterfaceType(){return static_cast<ETCInterfaceType>(0x31);}void RemoveMode(unsigned long);};
class CTCLightningOrb:public CTCBase{public:static ETCInterfaceType __fastcall StatGetInterfaceType(){return static_cast<ETCInterfaceType>(0x10a);}void Initialise(CThing&,CThing&);void SetLifeTime(float);};
// Layout: original subdef base25 then float members28..48, index4c/50/54, life-seconds float58, shared header confirms all.
class CSummonerDef:public CSubDefClassBase{public:float LightningRangeForAutomaticUse,LightningPercentChanceForRandomUse,LightningPercentChanceForBoastUse,LightningRandomChanceTestSeconds,LightningSecondsToWaitBeforeReuse,ChargeRangeNear,ChargeRangeFar,ChargeRangeAttackInterrupt,ChargeSecondsToWaitBeforeReuse;long SummonerDeathExplosion,LightningOrb,LightningOrbAttackFireheart;float LightningOrbLifeSeconds;static CCharString __fastcall StatGetTypeName(){return CCharString("CSummonerDef");}};
class CDefinitionManager:public CDefinitionManagerFields{public:CDefPointer<CDefClassBase>GetPDefFromGlobalIndex(long);};
template<class T>__declspec(noinline)bool CDefClassBase::GetSubDef(CDefPointer<const T>*out)const{const CSubDefInfo*info=GetPSubDefInfo(T::StatGetTypeName());if(!info)return false;CDefPointer<const T>value(PDefManager->GetPDefFromGlobalIndex(info->DefIndex));out->SetDynamicCast(value);return true;}
class CWorld;
class CCreatureActionBase:public CBaseObject{public:
 virtual void EditUpdate(float);virtual void PerformAction()=0;virtual void FinishAction();virtual bool IsPlayingAnimation()const;
 virtual long GetAnimDelayTime()const;virtual long GetAnimationIndex()const;virtual long GetAnimationTransitionInTime()const;virtual long GetAnimationTransitionOutTime()const;
 virtual unsigned long GetAnimExtraFlags()const;virtual CCreatureActionBase*Clone()const=0;virtual void Initialise();
protected:CWorld&GetWorld();CIntelligentPointer<CThingCreatureBase>PCreature;
private:unsigned char unobservedActionState[sizeof(CCreatureActionBaseFields)-sizeof(CBaseObject)-sizeof(CIntelligentPointer<CThingCreatureBase>)];};
class CCombatActionBase:public CCreatureActionBase{public:virtual void Initialise();void SetCombatAnimation(const CCharString&);
protected:CIntelligentPointer<CThing>PTarget;
private:unsigned char unobservedCombatActionState[sizeof(CCombatActionBaseFields)-sizeof(CCreatureActionBase)-sizeof(CIntelligentPointer<CThing>)];};
class CCreatureAction_SummonerLightningOrbOutOf:public CCombatActionBase{public:virtual void PerformAction();};
void CCreatureAction_SummonerLightningOrbOutOf::PerformAction()
{
    CTCCreatureModeManager* modeManager;
    if (PCreature.Get()->GetTC(&modeManager))
        modeManager->RemoveMode(0x5a);
    CDefPointer<const CSummonerDef> summonerDef;
    if (PTarget.Get() && !PTarget.Get()->IsDead() && PCreature.Get()->GetPBaseDef()->GetSubDef(&summonerDef)) {
        bool fireheart = false;
        long orbDef = summonerDef->LightningOrb;
        if (PTarget.Get()->GetDefName() == "OBJECT_LPDD_FIREHEART_01") {
            orbDef = summonerDef->LightningOrbAttackFireheart;
            fireheart = true;
        }
        CThingObject* orb = CThingObject::Create(orbDef, PCreature.Get()->GetPos(), PCreature.Get()->GetPlayerNumber(), 0, 0, "");
        C3DVector position;
        CRightHandedSet orientation;
        bool found;
        {
            CCharString name("LIGHTNING_ORB_FOCUS");
            found = PCreature.Get()->GetTCGraphicAppearance()->GetDummyObject(name, 0, 0, position, orientation, 1.0f);
        }
        if (found)
            orb->GetPhysicsTC()->ResetPosition(position);
        orb->GetPhysicsTC()->ResetRHSet(PCreature.Get()->GetPhysicsTC()->GetRHSet());
        CTCLightningOrb* lightningOrb;
        orb->GetTC(&lightningOrb);
        lightningOrb->Initialise(*PCreature.Get(), *PTarget.Get());
        if (!fireheart)
            lightningOrb->SetLifeTime(summonerDef->LightningOrbLifeSeconds);
    }
}