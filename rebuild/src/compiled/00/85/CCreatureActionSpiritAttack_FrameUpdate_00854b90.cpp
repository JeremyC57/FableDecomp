// CCreatureAction_SpiritAttack::FrameUpdate() -- per-frame update of the co-op spirit attack: while the target and the particle emitter are alive, feed the emitter LIGHTNING_START (spirit focal point +0.8 Z) and LIGHTNING_END (target focal point) vector parameters
// Leaf: ?FrameUpdate@CCreatureAction_SpiritAttack@@EAEXXZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Grade: RELOCATION_MATCH against the TRUE extent (own oracle: Fable.exe 0x00854B90..0x00854DC5, 566 bytes, `ret` at
//   0x00854DC5, int3 to 0x00854DD0 where ?PerformAction@CCreatureAction_SpiritAttack starts); the catalog/oracle row is a
//   defective 21-byte span (ends mid-instruction inside the body). Report for a manifest split/recut to 566 bytes.
// Identity: vtable CCreatureAction_SpiritAttack 0x01275D74 slot 56 = 0x00854B90 (rtti-vtables.tsv); Ego_r.pdb
//   ?FrameUpdate@CCreatureAction_SpiritAttack@@EAEXXZ (private virtual). Body: ChatGPT pass 10's reviewed 566-byte source
//   (its attempt 11), re-graded here (QLEN=566, bin/qx.py); every bound callee re-checked against the PDB name sets and
//   reloc_bindings.tsv (bin/names.py): CCreatureActionBase::FrameUpdate 0x00694020, CBaseIntelligentPointer::GetPItem
//   0x00A01B50, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (STLport pointer-iterator spelling, the PDB's
//   is Dinkumware), CTCCoopSpirit::GetFocalPos 0x0066FFD0, CTCGraphicAppearance::GetFocalPos 0x004C1060, CCRC::Calc
//   0x004014A0, CBaseClass ctor 0x0099A2D0, vector<pair<unsigned long,C3DVector>>::_M_insert_overflow 0x0076ADA0,
//   CParticleUpdateParameters<unsigned long,unsigned long>::Update 0x0076C750 / dtor 0x006E2500,
//   CTCDParticleEmitter::SetEmitterUpdateParameters 0x006E11A0.
#include "fable_compiler.h"
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
public:template<class T>bool GetTC(T**);CTCBase*AddTC(const CCharString&,bool,const CTCParamsBase*);const C3DVector&GetPos()const;bool IsDead()const{return (reinterpret_cast<const CThingFields*>(this)->CreatedBySerialisation[0]&1)!=0;}unsigned long GetTypeFlags()const{return reinterpret_cast<const CThingFields*>(this)->ThingTypeFlags;}static unsigned long __fastcall StatGetTypeFlags(){return 0;}
private:unsigned char _pad_members[sizeof(CThingFields)-sizeof(CClass)];
};

