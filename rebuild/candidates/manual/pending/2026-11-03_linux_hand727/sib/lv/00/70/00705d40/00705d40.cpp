// 00705d40 CTCCarryable::SetAsCarriedBy
// Leaf: ?SetAsCarriedBy@CTCCarryable@@AAEXPAVCTCCarrying@@@Z
// Flags:
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CDefPointer.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CParentDefClassBase.h"
#include "engine/CCountedPointer.h"
#define CThing CThingFields
#include "engine/_quarantine/CThing.h"
#undef CThing
#define CTCBase CTCBaseFields
#include "engine/CTCBase.h"
#undef CTCBase
#define CTCPhysicsBase CTCPhysicsBaseFields
#include "engine/CTCPhysicsBase.h"
#undef CTCPhysicsBase
#define CTCGraphicAppearance CTCGraphicAppearanceFields
#include "engine/_quarantine/CTCGraphicAppearance.h"
#undef CTCGraphicAppearance
#define CCarryableDef CCarryableDefFields
#include "engine/CCarryableDef.h"
#undef CCarryableDef
#define CWorldMap CWorldMapFields
#include "engine/CWorldMap.h"
#undef CWorldMap
#define CDefinitionManager CDefinitionManagerFields
#include "engine/CDefinitionManager.h"
#undef CDefinitionManager
class C3DVector{public:float X,Y,Z;};class CRightHandedSet{protected:C3DVector Up,Forward;};
class C2DCoordI{public:long X,Y;C2DCoordI(long x,long y):X(x),Y(y){}};
class CCharStringData;class CCharString{protected:CCharStringData*PStringData;public:CCharString(const char*,long=-1);~CCharString();};
class CWorldMap;
enum ETCInterfaceType{};class C2DVector;class CTCParamsBase;class CEnginePrimitivePrepareInfo;class CPersistContext;namespace NTCMessages{class CMessageBase;}
// Layout: shared CTCBase prefix; native table omits donor editor-only property transfer pair.
class CTCBase:public CBaseClassNonCopyable{protected:CWorldMap&GetWorldMap();CThing&ParentThing;public:CThing&GetParentThing(){return ParentThing;}const CThing&GetParentThing()const{return ParentThing;}private:bool ToBeRemoved,IsInGlobalUpdateTCs;
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
class CTCPhysicsBase:public CTCBase{protected:C3DVector Position;private:unsigned char unreadPhysics[0x40-0x18];public:const C3DVector&GetPosition()const{return Position;}
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
public:virtual const CRightHandedSet&GetLastRHSet()const;
public:virtual void SetFacingAnglesFromRHSet(const CRightHandedSet&);
public:virtual void SetRHSet(const CRightHandedSet&);
public:virtual C3DVector GetImpulse()const;
public:virtual void SetRHSetAsFlushOnNormal(const C3DVector&);
public:virtual void SetRHSetAsFlushOnGround();
public:virtual float GetRenderZOffset(float)const;
public:virtual void ResetInterpolation();
};

namespace NGraphicAppearance{
// Layout: shared graphics header Alpha5a and bitgroup59; original ForceResendEveryRenderFrame is bit1 in donor71/native59.
class CTCGraphicAppearance:public CTCGraphicAppearanceFields{public:void RemoveAllMeshEffectsInheritedFromCarrier();void InheritMeshEffectsFromCarrier(const CTCGraphicAppearance*);void SetAlpha(unsigned char);unsigned char GetAlpha()const{return Alpha;}void SetForceResendEveryRenderFrame(bool b){if(b)PrimitivesDrawn[0]|=2;else PrimitivesDrawn[0]&=~2;}};
}
namespace std{template<class T>class allocator{};template<class A,class B>struct pair{A first;B second;};template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};template<class T,class A>class _Vector_base{protected:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:T*end(){return this->_M_finish;}};}
template<class T>class CArray:public std::vector<T>{};template<class K,class V>class CKeyPairCompareLess{};template<class K,class V,class C=CKeyPairCompareLess<K,V> >class CVectorMap:public CArray<std::pair<K,V> >{C Compare;bool Dirty;public:std::pair<K,V>*LowerBound(const K&);};
class CThingBaseDef:public CParentDefClassBase{};
// Layout: shared native CThing PhysicsTC60, graphics64, base-definition70 and component map44/bitset20.
class CThing:public CThingFields{public:const C3DVector&GetPos()const;void SetCurrentMapNumber(long);void SetThingScanInclusion(bool);
 const CThingBaseDef&PeekBaseDef()const{return *reinterpret_cast<const CThingBaseDef*>(PBaseDef_Object);}
 CTCPhysicsBase*GetPhysicsTC(){return PhysicsTC;}
 NGraphicAppearance::CTCGraphicAppearance*GetTCGraphicAppearance(){return reinterpret_cast<NGraphicAppearance::CTCGraphicAppearance*>(PTCGraphicAppearance);}
 __forceinline bool HasTC(ETCInterfaceType kind)const{if(reinterpret_cast<const unsigned long*>(UsingComponents)[kind>>5]&(1UL<<(kind&31)))return true;return false;}
 template<class T>__forceinline bool GetTC(T**out){ETCInterfaceType kind=T::StatGetInterfaceType();if(HasTC(kind)){*out=static_cast<T*>(&GetTC(kind));return true;}return false;}
 __forceinline CTCBase&GetTC(ETCInterfaceType kind){ETCInterfaceType key=kind;CVectorMap<ETCInterfaceType,CTCBase*>&map=reinterpret_cast<CVectorMap<ETCInterfaceType,CTCBase*>&>(Components);std::pair<ETCInterfaceType,CTCBase*>*it=map.LowerBound(key);return *((it==map.end()||kind<it->first)?map.end():it)->second;}
};
class CTCWeaponTrail:public CTCBase{public:void ClearAllWeaponTrails();};
class CTCVolumeContainmentTracker:public CTCBase{public:static ETCInterfaceType __fastcall StatGetInterfaceType(){return static_cast<ETCInterfaceType>(0x75);}void OnThingCarried(CThing&);};
class CDrawMapInfo;class C2DBoxI;class CAFile;class CASuspendableProcess;class CMap;
class CIDrawWorldMap{public:
 virtual long DrawGetNoMaps()const=0;
 virtual CDrawMapInfo DrawGetMapInfo(long)const=0;
 virtual C2DBoxI DrawGetDimensions()const=0;
 virtual long DrawGetGranularity()const=0;
 virtual bool DrawIsPosInMap(const C3DVector&)const=0;
 virtual float DrawGetSizeZAt(const C3DVector&,float)const=0;
 virtual C3DVector DrawConvertWorldMapCoordsToLocalCoords(long,const C3DVector&)const=0;
 virtual long DrawGetMyPlayerMapNumber()const=0;
 virtual bool DrawFindLastUnblockedGroundPositionAndReportCollision(const C3DVector&,const C3DVector&,C3DVector&)const=0;
 virtual bool DrawFindLastUnblockedThemePositionAndReportCollision(const C3DVector&,const C3DVector&,C3DVector&)const=0;
 virtual long DrawGetFrame()const=0;
 virtual~CIDrawWorldMap();
};
class CWorldMap:public CBaseClassNonCopyable,public CIDrawWorldMap{public:
 virtual~CWorldMap();
 virtual void DeleteAllLevels();virtual void DeleteAllRegions();
 virtual void LoadFromFile(CAFile&,long);virtual void SaveToFile(CAFile&);virtual void RemoveLevel(long);
 virtual void LoadTopologyFromFile(long,CCountedPointer<CAFile>,CASuspendableProcess&);virtual void PostLoadTopology(long);
 virtual void ForceLoad(long);virtual void ForceUnload(long);virtual bool IsMapLoaded(long)const;virtual bool IsMapUsed(long)const;
 virtual long GetGranularity()const;virtual const C2DBoxI&GetDimensions()const;
 virtual CMap&GetMapAt(const C2DCoordI&);virtual const CMap&PeekMapAt(const C2DCoordI&)const;
 virtual long GetMapNumberAt(const C2DCoordI&)const;virtual long GetMapNumberAt(const C3DVector&)const;
};
// Layout: CCarryableDef original subdef base25, named fields28..4c agree with shared header.
class CCarryableDef:public CSubDefClassBase{public:long ActiveCarrySlot,SecondaryActiveCarrySlot,PassiveCarrySlot,OnKillFX;C3DVector OffsetCoordRelativeToAttachToDummy;float OffsetAmountForThingsOnTopOfMe,PassiveCarrySlotScale;bool IncludeInThingScansWhenCarried;static CCharString __fastcall StatGetTypeName(){return CCharString("CCarryableDef");}};
class CDefinitionManager:public CDefinitionManagerFields{public:CDefPointer<CDefClassBase>GetPDefFromGlobalIndex(long);};
template<class T>__declspec(noinline)bool CDefClassBase::GetSubDef(CDefPointer<const T>*out)const{const CSubDefInfo*info=GetPSubDefInfo(T::StatGetTypeName());if(!info)return false;CDefPointer<const T>value(PDefManager->GetPDefFromGlobalIndex(info->DefIndex));out->SetDynamicCast(value);return true;}
class CWorld;
// Layout: native CarrySlotsc..1b; donor vector extra word shifts BypassFilters20 to1c. Only this prefix is accessed.
class CTCCarrying:public CTCBase{private:unsigned char unreadCarrySlots[0x10];bool BypassFilters;public:bool IsAllCarriedThingsBypassFilter()const{return BypassFilters;}void ResetAllCarriedThingsAsBypassFilter();};
// Layout: original CTCCarryable matches CTCBase12, ForceFragilityc, PCarryingTC10, vector14, float20, size24.
class CTCCarryable:public CTCBase{public:bool ForceFragility;void UpdateCarriedPosition(bool);private:CTCCarrying*PCarryingTC;C3DVector OffsetCoordRelativeToAttachToDummy;float OffsetAmount;void SetAsCarriedBy(CTCCarrying*);};
void CTCCarryable::SetAsCarriedBy(CTCCarrying* carrier)
{
    if (ParentThing.HasTC(static_cast<ETCInterfaceType>(0x4c)))
        static_cast<CTCWeaponTrail&>(ParentThing.GetTC(static_cast<ETCInterfaceType>(0x4c))).ClearAllWeaponTrails();
    if (carrier == 0) {
        UpdateCarriedPosition(true);
        ParentThing.GetPhysicsTC()->ResetInterpolation();
        PCarryingTC = 0;
        ParentThing.GetPhysicsTC()->EnablePhysics(true);
        ParentThing.SetThingScanInclusion(true);
        CWorldMap& map = GetWorldMap();
        const C3DVector& pos = ParentThing.GetPos();
        C2DCoordI coord((long)pos.X, (long)pos.Y);
        CThing&parent = ParentThing;
        parent.SetCurrentMapNumber(map.GetMapNumberAt(coord));
        if (ParentThing.GetTCGraphicAppearance())
            ParentThing.GetTCGraphicAppearance()->RemoveAllMeshEffectsInheritedFromCarrier();
        return;
    }
    bool persist = false;
    CDefPointer<const CCarryableDef> def;
    if (ParentThing.PeekBaseDef().GetSubDef(&def))
        persist = def->IncludeInThingScansWhenCarried;
    ParentThing.GetPhysicsTC()->EnablePhysics(false);
    ParentThing.SetThingScanInclusion(persist);
    ParentThing.GetTCGraphicAppearance()->SetForceResendEveryRenderFrame(true);
    PCarryingTC = carrier;
    if (carrier->IsAllCarriedThingsBypassFilter())
        carrier->ResetAllCarriedThingsAsBypassFilter();
    NGraphicAppearance::CTCGraphicAppearance* carrierGraphic = carrier->GetParentThing().GetTCGraphicAppearance();
    NGraphicAppearance::CTCGraphicAppearance* graphic = ParentThing.GetTCGraphicAppearance();
    if (carrierGraphic && graphic)
        graphic->InheritMeshEffectsFromCarrier(carrierGraphic);
    {CTCVolumeContainmentTracker*tracker;
    if (carrier->GetParentThing().GetTC(&tracker))
        tracker->OnThingCarried(ParentThing);}
    if (carrier->GetParentThing().GetTCGraphicAppearance())
        ParentThing.GetTCGraphicAppearance()->SetAlpha(carrier->GetParentThing().GetTCGraphicAppearance()->GetAlpha());
    CTCPhysicsBase* physics = ParentThing.GetPhysicsTC();
    physics->ResetPosition(carrier->GetParentThing().GetPhysicsTC()->GetPosition());
    physics->ResetRHSet(carrier->GetParentThing().GetPhysicsTC()->GetRHSet());
    physics->ResetInterpolation();
}