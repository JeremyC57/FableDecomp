// Self-contained test on real shared classes, with controlled external callees.
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#define __int64 long long
#define __declspec(x)
#define FABLE_PORTABLE
#include <strings.h>
#define _stricmp strcasecmp
enum ESex {};
#endif
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_RETAIL_STLP_HOST_TAGS_PASS23
#define FABLE_RETAIL_CTCPHYSICSBASE_ORIGINAL_CTOR_PASS23
#define FABLE_RETAIL_CDEFENDINGCOMBATANTINFO_PRONG
#define FABLE_RETAIL_CCOMBATMANAGER_CTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE_CTOR_DECL
#define private public
#define protected public
// 006e99b0 CCombatManager::GetDefenderZone
// Flags: none
// Leaf: ?GetDefenderZone@CCombatManager@@QBE?AW4ECombatZone@@ABVCThingCreatureBase@@0J@Z
#define FABLE_RETAIL_COMBAT_DEFENDER_ZONE_PASS23
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_GETCOMBATTYPEDEF
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_LASTPOSITION
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CCOMBATMANAGER
#define FABLE_RETAIL_CCOMBATMANAGER_OUTERRING
#define FABLE_RETAIL_CCOMBATTYPEDEF
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ACCURATE_MAGNITUDE
#define FABLE_RETAIL_C3DVECTOR_CROSSWITHUP
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_SQUARE_ORIGINAL_FORMAL
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#define FABLE_RETAIL_C3DVECTOR_NONZERO_SQUARED_XYZ
#define FABLE_RETAIL_C3DVECTOR_DOT_XY_THEN_Z
#define FABLE_RETAIL_STLP_TREE_MFIND_DECL
#define FABLE_RETAIL_STLP_TREE_CONSTFIND
#include "fable_compiler.h"
#include "engine/ECombatZone.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CCombatTypeDef.h"
#include "engine/CCombatManager.h"
#include "engine/GFCos.h"
// Original template signature in both PDBs; donor0241c049 uses<float,float>.
template<class T,class L>inline void FABLE_FASTCALL GFLimitLower(T& value,const L& lower){if(value<lower)value=lower;}

ECombatZone CCombatManager::GetDefenderZone(const CThingCreatureBase& attackee,const CThingCreatureBase& attacker,long prediction_frames)const
{
    if(IsAttacker(attacker) && IsCreatureOnOuterRing(attacker))
        return COMBAT_ZONE_OUTER_RING;
    // Fidelity: group all five floating call results; measured81-tuple boundary then
    // falls before the Y load, reproducing native6e9af4 Z-zero store without new operations.
    const float FRONT_FAR_LIMIT_SQR=(GFSqr(attacker.GetCombatTypeDef().FrontFarLimit+0.6f));
    const float FRONT_MIDDLE_LIMIT_SQR=(GFSqr(attacker.GetCombatTypeDef().FrontMiddleLimit));
    const float FRONT_NEAR_LIMIT_SQR=(GFSqr(attacker.GetCombatTypeDef().FrontNearLimit));
    const float REAR_LIMIT_SQR=(GFSqr(attacker.GetCombatTypeDef().RearLimit+0.6f));
    const float SIDE_LIMIT_SQR=(GFSqr(attacker.GetCombatTypeDef().SideLimit+0.6f));
    const CTCPhysicsBase* ptc_phys=attackee.GetPhysicsTC();
    C3DVector velocity=ptc_phys->GetPosition()-ptc_phys->GetLastPosition();
    C3DVector target_pos=ptc_phys->GetPosition();
    C3DVector predicted_position=target_pos+velocity*(float)prediction_frames;
    C3DVector facing_direction=attacker.GetPhysicsTC()->GetFacingVector();
    C3DVector perp=facing_direction.CrossWithUp();
    C3DVector between=predicted_position-attacker.PeekPos();
    between.Z=0.0f;
    float distance=between.GetAccurateMagnitude();
    if(distance>DELTA_NOTIONAL_ZERO)between/=distance;
    C3DVector static_between=target_pos-attacker.PeekPos();
    static_between.Z=0.0f;
    if(static_between.IsNonZero())static_between.AccurateNormalise();
    distance-=attackee.GetPhysicsTC()->GetRadius();
    GFLimitLower(distance,0.0f);
    float sqr_distance=GFSqr(distance);
    if(between.Dot(static_between)<0.0f && sqr_distance<SIDE_LIMIT_SQR)
        return between.Dot(perp)<0.0f?COMBAT_ZONE_LEFT:COMBAT_ZONE_RIGHT;
    float cos_theta=between.Dot(facing_direction);
    if(cos_theta>GFCos(0.125f)) {
        if(sqr_distance<FRONT_NEAR_LIMIT_SQR)return COMBAT_ZONE_FRONT_NEAR;
        if(sqr_distance<FRONT_MIDDLE_LIMIT_SQR)return COMBAT_ZONE_FRONT_MIDDLE;
        if(sqr_distance<FRONT_FAR_LIMIT_SQR)return COMBAT_ZONE_FRONT_FAR;
        return COMBAT_ZONE_NO_ZONE;
    }
    if(cos_theta<GFCos(0.375f)) {
        if(sqr_distance<REAR_LIMIT_SQR)return COMBAT_ZONE_REAR;
        return COMBAT_ZONE_NO_ZONE;
    }
    if(sqr_distance<SIDE_LIMIT_SQR)
        return between.Dot(perp)<0.0f?COMBAT_ZONE_LEFT:COMBAT_ZONE_RIGHT;
    return COMBAT_ZONE_NO_ZONE;
}