#include "engine/CBaseClass.h"
#define CCreatureActionBase CCreatureActionBaseFields
#include "engine/CCreatureActionBase.h"
#undef CCreatureActionBase
#define CCreatureAction_SpiritAttack CCreatureAction_SpiritAttackFields
#include "engine/CCreatureAction_SpiritAttack.h"
#undef CCreatureAction_SpiritAttack
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
// Layout: original base prefix grounded in shared native CCreatureActionBase header; PCreature is protected at8.
class CTCWeaponTrail;
class CCreatureActionBase:public CBaseObject{protected:CIntelligentPointer<CThingCreatureBase>PCreature;unsigned char _pad_action[sizeof(CCreatureActionBaseFields)-16];public:virtual void EditUpdate(float);
public:virtual void PerformAction();
public:virtual void FinishAction();
public:virtual bool IsPlayingAnimation() const;
public:virtual long GetAnimDelayTime() const;
public:virtual long GetAnimationIndex() const;
public:virtual long GetAnimationTransitionInTime() const;
public:virtual long GetAnimationTransitionOutTime() const;
public:virtual unsigned long GetAnimExtraFlags() const;
public:virtual CCreatureActionBase* Clone() const;
public:virtual void Initialise();
public:virtual bool IsStrikeEvent(long) const;
public:virtual bool IsStrikeEventActive() const;
public:virtual bool IsStrikeAction() const;
public:virtual bool IsPartOfComboChain() const;
public:virtual bool IsStrikeEventFinishedBeingActive() const;
public:virtual bool IsVitalStrike() const;
public:virtual bool IsKnockdownStrike() const;
public:virtual bool IsBlockCounter() const;
public:virtual bool IsMeleeFlourish() const;
public:virtual bool IsActionWithStrikeEventHelperName() const;
public:virtual CCharString GetStrikeEventHelperName() const;
public:virtual bool IsOverrideableByActionOfSamePriority() const;
public:virtual void OnFrameUpdate(float);
public:virtual bool IsQueuable() const;
public:virtual bool IsActionReadyForInterruption() const;
public:virtual bool IsActionReadyForInterruptionByMovement(const C3DVector&) const;
public:virtual bool IsBlockable() const;
public:virtual bool IsAffectedByHaste() const;
public:virtual bool IsAffectedByHeroStats() const;
public:virtual bool IsPhysicsPushableByHero() const;
public:virtual bool IsAlwaysPhysicsPushableBySelfAvoidance() const;
public:virtual bool IsAlwaysUseAnimationMovement() const;
public:virtual bool IsClearingMovementVector() const;
public:virtual bool IsToHaveEmptyHands() const;
public:virtual bool IsToReplaceItemIfRemoved() const;
public:virtual bool IsAllowingAnimationMovementToMoveSpeakAndWaitCameras() const;
public:virtual bool IsPreventingQuickAccessItemUsage() const;
public:virtual void SetHitter(const CThing&);
public:virtual void SetHitDirection(C3DVector);
public:virtual void SetHitPosition(C3DVector);
public:virtual CTCWeaponTrail* GetWeaponTrailUsedInStrikeAction(long&) const;
public:virtual float GetTotalTimeSeconds() const;
public:virtual bool IsToEvadeAllHits() const;
public:virtual bool IsToCauseRecoil() const;
public:virtual bool IsAbleToBeAttacked() const;
public:virtual bool IsPlayHitResponse() const;
public:virtual bool IsPreventingNormalMovement() const;
public:virtual bool IsPreventingLookTurning() const;
public:virtual void OnBlowBlocked(bool);
public:virtual void OnFollowUpActionFailedToAdd();
public:virtual void UpdateActionRunningTime();
public:virtual bool IsDecapitatingAttack() const;
public:virtual bool PreventSneaking() const;
protected:virtual void FrameUpdate();};
class CCreatureAction_SpiritAttack:public CCreatureActionBase{private:CIntelligentPointer<CThing>PTarget,PParticleEmitter;virtual void FrameUpdate();};
FABLE_STATIC_ASSERT(sizeof(CCreatureAction_SpiritAttack)==sizeof(CCreatureAction_SpiritAttackFields));
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

#define CThingCreatureBase CThingCreatureBaseFields
#include "engine/CThingCreatureBase.h"
#undef CThingCreatureBase
// Layout: inherited shared native extents retain the original creature hierarchy; no creature-specific method is called.
class CThingCreatureBase:public CThingGameObject{private:unsigned char _pad_creature[sizeof(CThingCreatureBaseFields)-sizeof(CThingGameObject)];};

