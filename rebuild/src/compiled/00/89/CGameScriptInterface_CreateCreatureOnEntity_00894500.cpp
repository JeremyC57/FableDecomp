// 00894500 CGameScriptInterface::CreateCreatureOnEntity
// Leaf: ?CreateCreatureOnEntity@CGameScriptInterface@@UBE?AVCScriptThing@@ABVCCharString@@ABV2@0@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Flags:
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CDefPointer.h"
#include "engine/CDefClassBase.h"
#define CDefinitionManager CDefinitionManagerFields
#include "engine/CDefinitionManager.h"
#undef CDefinitionManager
#define CTCBase CTCBaseFields
#include "engine/CTCBase.h"
#undef CTCBase
#define CTCPhysicsBase CTCPhysicsBaseFields
#include "engine/CTCPhysicsBase.h"
#undef CTCPhysicsBase
#define CThing CThingFields
#include "engine/_quarantine/CThing.h"
#undef CThing
#include "engine/_quarantine/CThingCreatureDef.h"
class CCharStringData;
class CCharString{protected:CCharStringData*PStringData;public:CCharString(const char*,long=-1);CCharString(const CCharString&);~CCharString();const CCharString&operator=(const CCharString&);};
#include "engine/C3DVector.h"
class CRightHandedSet;
inline CScriptThing::CScriptThing(){}
inline CScriptThing::CScriptThing(const CScriptThing&o):CBaseClass(o),PImp(o.PImp){}
inline CScriptThing::~CScriptThing(){}
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

// Layout: shared native CThing prefix and original const script conversion.
class CThing:public CThingFields{public:CScriptThing GetScriptThing()const;CTCPhysicsBase*GetPhysicsTC(){return PhysicsTC;}const CTCPhysicsBase*GetPhysicsTC()const{return PhysicsTC;}};
class CThingPhysical:public CThing{};class CThingGameObject:public CThingPhysical{};class CThingCreatureBase:public CThingGameObject{};
class CDefinitionManager:public CDefinitionManagerFields{public:long GetDefGlobalIndexFromName(const CCharString&)const;CDefPointer<const CDefClassBase>GetPDefFromGlobalIndex(long)const;template<class T>bool GetDef(long,CDefPointer<const T>*)const;};
class CGameDefinitionManager:public CDefinitionManager{};
class CThingBuilding;
// Layout: original28-byte init fields and exact six-parameter constructor; native004facd0 proves arg6 Drawable.
class CThingAICreatureInit{public:CThingPhysical*OriginalThing,*PreviousThing;CCharString ScriptName,OverridingBrainName;bool PlayerCharacter,Drawable,PlaceAtEndOfList,EnableCreatureAutoPlacing;CThingBuilding*HomeBuilding,*WorkBuilding;
 CThingAICreatureInit(CThingPhysical*original=0,CThingPhysical*previous=0,bool player=false,const CCharString&script=CCharString(""),const CCharString&brain=CCharString(""),bool drawable=true):OriginalThing(original),PreviousThing(previous),ScriptName(script),OverridingBrainName(brain),PlayerCharacter(player),Drawable(drawable),PlaceAtEndOfList(true),EnableCreatureAutoPlacing(false),HomeBuilding(0),WorkBuilding(0){}
};
class CThingAICreature:public CThingCreatureBase{public:static CThingAICreature*__fastcall Create(long,const C3DVector&,long,const CThingAICreatureInit&);};
template<class T>__declspec(noinline)bool CDefinitionManager::GetDef(long index,CDefPointer<const T>*out)const{if(index<=0)return false;CDefPointer<const CDefClassBase>value=GetPDefFromGlobalIndex(index);if(value.IsNull())return false;out->SetDynamicCast(value);return true;}
CScriptThing CGameScriptInterface::CreateCreatureOnEntity(const CCharString&defName,const CScriptThing&entity,const CCharString&scriptName)const{
 CScriptThing result;
 if(entity.IsAlive()){
  long index=DefinitionManager->GetDefGlobalIndexFromName(defName);
  CThingAICreatureInit init(0,0,false,"","",true);init.ScriptName=scriptName;
  CDefPointer<const CThingCreatureDef>def;DefinitionManager->GetDef(index,&def);
  const CThingCreatureDef*creatureDef=reinterpret_cast<const CThingCreatureDef*>(def.InternalGetObject());
  CThingAICreature*creature=CThingAICreature::Create(index,entity.GetPos(),creatureDef->DefaultOwner,init);
  if(creature){CTCPhysicsBase*physics=creature->GetPhysicsTC();if(physics){CTCPhysicsBase*entityPhysics=entity.GetPThing()->GetPhysicsTC();if(entityPhysics)physics->ResetRHSet(entityPhysics->GetRHSet());}
  result=creature->GetScriptThing();
  }
  CDefPointeeBase*object=def.InternalGetObject();if(object->Release()==0)object->Delete();def.Object=0;
 }
 return result;
}