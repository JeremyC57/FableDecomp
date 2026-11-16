// Self-contained fixture: actual shared classes, production body and recording callee doubles.
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
enum EPlayerMode {};enum ESaveGameLoadStatus {};enum ERegionLoadStatus {};enum EPlayerSpawnStatus {};enum ESaveType {};
namespace NCameraModes { enum EMode {}; enum ESubComponentType {}; }
#endif
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_RETAIL_STLP_HOST_TAGS_PASS23
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCHITLOCATIONS_ORIGINAL_CTOR_PASS23
#define FABLE_RETAIL_DEF_CLASS_BASE_CTOR_DECL
#define FABLE_RETAIL_CTCPHYSICSBASE_ORIGINAL_CTOR_PASS23
#define private public
#define protected public
// NCameraModes::CMoveOnCylinder::Update() -- orbit the observed creature using profile camera controls and keep the camera above gheight.
// Leaf: ?Update@CMoveOnCylinder@NCameraModes@@UAEXXZ
// Retail: 0x0085C470, 857 bytes, thiscall.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLDMAP_CYLINDER_QUERY
#define FABLE_RETAIL_CWORLDMAP_HEIGHTZ
#define FABLE_RETAIL_NCAMERAMODES_CBASE_CYLINDER
#define FABLE_RETAIL_NCAMERAMODES_CBASE_INLINE_ACCESSORS
#define FABLE_RETAIL_NCAMERAMODES_CBASE_CYLINDER_INPUT
#define FABLE_RETAIL_CUSERPROFILEMANAGER_CYLINDER_INPUT
#define FABLE_RETAIL_CCAMERAMODEDEF_CYLINDER
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CCameraModeDef.h"
#include "engine/CThingGameObject.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCHitLocations.h"
#include "engine/CUserProfileManager.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/NCameraModes_CMoveOnCylinder.h"

namespace NCameraModes {
void CMoveOnCylinder::Update()
{
    const CThingGameObject* thing = GetObservedThing();
    if (!thing || (thing->IsDead()))
        return;
    {
        C2DVector cam_stick = GetLeftStickVector();
        if (CUserProfileManager::Get().IsCameraPCViewHeroXYInverted())
            cam_stick.X *= -1.0f;
        if (CUserProfileManager::Get().IsCameraPCViewHeroYZInverted())
            cam_stick.Y *= -1.0f;
        float XY_ROTATION_SPEED = GetCameraModeDef()->ViewHeroXYRotationMaxVelocity;
        float VERTICAL_ROTATION_SPEED = GetCameraModeDef()->ViewHeroHeightParamMaxVelocity;
        float cam_sensitivity = CUserProfileManager::Get().GetCameraSensitivity();
        if (cam_sensitivity < 0.0f)
            cam_sensitivity = 0.0f;
        else if (cam_sensitivity > 1.0f)
            cam_sensitivity = 1.0f;
        cam_sensitivity += 0.5f;
        Theta += XY_ROTATION_SPEED * cam_sensitivity * cam_stick.X;
        HeightParam += cam_sensitivity * VERTICAL_ROTATION_SPEED * cam_stick.Y;
        if (HeightParam < 0.0)
            HeightParam = 0.0f;
        else if (HeightParam > 1.0f)
            HeightParam = 1.0f;
    }

    float rad = GetObservedThing()->GetPhysicsTC()->GetRadius();
    float height_limit = GetCameraModeDef()->ViewHeroXYOffsetMultiplier;
    const CThingGameObject* observed = GetObservedThing();
    const CTCHitLocations* ptc_hit;
    observed->PeekTC(&ptc_hit);
    C3DVector centre_pos;
    float height;
    ptc_hit->GetBoundingSphere(centre_pos, height, false);
    height *= 2.0f;
    C3DVector xy_out = AxisX;
    xy_out.Rotate(AxisZ, Theta);
    C3DVector base_pos = GetObservedThing()->GetPos();
    C3DVector adjust_x = xy_out * rad * height_limit;
    C3DVector ground_test_pos = base_pos + adjust_x + AxisZ;
    C3DVector view_pos = base_pos + AxisZ * height * HeightParam;
    C3DVector new_pos = view_pos + adjust_x;
    float gheight = GetWorld().PeekWorldMap().GetHeightZBelowPos(ground_test_pos);
    gheight += GetCameraModeDef()->ViewHeroGroundHeightOffset;
    if (new_pos.Z < gheight)
        new_pos.Z = gheight;
    AddAttractorGoTo(new_pos, 1.0f);
    LookAt.Position = view_pos;
}
}