#include "engine/CWorld.h"
#include "engine/CRightHandedSet.h"
#include "engine/CDefString.h"
#include "engine/CPhysicsMeshObject.h"
#include "engine/CPhysicsDef.h"
#include "engine/CCombatDialogueDef.h"

// All empty-container specializations apply only to unused fixture storage.
namespace std {
template<> _Vector_base<CDefendingCombatantInfo::CCombatRing,allocator<CDefendingCombatantInfo::CCombatRing> >::~_Vector_base(){assert(!_M_start);}
template<> vector<CDefendingCombatantInfo::CCombatRing>::~vector(){assert(!_M_start);}
}
#undef private
#undef protected
CBaseClass::CBaseClass(){}
CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){}
CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CBaseObject::CBaseObject():IntelligentPointer(0){}
CBaseObject::~CBaseObject(){}
CBaseIntelligentPointer::CBaseIntelligentPointer():PData(0){}
CBaseIntelligentPointer::~CBaseIntelligentPointer(){}
CCharString::CCharString():PStringData(0){}
CCharString::~CCharString(){}
CWideString::CWideString():PStringData(0){}
CWideString::~CWideString(){}
CTCBase::CTCBase(CThing& thing):ParentThing(&thing){}
CTCBase::~CTCBase(){}
CTCPhysicsBase::CTCPhysicsBase(CThing& thing):CTCBase(thing){}
CTCPhysicsBase::~CTCPhysicsBase(){}
CCombatManager::CCombatManager(CWorld& world):World(world){}
CResource::CResource():ResourceList(0),PrevResource(0),NextResource(0),ResourceSize(0),LastUsedFrame(0){IVRefCount=1;}
CResource::~CResource(){}
CDefClassBase::CDefClassBase():PDefManager(0),GlobalIndex(0),Setup(false),Template(false),DefaultValsApplied(false){}
CParentDefClassBase::CParentDefClassBase(){}
CParentDefClassBase::~CParentDefClassBase(){}
void CIVCountedPointeeBase::Delete() const {abort();}
class FixtureCombatDef:public CCombatTypeDef {public:void Transfer(CPersistContext&){abort();}void Copy(const CDefClassBase*){abort();}unsigned long GetSizeofClass(){return sizeof(*this);}};
static FixtureCombatDef combatDef;
static C3DVector facing;
static bool outerRing=false;
static unsigned defCalls=0,outerCalls=0,peekCalls=0,facingCalls=0,normaliseCalls=0;
const float DELTA_NOTIONAL_ZERO=0.0001f;
const CCombatTypeDef& CThingCreatureBase::GetCombatTypeDef()const{++defCalls;return combatDef;}
const C3DVector& CThingPhysical::PeekPos()const{++peekCalls;return GetPhysicsTC()->GetPosition();}
bool CCombatManager::IsCreatureOnOuterRing(const CThingCreatureBase&)const{++outerCalls;return outerRing;}
// Native00a14440 rounds one reciprocal, then multiplies each component.
C3DVector& C3DVector::AccurateNormalise(){++normaliseCalls;float one_on_magnitude=1.0f/(float)sqrt(X*X+Y*Y+Z*Z);X*=one_on_magnitude;Y*=one_on_magnitude;Z*=one_on_magnitude;return *this;}
C3DVector CTCPhysicsBase::GetFacingVector()const{++facingCalls;return facing;}
C3DVector CTCPhysicsBase::GetFacingVector(float)const{abort();}
class FixturePhysics:public CTCPhysicsBase {
public:
 FixturePhysics(CThing& t):CTCPhysicsBase(t){}
 CCharString GetClassTypeName()const{abort();}
};
namespace std {
template<class K,class V,class KeyOfValue,class Compare,class A>
template<class Key> _Rb_tree_node<V>* _Rb_tree<K,V,KeyOfValue,Compare,A>::_M_find(const Key& key)const {
 _Link_type y=this->_M_header._M_data;_Link_type x=_M_root();
 while(x){if(!_M_key_compare(_S_key(x),key)){y=x;x=_S_left(x);}else x=_S_right(x);}
 if(y==this->_M_header._M_data||_M_key_compare(key,_S_key(y)))y=this->_M_header._M_data;
 return y;
}
}

