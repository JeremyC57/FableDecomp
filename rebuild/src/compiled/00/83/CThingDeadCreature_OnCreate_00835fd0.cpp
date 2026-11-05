// 00835fd0 CThingDeadCreature::OnCreate
// Leaf: ?OnCreate@CThingDeadCreature@@MAE_NXZ
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Flags:
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
#define CClass CClassFields
#include "engine/CClass.h"
#undef CClass
#define CThing CThingFields
#include "engine/_quarantine/CThing.h"
#undef CThing
#define CThingPhysical CThingPhysicalFields
#include "engine/CThingPhysical.h"
#undef CThingPhysical
#define CThingGameObject CThingGameObjectFields
#include "engine/CThingGameObject.h"
#undef CThingGameObject
class CCharString;class C3DVector;class C2DVector;class CThing;class CThingPhysical;class CThingCreatureBase;class CThingAICreature;class CThingPlayerCreature;class CThingBuilding;class CTCBase;class CMaterialDef;class CEnvironment;class CEngineGraphic;class CInventoryItem;class CThingDef;class CThingProperties;class CInterpolationInfo;class CEnginePrimitivePrepareInfo;class CPhysicsObject;class CTCParamsBase;class CThingLevelScript;class CThingGameState;class CPhysicsObjectDamage;class CCollisionInfo;class CThingDrawInfo;class CThingDebugDrawInfo;class CThingPhysicalProperties;class CSoundPair;class CAnimationEntry;
template<class T>class CDefPointer;template<class T>class CArray;namespace std{template<class T>class allocator;template<class T,class A>class vector;}
#include "engine/C3DVector.h"
class CCharString;
class CCreatureAbilityDef;
class CEditEngineDrawInfo;
class CEngineGraphic;
class CEnginePrimitivePrepareInfo;
class CHitParameters;
class CInterpolationInfo;
class CMaterialDef;
class CMemoryDataInputStream;
class CPersistContext;
class CSoundPair;
class CThing;
class CThingBaseDef;
class CThingCreatureBase;
class CThingDrawInformation;
class CThingInfo;
class CThingPhysical;
class CThingPhysicalDebugInfo;
enum ECollideType{};
// Layout: native CClass17-byte flattened header, rounded20 when used as a polymorphic base.
class CClass:public CBaseObject{public:virtual void Save(CCharString&);private:unsigned char _pad_class[sizeof(CClassFields)-sizeof(CBaseObject)];};
class CThing:public CClass{public:class CSerialiseInfo;
public:virtual bool SerialiseGameState(CPersistContext&);
public:virtual bool ConstructFromLevelScript(const char*);
public:virtual bool ConstructAlreadyExistingThingFromLevelScript(const char*);
public:virtual bool ConstructFromGameStateScript(const char*);
public:virtual void CopyGameState(const CThing&);
public:virtual bool OnCreate();
public:virtual void InitialActivate();
public:virtual void OnInitialActivate();
protected:virtual void OnDie();
protected:virtual void OnKill();
protected:virtual bool PreOnKill();
protected:virtual void OnSetInLimbo(bool);
protected:virtual void CreateFromDef(const CThingBaseDef&);
public:virtual void ConstructFromBinarySave(CMemoryDataInputStream&,const CThing::CSerialiseInfo&);
public:virtual bool IsToBePlacedInMap() const;
public:virtual bool ValidateThing(CCharString&) const;
public:virtual bool WorldUpdate();
public:virtual void WorldUpdateBulletTime();
public:virtual bool RenderUpdate(const CInterpolationInfo*);
public:virtual bool IsEntitySerialisable() const;
public:virtual unsigned long GetChecksum() const;
public:virtual void GetInfo(CThingInfo*) const;
public:virtual bool DrawGetEditDrawInfo(float,CEditEngineDrawInfo*) const;
public:virtual void DrawUpdate(const CInterpolationInfo*);
public:virtual bool DrawIsDrawable() const;
public:virtual void SetAsDrawable(bool);
public:virtual bool DrawIsEditDrawable() const;
public:virtual bool DrawIsInDrawList() const;
public:virtual unsigned long DrawGetPhysicsMesh() const;
public:virtual bool DrawIsShowingFacingDirection() const;
public:virtual bool DrawIsDisplayingInformation() const;
public:virtual bool DrawGetThingDisplayInformation(CThingDrawInformation*) const;
public:virtual bool DrawGetPhysicsDebugInfo(CThingPhysicalDebugInfo&) const;
public:virtual bool DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&);
public:virtual void SetThingStandingOn(CThing*);
public:virtual CThing* GetThingStandingOn();
public:virtual const CThing* GetThingStandingOn() const;
public:virtual long GetFloorMaterial() const;
public:virtual void OnCollision(ECollideType,const C3DVector&,const C3DVector&,CThing*);
public:virtual void OnNonKillingLevelUnload();
public:virtual void OnLevelUnload();
public:virtual bool IsFrameUpdated() const;
public:virtual void SetPositionUsingBrain();
public:template<class T>bool GetTC(T**);CTCBase*AddTC(const CCharString&,bool,const CTCParamsBase*);const C3DVector&GetPos()const;bool IsDead()const{return (reinterpret_cast<const unsigned char*>(this)[0x91]&1)!=0;}unsigned long GetTypeFlags()const{return reinterpret_cast<const CThingFields*>(this)->ThingTypeFlags;}static unsigned long __fastcall StatGetTypeFlags(){return 0;}
private:unsigned char _pad_members[sizeof(CThingFields)-sizeof(CClass)];
};
class CThingPhysical:public CThing{
public:virtual bool IsSolidToThing(const CThingPhysical*) const;
public:virtual bool IsAffectedByDamage() const;
public:virtual unsigned long GetProperties() const;
public:virtual bool IsCrop() const;
public:virtual bool IsHidden() const;
public:virtual bool IsTargetedByPlayer(long) const;
public:virtual void SetTargetedByPlayer(long);
public:virtual void ClearTargetedByPlayer(long);
public:virtual bool IsFragile() const;
public:virtual bool CanBlockNavigatingCreature() const;
public:virtual bool CanBlockUpperLevelNavigatingCreature() const;
public:virtual bool UseHighDetailQuadTree() const;
public:virtual CDefPointer<CMaterialDef const > PeekPMaterial() const;
public:virtual void ModifyHealth(float,bool);
private:unsigned char _pad_members[sizeof(CThingPhysicalFields)-sizeof(CThing)];
};
class CThingGameObject:public CThingPhysical{protected:virtual bool OnCreate();
public:virtual bool DrawIsTargeted() const;
public:virtual bool IsUsingTriggerRadius() const;
public:virtual float GetTriggerRadius() const;
public:virtual void SetTriggerRadius(float);
public:virtual bool IsCover() const;
public:virtual void OnHit(CHitParameters&);
public:virtual bool IsGameObject() const;
public:virtual bool IsIndestructible() const;
public:virtual void ForceAnim(long,bool);
public:virtual void ForceAnim(const CCharString&,bool);
public:virtual long GetMaterialOnHitByMelee() const;
private:unsigned char _pad_members[sizeof(CThingGameObjectFields)-sizeof(CThingPhysical)];
};