#include "engine/CCamera.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CGameCameraManager.h"
#include "engine/CDefString.h"
#undef private
#undef protected

// Other virtual methods abort if unexpectedly called.
void CBaseObject::Validate() const { abort(); }
void CClass::Save(CCharString&) { abort(); }
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
C3DVector CTCPhysicsBase::GetFacingVector() const { abort(); }
C3DVector CTCPhysicsBase::GetFacingVector(float) const { abort(); }
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
void CThingGameObject::OnHit(CHitParameters&) { abort(); }
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
void CWorld::PostLoadInit() { abort(); }
void CWorld::LoadWorldFromFile(CAFile& file, long version) { abort(); }
CThingManager& CWorld::GetThingManager() { abort(); }
const CThingManager& CWorld::PeekThingManager() const { abort(); }
CCamera CWorld::GetCurrentRenderCamera() const { abort(); }
CCamera CWorld::GetCurrentRenderCamera(const CInterpolationInfo& info) const { abort(); }
void CWorld::PostInit(CDisplayEngine& displayEngine) { abort(); }
C3DVector CWorld::GetGameLODCentre() const { abort(); }
void CWorld::Init() { abort(); }
void CWorld::Uninitialise() { abort(); }
void CWorld::PreparePrimitivesForEngine(const CEnginePrimitivePrepareInfo& info) { abort(); }
CWorldMap& CWorld::GetWorldMap() { abort(); }
const CProceduralTexture* CWorld::DrawGetProceduralTexture() const { abort(); }
const CIDrawWorldMap* CWorld::DrawGetWorldMap() const { abort(); }
const CIDrawEnvironment* CWorld::DrawGetEnvironment() const { abort(); }
void NCameraModes::CBase::OnCameraInsideObservedThing() { abort(); }
bool NCameraModes::CBase::IsUseRightStickLookAround() const { abort(); }
void NCameraModes::CBase::PreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo & p0) const { abort(); }
bool NCameraModes::CBase::IsShortenOnOcclusion() const { abort(); }
bool NCameraModes::CBase::IsRaiseUpOnHeroNearby() const { abort(); }
bool NCameraModes::CBase::IsStretchAroundHero() const { abort(); }
bool NCameraModes::CBase::IsAvoidLandscape() const { abort(); }
bool NCameraModes::CBase::IsFOVChange() const { abort(); }
bool NCameraModes::CBase::IsTrackMeleeOpponents() const { abort(); }
bool NCameraModes::CBase::IsTrackMeleeOpponentsAutoFOVChange() const { abort(); }
bool NCameraModes::CBase::IsFinished() const { abort(); }
bool NCameraModes::CBase::IsStringAndCage() const { abort(); }
float NCameraModes::CBase::GetObservedThingCurrentMeshHeight() const { abort(); }
void NCameraModes::CBase::Recentre() { abort(); }
void NCameraModes::CBase::UpdateSubComponents() { abort(); }
float NCameraModes::CBase::GetDefaultFOV() const { abort(); }
float NCameraModes::CBase::GetDefaultRollAngle() const { abort(); }
float NCameraModes::CBase::GetDefaultObservedPositionZOffset() const { abort(); }
void NCameraModes::CBase::OnReset() { abort(); }
NCameraModes::EMode NCameraModes::CMoveOnCylinder::GetModeType() const { abort(); }
void NCameraModes::CMoveOnCylinder::OnInit() { abort(); }
void NCameraModes::CMoveOnCylinder::OnDie() { abort(); }
bool NCameraModes::CMoveOnCylinder::IsRaiseUpOnHeroNearby() const { abort(); }
bool NCameraModes::CMoveOnCylinder::IsStretchAroundHero() const { abort(); }