// Unused virtual callees fail immediately if reached.
void CBaseObject::Validate() const { abort(); }
void CClass::Save(CCharString&) { abort(); }
void CThingGameObject::OnHit(CHitParameters&) { abort(); }
void CThingCreatureBase::OnHit(CHitParameters&) { abort(); }
bool CThingCreatureBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) { abort(); }
void CTCBase::OnSerialise(CPersistContext&, bool) { abort(); }
void CTCBase::OnRemove() { abort(); }
void CTCBase::ConstructFromParams(const CTCParamsBase&) { abort(); }
void CTCBase::OnCreate() { abort(); }
void CTCBase::InitialActivate() { abort(); }
void CTCBase::OnInitialActivate() { abort(); }
void CTCBase::RenderUpdate(float) { abort(); }
bool CTCBase::HasFrameUpdate() const { abort(); }
void CTCBase::FrameUpdate() { abort(); }
void CTCBase::BulletTimeFrameUpdate() { abort(); }
void CTCBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&) { abort(); }
void CTCBase::Copy(const CTCBase&) { abort(); }
void CTCBase::OnDie() { abort(); }
void CTCBase::OnKill() { abort(); }
void CTCBase::OnLevelUnload() { abort(); }
void CTCBase::OnSetInLimbo(bool) { abort(); }
bool CTCBase::Validate(CCharString&) const { abort(); }
bool CTCBase::IsFreeForDeletion() const { abort(); }
bool CTCBase::IsSerialiseCreatable() const { abort(); }
bool CTCBase::IsSerialisableForHeroState() const { abort(); }
bool CTCBase::IsComponentSerialisable() const { abort(); }
bool CTCBase::IsDriver() const { abort(); }
bool CTCBase::IsCopyable() const { abort(); }
void CTCBase::GlobalUpdate(bool) { abort(); }
void CTCBase::HandleMessage(const NTCMessages::CMessageBase&) { abort(); }
ETCInterfaceType CTCPhysicsBase::GetInterfaceType() const { abort(); }
C2DVector CTCPhysicsBase::GetOffsetVector() const { abort(); }
C3DVector CTCPhysicsBase::GetOffsetVector3D() const { abort(); }
void CTCPhysicsBase::SetPosition(const C3DVector &) { abort(); }
void CTCPhysicsBase::ResetPosition(const C3DVector &) { abort(); }
void CTCPhysicsBase::ResetRHSet(const CRightHandedSet &) { abort(); }
void CTCPhysicsBase::SetPositionToNavigateTo(const C3DVector &) { abort(); }
void CTCPhysicsBase::SetForcedToMovePos(const C3DVector &,bool) { abort(); }
void CTCPhysicsBase::ResetMovementState() { abort(); }
void CTCPhysicsBase::EnablePhysics(bool) { abort(); }
bool CTCPhysicsBase::IsPhysicsEnabled() const { abort(); }
void CTCPhysicsBase::EnableGravity(bool) { abort(); }
bool CTCPhysicsBase::IsGravityEnabled() const { abort(); }
void CTCPhysicsBase::EnableNavigator(bool) { abort(); }
bool CTCPhysicsBase::IsNavigationEnabled() const { abort(); }
bool CTCPhysicsBase::HasNavigationLayers() const { abort(); }
void CTCPhysicsBase::SetInteractionFlags(long) { abort(); }
void CTCPhysicsBase::ReturnToDefaultInteractionFlags() { abort(); }
void CTCPhysicsBase::OnDroppedFromUpdateList() { abort(); }
void CTCPhysicsBase::OnAddedToUpdateList() { abort(); }
bool CTCPhysicsBase::IsAlwaysFacingMovementDirection() const { abort(); }
bool CTCPhysicsBase::IsAffectedByGravity() const { abort(); }
bool CTCPhysicsBase::IsAffectedByAir() const { abort(); }
bool CTCPhysicsBase::IsAffectedByWind() const { abort(); }
bool CTCPhysicsBase::IsAngledWithGround() const { abort(); }
bool CTCPhysicsBase::IsImmoveable() const { abort(); }
bool CTCPhysicsBase::IsOnAnySurface() const { abort(); }
bool CTCPhysicsBase::IsOnAnyStationarySurface() const { abort(); }
bool CTCPhysicsBase::IsOnMeshSurface() const { abort(); }
bool CTCPhysicsBase::IsOnGroundSurface() const { abort(); }
bool CTCPhysicsBase::CanCollideWithThings() const { abort(); }
bool CTCPhysicsBase::CanCollideWithThingsOneWay() const { abort(); }
bool CTCPhysicsBase::CanCollideWithLandscape() const { abort(); }
bool CTCPhysicsBase::CanInteractWithThings() const { abort(); }
bool CTCPhysicsBase::IsExcludingCreatures() const { abort(); }
bool CTCPhysicsBase::IsPhysicsMeshImpassable() const { abort(); }
float CTCPhysicsBase::GetFacingAngleXY() const { abort(); }
float CTCPhysicsBase::GetFacingAngleYZ() const { abort(); }
float CTCPhysicsBase::GetFacingAngleXZ() const { abort(); }
void CTCPhysicsBase::SetFacingAngleXY(float) { abort(); }
void CTCPhysicsBase::SetFacingAngleYZ(float) { abort(); }
void CTCPhysicsBase::SetFacingAngleXZ(float) { abort(); }
const CRightHandedSet & CTCPhysicsBase::GetRHSet() const { abort(); }
CRightHandedSet CTCPhysicsBase::GetRHSet(float) const { abort(); }
const CRightHandedSet & CTCPhysicsBase::GetLastRHSet() const { abort(); }
void CTCPhysicsBase::SetFacingAnglesFromRHSet(const CRightHandedSet &) { abort(); }
void CTCPhysicsBase::SetRHSet(const CRightHandedSet &) { abort(); }
C3DVector CTCPhysicsBase::GetImpulse() const { abort(); }
void CTCPhysicsBase::SetRHSetAsFlushOnNormal(const C3DVector &) { abort(); }
void CTCPhysicsBase::SetRHSetAsFlushOnGround() { abort(); }
float CTCPhysicsBase::GetRenderZOffset(float) const { abort(); }
void CTCPhysicsBase::ResetInterpolation() { abort(); }
void CTCPhysicsBase::ApplyControlledImpulse(const C3DVector &) { abort(); }
void CTCPhysicsBase::ApplyControlledAcceleration(const C3DVector &,float) { abort(); }
bool CTCPhysicsBase::IsPushableByHero() const { abort(); }
void CTCPhysicsBase::SetAsPushableByHero(bool) { abort(); }
void CTCPhysicsBase::FrameUpdate() { abort(); }
bool CTCPhysicsBase::HasFrameUpdate() const { abort(); }
void CTCPhysicsBase::SetPhysicsMeshBankIndex(long) { abort(); }
void CTCPhysicsBase::SetScaleFromOwner() { abort(); }
void CTCPhysicsBase::ConstructFromParams(const CTCParamsBase &) { abort(); }
void CTCPhysicsBase::OnCreate() { abort(); }
void CTCPhysicsBase::InitialActivate() { abort(); }
void CTCPhysicsBase::Move() { abort(); }
bool CTCPhysicsBase::DrawGetPhysicsDebugInfo(CThingPhysicalDebugInfo &) const { abort(); }
void CTCPhysicsBase::OnSerialise(CPersistContext &,bool) { abort(); }
bool CTCPhysicsBase::IsComponentSerialisable() const { abort(); }
bool CTCPhysicsBase::IsSerialisableForHeroState() const { abort(); }
bool CTCPhysicsBase::IsCopyable() const { abort(); }
void CTCPhysicsBase::FillPhysicsDebugInfoForSelf(CPhysicsDebugInfo &) const { abort(); }
bool CTCPhysicsBase::HasCollidedWithThingFromTo(const CTCPhysicsBase &,const C3DVector &,const C3DVector &,CPhysicsCollideResponse &) const { abort(); }
bool CTCPhysicsBase::HasCollidedWithThingAt(const CTCPhysicsBase &,const C3DVector &,CPhysicsCollideResponse &) const { abort(); }
bool CTCPhysicsBase::HasCollidedWithLandscapeAt(const C3DVector &,CPhysicsCollideResponse &) const { abort(); }
const CThingPhysicsMeshInfo * CTCPhysicsBase::GetPhysicsMeshInfo() const { abort(); }
void CTCPhysicsBase::UpdateInterpolation() { abort(); }
void CTCPhysicsBase::UpdateOldRHSet() { abort(); }
void CTCPhysicsBase::UpdateOldPos() { abort(); }
bool CResource::UnloadResource() { abort(); }
unsigned long CResource::GetUnloadableMemory() const { abort(); }
bool CResource::IsResourceLoaded() const { abort(); }
void CResource::OnResourceLoaded() { abort(); }
bool CResource::CanAbortResourceLoad(long) const { abort(); }
void CResource::OnResourceLoadAborted() { abort(); }
unsigned long CResource::GetDebugMemoryInUse() const { abort(); }
void CResource::ResetDebugMemoryInUse() { abort(); }
bool CDefClassBase::UnloadResource() { abort(); }
CDefString CDefClassBase::GetInstantiationName() const { abort(); }
void CDefClassBase::SetInstantiationName(CDefString) { abort(); }
void CDefClassBase::AddSubDef(const CCharString&, long, long) { abort(); }
void CDefClassBase::RemoveSubDef(const CCharString&) { abort(); }
CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&) { abort(); }
const CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&) const { abort(); }
bool CDefClassBase::IsParentDef() const { abort(); }
void CDefClassBase::PostTransfer() { abort(); }
void CDefClassBase::InternalSaveBinary(CDataOutputStream&) { abort(); }
void CDefClassBase::InternalLoadBinary(CDataInputStream&) { abort(); }
void CDefClassBase::Validate(const CValidateDefinitionInitBase&, const CCharString&) const { abort(); }
CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> >* CDefClassBase::GetPSubDefInfoMap() { abort(); }
CDefString CParentDefClassBase::GetInstantiationName() const { abort(); }
void CParentDefClassBase::SetInstantiationName(CDefString) { abort(); }
void CParentDefClassBase::AddSubDef(const CCharString&, long, long) { abort(); }
void CParentDefClassBase::RemoveSubDef(const CCharString&) { abort(); }
CDefClassBase::CSubDefInfo* CParentDefClassBase::GetPSubDefInfo(const CCharString&) { abort(); }
const CDefClassBase::CSubDefInfo* CParentDefClassBase::GetPSubDefInfo(const CCharString&) const { abort(); }
bool CParentDefClassBase::IsParentDef() const { abort(); }
void CParentDefClassBase::InternalSaveBinary(CDataOutputStream&) { abort(); }
void CParentDefClassBase::InternalLoadBinary(CDataInputStream&) { abort(); }
CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> >* CParentDefClassBase::GetPSubDefInfoMap() { abort(); }
static unsigned cases=0;
static ECombatZone Oracle(const C3DVector& position,const C3DVector& oldPosition,const C3DVector& attackerPosition,const C3DVector& direction,float radius,long frames) {
 double x=double(position.X)+double(position.X-oldPosition.X)*frames-attackerPosition.X;
 double y=double(position.Y)+double(position.Y-oldPosition.Y)*frames-attackerPosition.Y;
 double cx=double(position.X)-attackerPosition.X,cy=double(position.Y)-attackerPosition.Y;
 float distance=(float)sqrt(x*x+y*y);
 double gap=double(distance)-radius;if(gap<0)gap=0;
 double reach=gap*gap;
 float farLimit=4.0f+0.6f,sideLimit=2.0f+0.6f,rearLimit=3.0f+0.6f;
 // Direction classification uses independent geometric projections.
 double lateral=x*direction.Y-y*direction.X;
 ECombatZone side=lateral<0?COMBAT_ZONE_LEFT:COMBAT_ZONE_RIGHT;
 if(x*cx+y*cy<0 && reach<double(sideLimit*sideLimit))return side;
 float nx=(float)x,ny=(float)y;
 if(distance>DELTA_NOTIONAL_ZERO){float reciprocal=1.0f/distance;nx*=reciprocal;ny*=reciprocal;}
 double projection=double(nx)*direction.X+double(ny)*direction.Y;
 if(projection>cos(double(0.125f)*double(6.2831855f))) {
  if(reach<1.0)return COMBAT_ZONE_FRONT_NEAR;
  if(reach<4.0)return COMBAT_ZONE_FRONT_MIDDLE;
  if(reach<double(farLimit*farLimit))return COMBAT_ZONE_FRONT_FAR;
  return COMBAT_ZONE_NO_ZONE;
 }
 if(projection<cos(double(0.375f)*double(6.2831855f))) {
  if(reach<double(rearLimit*rearLimit))return COMBAT_ZONE_REAR;
  return COMBAT_ZONE_NO_ZONE;
 }
 return reach<double(sideLimit*sideLimit)?side:COMBAT_ZONE_NO_ZONE;
}
int main(){
 CWorld world;CCombatManager manager(world);CThingCreatureBase target,attacker;
 FixturePhysics targetPhysics(target),attackerPhysics(attacker);
 target.PhysicsTC=&targetPhysics;attacker.PhysicsTC=&attackerPhysics;
 combatDef.FrontFarLimit=4.0f;combatDef.FrontMiddleLimit=2.0f;combatDef.FrontNearLimit=1.0f;combatDef.SideLimit=2.0f;combatDef.RearLimit=3.0f;
 typedef std::_Rb_tree_node<std::pair<const unsigned __int64,CCombatantInfo> > Node;
 Node node;node._M_left=node._M_right=0;node._M_parent=manager.Combatants._M_t._M_header._M_data;
 node._M_value_field.second.AttackerInfo.Attacked=&target;
 const float xs[]={-3.0f,-1.5f,-0.25f,0.0f,0.25f,1.5f,3.0f,6.0f};
 const float ys[]={0.0f,0.5f,2.0f,-0.5f,-2.0f};
 const float vx[]={0.0f,0.5f,-1.0f},vy[]={0.0f,-0.25f,0.5f};
 const long frames[]={0,1,4,-1};const float radii[]={0.0f,0.5f,2.0f};
 const C3DVector facings[]={C3DVector(1,0,0),C3DVector(0,1,0),C3DVector(-1,0,0),C3DVector(0,-1,0)};
 unsigned zones[8]={0,0,0,0,0,0,0,0};
 for(unsigned xi=0;xi<8;++xi)for(unsigned yi=0;yi<5;++yi)for(unsigned velocity=0;velocity<3;++velocity)for(unsigned frame=0;frame<4;++frame)for(unsigned radius=0;radius<3;++radius)for(unsigned direction=0;direction<4;++direction)for(unsigned gate=0;gate<5;++gate){
  manager.Combatants._M_t._M_header._M_data->_M_parent=gate==0?0:&node;
  attacker.UniqueID=gate==1?1:0;
  node._M_value_field.second.AttackerInfo.Attacked=gate==2?0:&target;
  outerRing=gate==4;facing=facings[direction];
  attackerPhysics.Position=C3DVector(4.0f,-2.0f,9.0f);
  targetPhysics.Position=C3DVector(xs[xi]+4.0f,ys[yi]-2.0f,-3.0f);
  targetPhysics.OldPosition=C3DVector(targetPhysics.Position.X-vx[velocity],targetPhysics.Position.Y-vy[velocity],-8.0f);
  targetPhysics.Radius=radii[radius];defCalls=outerCalls=peekCalls=facingCalls=normaliseCalls=0;
  ECombatZone expected=gate==4?COMBAT_ZONE_OUTER_RING:Oracle(targetPhysics.Position,targetPhysics.OldPosition,attackerPhysics.Position,facing,radii[radius],frames[frame]);
  ECombatZone got=manager.GetDefenderZone(target,attacker,frames[frame]);
  if(got!=expected){fprintf(stderr,"case %u x%u y%u v%u f%u r%u d%u g%u got%d expected%d\n",cases,xi,yi,velocity,frame,radius,direction,gate,(int)got,(int)expected);return 1;}
  assert(outerCalls==(gate>=3?1u:0u));assert(defCalls==(gate==4?0u:5u));assert(peekCalls==(gate==4?0u:2u));assert(facingCalls==(gate==4?0u:1u));
  assert(normaliseCalls==(gate==4||(!xs[xi]&&!ys[yi])?0u:1u));
  unsigned bit=0;while((1u<<bit)!=(unsigned)got)++bit;assert(bit<8);++zones[bit];++cases;
 }
 manager.Combatants._M_t._M_header._M_data->_M_parent=0;
 for(unsigned z=0;z<8;++z)assert(zones[z]);
 assert(cases==28800);
 puts("006e99b0_TEST PASS");return 0;
}