class CCRC{public:static unsigned long __fastcall Calc(unsigned long,const void*,unsigned long);};
inline void*operator new(unsigned int,void*p){return p;}
extern "C" void __cdecl free(void*);
namespace std{
struct __false_type{};
template<class A,class B>struct pair{A first;B second;pair(const A&a,const B&b):first(a),second(b){}};
template<class T>class allocator{public:allocator()throw(){}allocator(const allocator&)throw(){}template<class U>allocator(const allocator<U>&)throw(){}~allocator()throw(){}};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;_STLP_alloc_proxy(const A&a,V p):A(a),_M_data(p){}};
template<class T,class A>class _Vector_base{protected:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;public:_Vector_base(const A&a):_M_start(0),_M_finish(0),_M_end_of_storage(a,0){}~_Vector_base(){if(_M_start)free(_M_start);}};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:FABLE_FORCEINLINE explicit vector(const A&a=A()):_Vector_base<T,A>(a){}T*begin(){return this->_M_start;}T*end(){return this->_M_finish;}protected:void _M_insert_overflow(T*,const T&,const __false_type&,unsigned int,bool);public:FABLE_FORCEINLINE void push_back(const T&x){if(this->_M_finish!=this->_M_end_of_storage._M_data){new(this->_M_finish)T(x);++this->_M_finish;}else{__false_type tag;_M_insert_overflow(this->_M_finish,x,tag,1,true);}}};
}
extern "C" void*__cdecl malloc(unsigned int);
namespace std{
template<class T,class A>__declspec(noinline)void vector<T,A>::_M_insert_overflow(T*position,const T&x,const __false_type&,unsigned int fill,bool atEnd){
 unsigned int oldSize=this->_M_finish-this->_M_start;
 unsigned int length=oldSize+(oldSize>fill?oldSize:fill);
 T*start=static_cast<T*>(malloc(length*sizeof(T)));T*finish=start;
 for(T*p=this->_M_start;p!=position;++p,++finish)new(finish)T(*p);
 for(unsigned int n=0;n<fill;++n,++finish)new(finish)T(x);
 if(!atEnd)for(T*p=position;p!=this->_M_finish;++p,++finish)new(finish)T(*p);
 for(T*p=this->_M_start;p!=this->_M_finish;++p)p->~T();
 free(this->_M_start);
 this->_M_start=start;this->_M_finish=finish;this->_M_end_of_storage._M_data=start+length;
}
}
template<class T>class CArray:public std::vector<T>{};
template<class K,class V>class CKeyPairCompareLess{};
template<class K,class V,class C=CKeyPairCompareLess<K,V> >class CVectorMap:public CArray<std::pair<K,V> >{private:C Compare;bool Dirty;public:FABLE_FORCEINLINE CVectorMap():Dirty(false){}std::pair<K,V>*LowerBound(const K&);FABLE_FORCEINLINE void Insert(const std::pair<K,V>&value){this->push_back(value);Dirty=true;}};
enum ETCInterfaceType{};
template<class T>FABLE_FORCEINLINE bool CThing::GetTC(T**out){ETCInterfaceType type=T::StatGetInterfaceType();CThingFields*fields=reinterpret_cast<CThingFields*>(this);if(reinterpret_cast<unsigned long*>(fields->UsingComponents)[type>>5]&(1UL<<(type&31))){ETCInterfaceType key=type;CVectorMap<ETCInterfaceType,CTCBase*>&map=reinterpret_cast<CVectorMap<ETCInterfaceType,CTCBase*>&>(fields->Components);std::pair<ETCInterfaceType,CTCBase*>*i=map.LowerBound(key);if(i==map.end()||type<i->first)i=map.end();*out=reinterpret_cast<T*>(i->second);return true;}return false;}
namespace NParticleEngine{
template<class K,class V>class CParticleUpdateParamCallback;
template<class K,class V>class CParticleUpdateParamDesc:public CBaseClass{private:CParticleUpdateParamCallback<K,V>*Callback;V Index;};
template<class K,class V>class CParticleUpdateParameters:public CBaseClass{public:template<class T>class CParticleUpdateCCompareVector:public CKeyPairCompareLess<T,C3DVector>{};template<class T>class CParticleUpdateCCompareParamDesc:public CKeyPairCompareLess<T,CParticleUpdateParamDesc<K,V> >{};typedef CVectorMap<K,C3DVector,CParticleUpdateCCompareVector<K> >TVectorParams;typedef CVectorMap<K,CParticleUpdateParamDesc<K,V>,CParticleUpdateCCompareParamDesc<K> >TUpdateParams;TVectorParams VectorParams;TUpdateParams UpdateParams;FABLE_FORCEINLINE CParticleUpdateParameters(){}virtual~CParticleUpdateParameters();void Update();};
}
class CTCDParticleEmitter{public:static ETCInterfaceType __fastcall StatGetInterfaceType(){return (ETCInterfaceType)0x4b;}void SetEmitterUpdateParameters(const NParticleEngine::CParticleUpdateParameters<unsigned long,unsigned long>&);};
#define CTCCoopSpirit CTCCoopSpiritFields
#include "engine/_quarantine/CTCCoopSpirit.h"
#undef CTCCoopSpirit
class CTCCoopSpirit:public CTCCoopSpiritFields{public:static ETCInterfaceType __fastcall StatGetInterfaceType(){return (ETCInterfaceType)0xc7;}C3DVector GetFocalPos()const;};
#define CTCGraphicAppearance CTCGraphicAppearanceFields
#include "engine/_quarantine/CTCGraphicAppearance.h"
#undef CTCGraphicAppearance
namespace NGraphicAppearance{class CTCGraphicAppearance:public CTCGraphicAppearanceFields{public:C3DVector GetFocalPos(float)const;};}
void CCreatureAction_SpiritAttack::FrameUpdate(){
 CCreatureActionBase::FrameUpdate();
 if(PTarget.Get()!=0&&!PTarget->IsDead()&&PParticleEmitter.Get()!=0&&!PParticleEmitter->IsDead()){
  const CThing&target=*PTarget.Get();CTCDParticleEmitter*emitter=0;PParticleEmitter->GetTC(&emitter);const CTCCoopSpirit*tc_spirit=0;PCreature->GetTC(&tc_spirit);
  C3DVector start_pos=tc_spirit->GetFocalPos();start_pos.Z+=0.8f;
  C3DVector end_pos=reinterpret_cast<const NGraphicAppearance::CTCGraphicAppearance*>(reinterpret_cast<const CThingFields*>(&target)->PTCGraphicAppearance)->GetFocalPos(1.0f);
  std::pair<unsigned long,C3DVector>u_start_pos(CCRC::Calc(0,"LIGHTNING_START",15),start_pos);
  std::pair<unsigned long,C3DVector>u_end_pos(CCRC::Calc(0,"LIGHTNING_END",13),end_pos);
  NParticleEngine::CParticleUpdateParameters<unsigned long,unsigned long>update_params;
  update_params.VectorParams.Insert(u_start_pos);update_params.VectorParams.Insert(u_end_pos);
  update_params.Update();emitter->SetEmitterUpdateParameters(update_params);
 }
}