#include "engine/CBaseClassNonCopyable.h"
#define CTCBase CTCBaseFields
#include "engine/CTCBase.h"
#undef CTCBase
#define CTCPhysicsBase CTCPhysicsBaseFields
#include "engine/CTCPhysicsBase.h"
#undef CTCPhysicsBase
#define CTCGraphicAppearance CTCGraphicAppearanceFields
#include "engine/_quarantine/CTCGraphicAppearance.h"
#undef CTCGraphicAppearance
#define CThingCreatureDef CThingCreatureDefFields
#include "engine/_quarantine/CThingCreatureDef.h"
#undef CThingCreatureDef
#define CTCTargeted CTCTargetedFields
#include "engine/CTCTargeted.h"
#undef CTCTargeted
class CRightHandedSet{protected:C3DVector Up,Forward;public:CRightHandedSet(const CRightHandedSet&r):Up(r.Up),Forward(r.Forward){}};
class CCharStringData;class CCharString{protected:CCharStringData*PStringData;public:CCharString(const char*,long=-1);~CCharString();};
class CWorldMap;enum ETCInterfaceType{};class C2DVector;class CTCParamsBase;class CEnginePrimitivePrepareInfo;class CPersistContext;namespace NTCMessages{class CMessageBase;}
// Layout: shared CTCBase prefix; native table omits donor editor-only property transfer pair.
class CTCBase:public CBaseClassNonCopyable{protected:const CWorldMap&PeekWorldMap()const;CThing&ParentThing;private:bool ToBeRemoved,IsInGlobalUpdateTCs;
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
class CTCPhysicsBase:public CTCBase{protected:C3DVector Position;private:unsigned char unreadPhysics[0x40-0x18];public:const C3DVector&GetPosition()const{return Position;}void SetAsMoving(bool);
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


// Layout: supplied graphic projection is14 bytes at44; copying its real three words and final two bytes avoids adding packing.
extern "C" void*__cdecl memcpy(void*,const void*,size_t);
#pragma intrinsic(memcpy)
namespace NGraphicAppearance{class CTCGraphicAppearance:public CTCGraphicAppearanceFields{public:void SetScale(float);void SetMainGraphic(const CEngineGraphic&g){memcpy(&MainGraphic_BankIndex,&g,14);}};}
class CThingCreatureDef:public CThingCreatureDefFields{};
class CTCParamsBase:public CBaseClassNonCopyable{};
class CTCParamsPhysicsBase:public CTCParamsBase{public:C3DVector Position;CRightHandedSet Rhs;__declspec(noinline) CTCParamsPhysicsBase(const C3DVector&,const CRightHandedSet&);};

CTCParamsPhysicsBase::CTCParamsPhysicsBase(const C3DVector&p,const CRightHandedSet&r):Position(p),Rhs(r){}
class CTCTargeted:public CTCTargetedFields{public:void SetTargetingFlags(unsigned long);void SetTargetable(bool b){Targetable=b;}};
class CTCDiscovery:public CTCBase{public:static ETCInterfaceType __fastcall StatGetInterfaceType(){return (ETCInterfaceType)64;}};
class CBaseObjectPointer;class CBaseIntelligentPointer{private:CBaseObjectPointer*PData;public:CBaseIntelligentPointer();virtual~CBaseIntelligentPointer();CBaseObject*GetPItem()const;void SetPItem(CBaseObject*);};
class CThingCreatureBase:public CThingGameObject{};
template<class T>class CIntelligentPointer:public CBaseIntelligentPointer{public:CIntelligentPointer(T*p){SetPItem(const_cast<CBaseObject*>(static_cast<const CBaseObject*>(p)));}CIntelligentPointer&operator=(const CIntelligentPointer&x){SetPItem(x.GetPItem());return *this;}};
#include "engine/CArray.h"
template<class K,class V>class CKeyPairCompareLess{};template<class K,class V,class C=CKeyPairCompareLess<K,V> >class CVectorMap:public CArray<std::pair<K,V> >{private:C Compare;bool Dirty;public:std::pair<K,V>*LowerBound(const K&);};
template<class T>__forceinline bool CThing::GetTC(T**out){ETCInterfaceType type=T::StatGetInterfaceType();CThingFields*fields=reinterpret_cast<CThingFields*>(this);if(reinterpret_cast<unsigned long*>(fields->UsingComponents)[type>>5]&(1UL<<(type&31))){ETCInterfaceType key=type;CVectorMap<ETCInterfaceType,CTCBase*>&map=reinterpret_cast<CVectorMap<ETCInterfaceType,CTCBase*>&>(fields->Components);std::pair<ETCInterfaceType,CTCBase*>*i=map.LowerBound(key);if(i==map.end()||type<i->first)i=map.end();*out=static_cast<T*>(i->second);return true;}return false;}
class CThingBodyReorienter{private:bool BodyReorientationDone,ReorientingBody;long ReorientationCount;bool StillCheckingForNewBodyReorientationAngle,HaveFinalReorientationAngle;float FinalReorientationAngleXY;enum ERotationDirection{};ERotationDirection RotationDirection;bool HasPossibleIntersectingThings;std::vector<CIntelligentPointer<CThing> >PossibleIntersectingThings;CThing&Body;public:CThingBodyReorienter(CThing&);~CThingBodyReorienter();};
void __cdecl operator delete(void*);
namespace boost{template<class T>class scoped_ptr{private:T*ptr;public:explicit scoped_ptr(T*p=0):ptr(p){}~scoped_ptr(){delete ptr;}void reset(T*p=0){T*old=ptr;ptr=p;delete old;}};}
class CMainGameComponent{protected:static long ConstantFPS;public:static long __fastcall GetConstantFPS(){return ConstantFPS;}};
class CAnimFilterBase;
// Layout: shared game-object prefixd0, original counted filter8 atd0, intelligent killer8 atd8, timer e0/scoped owner e4/started e8.
class CThingDeadCreature:public CThingGameObject{protected:CCountedPointer<CAnimFilterBase>PAnimFilter;CIntelligentPointer<const CThingCreatureBase>PKiller;long BodyDieTimer;boost::scoped_ptr<CThingBodyReorienter>BodyReorienter;bool StartedDecay;const CThingCreatureDef&PeekDefinition()const;void PreloadDeadAnimation();void SetupDeadAnimation();virtual bool OnCreate();};
FABLE_STATIC_ASSERT(sizeof(CThingBodyReorienter)==40);
bool CThingDeadCreature::OnCreate(){
 CThingFields*fields=reinterpret_cast<CThingFields*>(this);
 NGraphicAppearance::CTCGraphicAppearance*appearance=reinterpret_cast<NGraphicAppearance::CTCGraphicAppearance*>(fields->PTCGraphicAppearance);
 StartedDecay=false;
 if(appearance){
  appearance->SetMainGraphic(reinterpret_cast<const CEngineGraphic&>(PeekDefinition().Graphic_BankIndex));
  appearance->SetScale(reinterpret_cast<CThingPhysicalFields*>(this)->Scale);
  if(CThingGameObject::OnCreate()){
   PreloadDeadAnimation();
   C3DVector position=fields->PhysicsTC->GetPosition();
   CRightHandedSet rhs=fields->PhysicsTC->GetRHSet();
   CTCParamsPhysicsBase params(position,rhs);
   AddTC("CTCPhysicsStandard",true,&params);
   fields->PhysicsTC->SetAsMoving(false);
   fields->PhysicsTC->SetInteractionFlags(0x10a);
   AddTC("CTCDiscovery",false,0);
   CTCDiscovery*discovery;GetTC(&discovery);discovery->OnCreate();
   CTCTargeted*targeted=reinterpret_cast<CTCTargeted*>(AddTC("CTCTargeted",false,0));targeted->SetTargetingFlags(4);targeted->SetTargetable(true);
   PKiller=CIntelligentPointer<const CThingCreatureBase>(0);
   SetupDeadAnimation();
   reinterpret_cast<CThingPhysicalFields*>(this)->MaxHealth=reinterpret_cast<CThingPhysicalFields*>(this)->Health=CMainGameComponent::GetConstantFPS()*PeekDefinition().NumSecondsToPersistOnDeath;
   fields->PausedDuringCutscenes[0]&=~0x10;fields->PausedDuringCutscenes[0]&=~0x20;
   BodyReorienter.reset(new CThingBodyReorienter(*this));return true;
  }
 }
 return false;
}