const float DELTA_NOTIONAL_ZERO=0.0001f;
static const CThingGameObject* observedThing=0;
static CWorld* testWorld=0;
static CWorldMap testMap;
static C3DVector observedPos(10.0f,20.0f,2.0f),recordedAttractor,recordedProbe;
static float groundHeight=0.0f;
static unsigned int attractorCalls=0,rotationCalls=0;
static CUserProfileManager profile;
CUserProfileManager& __fastcall CUserProfileManager::Get(){return profile;}
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
CTCBase::CTCBase(CThing& thing):ParentThing(&thing){}
CTCBase::~CTCBase(){}
CTCPhysicsBase::CTCPhysicsBase(CThing& thing):CTCBase(thing){}
CTCPhysicsBase::~CTCPhysicsBase(){}
CTCHitLocations::CTCHitLocations(CThing& thing):CTCBase(thing){}
class TestPhysics:public CTCPhysicsBase {public:TestPhysics(CThing& t):CTCPhysicsBase(t){} CCharString GetClassTypeName()const{return CCharString();}};
class TestHit:public CTCHitLocations {public:TestHit(CThing& t):CTCHitLocations(t){} ETCInterfaceType GetInterfaceType()const{return TCI_HIT_LOCATIONS;} CCharString GetClassTypeName()const{return CCharString();}};
CIDrawWorld::~CIDrawWorld(){}
CWideString::CWideString(){}
CWideString::~CWideString(){}
CWorld::~CWorld(){}
const CWorldMap& CWorld::PeekWorldMap()const{return testMap;}
CResource::CResource():ResourceList(0),PrevResource(0),NextResource(0),ResourceSize(0),LastUsedFrame(0){IVRefCount=1;}
CResource::~CResource(){}
CDefClassBase::CDefClassBase():PDefManager(0),GlobalIndex(0),Setup(false),Template(false),DefaultValsApplied(false){}
CParentDefClassBase::CParentDefClassBase(){}
CParentDefClassBase::~CParentDefClassBase(){}
void CIVCountedPointeeBase::Delete()const{abort();}
class TestCameraDef:public CCameraModeDef {
public:void Transfer(CPersistContext&){} void Copy(const CDefClassBase*){} unsigned long GetSizeofClass(){return sizeof(*this);}
};
namespace NCameraModes {
CBase::CBase(CGameCameraManager& mgr,const CDefPointer<const CCameraModeDef>& def):CameraManager(mgr),PCameraModeDef(def){}
CBase::~CBase(){}
CMoveOnCylinder::CMoveOnCylinder(CGameCameraManager& mgr,const CDefPointer<const CCameraModeDef>& def):CBase(mgr,def){}
CMoveOnCylinder::~CMoveOnCylinder(){}
const CThingGameObject* CBase::GetObservedThing()const{return observedThing;}
const CWorld& CBase::GetWorld()const{assert(testWorld);return *testWorld;}
void CBase::AddAttractorGoTo(const C3DVector& pos,float rate){assert(rate==1.0f);recordedAttractor=pos;++attractorCalls;}
}
const C3DVector& CThing::GetPos()const{return observedPos;}
void CTCHitLocations::GetBoundingSphere(C3DVector& centre,float& radius,bool exact)const{assert(!exact);centre=C3DVector(7,8,9);radius=2.0f;}
float CWorldMap::GetHeightZBelowPos(const C3DVector& pos)const{recordedProbe=pos;return groundHeight;}
void C3DVector::Rotate(const C3DVector& axis,float angle){
 assert(axis.X==0.0f&&axis.Y==0.0f&&axis.Z==1.0f);++rotationCalls;
 double a=angle*6.2831853071795864769,x=X,y=Y;X=(float)(x*cos(a)-y*sin(a));Y=(float)(x*sin(a)+y*cos(a));
}
static bool near(float a,double b){return fabs(a-b)<0.0001*(1.0+fabs(b));}
int main(){
 CWorld world;testWorld=&world;
 CThingGameObject thing;TestPhysics physics(thing);TestHit hit(thing);
 thing.PhysicsTC=&physics;thing.Dead=false;physics.Radius=2.0f;
 // Shared original CVectorMap owns its real pair storage. This one-entry allocation remains alive for all cases.
 typedef std::pair<ETCInterfaceType,CTCBase*> Entry;
 thing.Components.push_back(Entry(TCI_HIT_LOCATIONS,&hit));
 memset(&thing.UsingComponents,0,sizeof(thing.UsingComponents));thing.UsingComponents._M_w[127/(8*sizeof(unsigned long))]|=1UL<<(127%(8*sizeof(unsigned long)));
 TestCameraDef def;def.IVRefCount=1;def.ViewHeroXYOffsetMultiplier=3.0f;def.ViewHeroXYRotationMaxVelocity=0.25f;
 def.ViewHeroHeightParamMaxVelocity=0.5f;def.ViewHeroGroundHeightOffset=1.0f;
 CDefPointer<const CCameraModeDef> handle;handle.Object=&def;def.AddRef();
 CGameCameraManager manager;NCameraModes::CMoveOnCylinder mode(manager,handle);
 mode.AxisX=C3DVector(1,0,0);mode.AxisZ=C3DVector(0,0,1);
 observedThing=0;mode.Update();assert(attractorCalls==0);
 observedThing=&thing;thing.Dead=true;mode.Update();assert(attractorCalls==0);thing.Dead=false;
 static const float sensitivities[4]={-2,0.25f,1,3};
 for(int use=0;use<2;++use)for(int inv=0;inv<4;++inv)for(int sensitivity=0;sensitivity<4;++sensitivity)for(int clamp=0;clamp<3;++clamp){
  profile.UseNewProfileValues=use!=0;
  profile.CameraPCViewHeroXYInverted=(inv&1)!=0;profile.CameraPCViewHeroYZInverted=(inv&2)!=0;
  profile.CameraPCViewHeroXYInvertedNewProfile=(inv&1)==0;profile.CameraPCViewHeroYZInvertedNewProfile=(inv&2)==0;
  profile.CameraSensitivity=sensitivities[sensitivity];profile.CameraSensitivityNewProfile=sensitivities[3-sensitivity];
  mode.LeftStickVector=C2DVector(0.5f,-0.25f);mode.Theta=0.125f;mode.HeightParam=clamp==0?-0.5f:clamp==1?0.5f:1.5f;
  float initialHeight=mode.HeightParam;
  groundHeight=clamp==0?20.0f:-10.0f;
  bool ix=use?profile.CameraPCViewHeroXYInvertedNewProfile:profile.CameraPCViewHeroXYInverted;
  bool iy=use?profile.CameraPCViewHeroYZInvertedNewProfile:profile.CameraPCViewHeroYZInverted;
  double sen=use?profile.CameraSensitivityNewProfile:profile.CameraSensitivity;if(sen<0)sen=0;if(sen>1)sen=1;sen+=0.5;
  double theta=0.125+0.25*sen*(ix?-0.5:0.5),height=initialHeight+sen*0.5*(iy?0.25:-0.25);if(height<0)height=0;if(height>1)height=1;
  unsigned int calls=attractorCalls;mode.Update();assert(attractorCalls==calls+1);
  assert(near(mode.Theta,theta)&&near(mode.HeightParam,height));
  assert(near(mode.LookAt.Position.X,10)&&near(mode.LookAt.Position.Y,20)&&near(mode.LookAt.Position.Z,2+4*height));
  assert(near(recordedAttractor.X,10+6*cos(theta*6.2831853071795864769)));
  assert(near(recordedAttractor.Y,20+6*sin(theta*6.2831853071795864769)));
  double z=2+4*height;if(z<groundHeight+1)z=groundHeight+1;assert(near(recordedAttractor.Z,z));
  assert(near(recordedProbe.X,recordedAttractor.X)&&near(recordedProbe.Y,recordedAttractor.Y)&&near(recordedProbe.Z,3));
  assert(mode.LeftStickVector.X==0.5f&&mode.LeftStickVector.Y==-0.25f);
 }
 assert(rotationCalls==96&&attractorCalls==96);
 puts("0085c470_TEST PASS");return 0;
}