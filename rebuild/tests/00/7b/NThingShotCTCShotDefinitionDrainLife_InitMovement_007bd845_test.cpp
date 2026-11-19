// Behaviour test for 0x007BD845 NThingShot::CTCShotDefinitionDrainLife::InitMovement: the row's defines, includes and body verbatim inside
// the same-TU twin 0x007BCF7F's landed self-contained test harness (real shared class models, controlled external operations).
// Checked over a grid of distances / speeds / random fractions / def lifetimes: the call order (shoot-at pos, SetInteractionFlags(0),
// Randomise, three normalises, RH set, sub-def lookup, two random wobbles with their rotations, SetVelocity, gravity off, the speed
// random), the RH set (up, line), the clamped distance -> max_angle, the wobble ranges and angles, the launch velocity
// (InitialSpeed / FPS along the line), the turn cone (MaxAngleToTurnDegs / 360 and its cosine), Speed = shot Speed / FPS plus
// GFFloatRandom(SpeedRange / FPS), and the sub-def reference released (deleted when it was the last).
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __stdcall
#define __int64 long long
#define __declspec(x)
#define FABLE_PORTABLE
#include <strings.h>
#define _stricmp strcasecmp
enum ESex{};enum EAIStateGroupType{};enum EScriptAIPriority{};enum EReactionSpeechType{FixtureReactionZero};enum ESaveGameLoadStatus{};enum ERegionLoadStatus{};enum EPlayerSpawnStatus{};enum ESaveType{};enum EEnginePrimitiveStatIndex{};
#endif
#include "engine/EPlayerMode.h"
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_RETAIL_STLP_HOST_TAGS_PASS23
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_STLP_TREE_CONSTFIND
#define FABLE_RETAIL_DEF_CLASS_BASE_CTOR_DECL
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_CTOR
#define FABLE_RETAIL_CDEFPOINTER_POINTER_CTOR_ADOPT
#define FABLE_RETAIL_CDEFPOINTER_RESET
#define FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL
#define FABLE_RETAIL_CTCBOASTINGCROWDCONTROL
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CGAME_CORE_PASS23
#define FABLE_RETAIL_CINITBASECLASS
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define private public
#define protected public

#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#include "engine/EMessageEventType.h"
#define FABLE_RETAIL_MULTISHOT_LIFETIMES_PASS23
#include "engine/fable_ai_action_enums.h"
// NThingShot::CTCShotDefinitionDrainLife::InitMovement() -- aim the drain-life bolt (randomised), face it, wobble it about up/right, launch it, set its turn cone and randomised speed
// Leaf: ?InitMovement@CTCShotDefinitionDrainLife@NThingShot@@MAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x007BD845, 642 bytes, thiscall `ret` (vtable slot 33; Ego_r + FableWin ?InitMovement@CTCShotDefinitionDrainLife@NThingShot@@MAEXXZ).
// Callees: CTCShotDefinitionBase::GetShootAtPos 0x007BEDD0, CTCPhysicsBase::SetInteractionFlags (virtual +0xAC), __ftol2 0x00BFEA70
//   (GetShotAccuracyPercentage, inline), CTCBase::GetWorld 0x00686A70, CWorld::GetWorldSeed 0x0049D810, NThingShot::RandomiseShotDirection
//   0x007BEBE0, C3DVector::AccurateNormalise 0x00A14440 (three times), CRightHandedSet::CRightHandedSet 0x00487050, CTCPhysicsBase::ResetRHSet
//   (virtual +0x80), CDefClassBase::GetSubDef<CThingDrainLifeShotDef> 0x007BE92E, GFFloatRandom 0x004978A0 (three times), C3DVector::Rotate
//   0x00A13C60 (twice), CTCPhysicsBase::SetVelocity 0x0066F9B0, CTCPhysicsBase::EnableGravity (virtual +0x98); ConstantFPS 0x01375550.
// Shape: the landed same-TU twin CTCShotDefinitionDrainLife::InitMovement (0x007BCF7F) text and include / pragma set, in FableWin.pdb's /Od
//   statement order for this function (FableWin.exe 0x0270CF31: fire_at_pos, SetInteractionFlags(0), line_to_target, distance, Randomise,
//   up = CrossWithUp().Cross(line), right, the RH set, pt_shot_definition, CDefPointer + GetSubDef, speed, clamp, max_angle, two Rotate,
//   initial_velocity, ..., Speed = GetShotDef().Speed / FPS, speed_range, Speed += GFFloatRandom). The donor's two ASSERTs (line_to_target
//   IsNonZero, the GetSubDef result) emit nothing in retail.
//   - FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE (as the same-TU siblings 0x007BBE34 / 0x007BBB3B): the CRightHandedSet temporary keeps its own
//     frame slot (ebp-5Ch, frame 5Ch) while retail still calls 0x00487050; with the declaration-only ctor it overlays the C3DVector call
//     temporary (ebp-50h, frame 50h).
// Bracket: FABLE_RETAIL_C3DVECTOR_SUBTRACT_XY_GROUP (operator- with the X and Y differences grouped, existing shared opt-in): puts
//   `lea ecx,[ebp-20h]` (up's AccurateNormalise) between the two products of up.Z (0x007BD90F). This function has no branch before
//   0x007BD9A1, so line_to_target's subtraction sits in the same block. Measured: ungrouped two x87 ops late, X-only grouped one late;
//   the member-Cross groupings are shadowed by CTCSHOTDEFINITIONBASE_COLLISION's Cross text (dropping it costs 6-8 lines); statement
//   brackets on the up / right lines change nothing.
#ifdef _MSC_VER
#endif
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SUBTRACT_XY_GROUP
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#define FABLE_RETAIL_C3DVECTOR_ACCURATE_MAGNITUDE
#define FABLE_RETAIL_C3DVECTOR_ANGLE_BODIES
#define FABLE_RETAIL_C3DVECTOR_SCALE_X_PAREN
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_SETVELOCITY_OUTOFLINE
#define FABLE_RETAIL_CTCPHYSICSBASE_SETASMOVING
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_ONHITBYMELEE
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_MOVEMENT
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_COLLISION
#define FABLE_RETAIL_CTCSHOTDEFINITIONBASE_SHOTACCESSORS
#define FABLE_RETAIL_CTHINGDRAINLIFESHOTDEF_CLASS
#include <math.h>
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CRightHandedSet.h"
#ifdef _MSC_VER
#endif
#include "engine/CTCPhysicsBase.h"
#ifdef _MSC_VER
#endif
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/GFFloatRandom.h"
#include "engine/CDefClassBase_GetSubDef.h"
#include "engine/CThingDrainLifeShotDef.h"
#include "engine/NThingShot_CTCShotDefinitionDrainLife.h"
#ifdef _MSC_VER
#pragma intrinsic(sqrt,cos)
#endif
#include "engine/GFCos.h"
namespace NThingShot {
C3DVector FABLE_FASTCALL RandomiseShotDirection(const C3DVector& direction,long accuracy,unsigned long* seed);
void CTCShotDefinitionDrainLife::InitMovement()
{
    CTCPhysicsBase* ptc_phys=ParentThing->GetPhysicsTC();
    C3DVector fire_at_pos=GetShootAtPos();
    ptc_phys->SetInteractionFlags(0);
    C3DVector line_to_target=fire_at_pos-ptc_phys->GetPosition();
    float distance=line_to_target.GetAccurateMagnitude();
    line_to_target=RandomiseShotDirection(line_to_target,GetShotAccuracyPercentage(),GetWorld().GetWorldSeed());
    line_to_target.AccurateNormalise();
    C3DVector up=line_to_target.CrossWithUp().Cross(line_to_target);
    C3DVector right=line_to_target.Cross(up);
    up.AccurateNormalise();
    right.AccurateNormalise();
    ptc_phys->ResetRHSet(CRightHandedSet(up,line_to_target));
    const CThingShotDef* pt_shot_definition=&GetShotDef();
    CDefPointer<const CThingDrainLifeShotDef> pt_drain_shot_definition;
    pt_shot_definition->GetSubDef(&pt_drain_shot_definition);
    float speed=pt_drain_shot_definition->InitialSpeed/CMainGameComponent::GetConstantFPS();
    if(distance>pt_drain_shot_definition->MaxDistance)distance=pt_drain_shot_definition->MaxDistance;
    float max_angle=pt_drain_shot_definition->MaxAngle*distance/pt_drain_shot_definition->MaxDistance;
    float angle=-max_angle+GFFloatRandom(max_angle*2.0f,GetWorld().GetWorldSeed());
    line_to_target.Rotate(up,angle/360.0f);
    angle=-max_angle+GFFloatRandom(max_angle,GetWorld().GetWorldSeed());
    line_to_target.Rotate(right,angle/360.0f);
    C3DVector initial_velocity=line_to_target*speed;
    ptc_phys->SetVelocity(initial_velocity);
    ptc_phys->EnableGravity(false);
    MaxAngleToTurn=pt_drain_shot_definition->MaxAngleToTurnDegs/360.0f;
    MaxCosAngleToTurn=(float)GFCos(MaxAngleToTurn);
    Speed=GetShotDef().Speed/CMainGameComponent::GetConstantFPS();
    float speed_range=pt_drain_shot_definition->SpeedRange/CMainGameComponent::GetConstantFPS();
    Speed+=GFFloatRandom(speed_range,GetWorld().GetWorldSeed());
}
}

#include "engine/CDefString.h"
#include "engine/CInitBaseClass.h"
#include "engine/CCamera.h"
#include "engine/C2DVector.h"

static int interactionCalls,stringsLive,worldCalls,randomiseCalls,normaliseCalls,rotateCalls,randomCalls,shootCalls,resetCalls,movingCalls,gravityCalls,lookupCalls,defCalls,deleteCalls;
static unsigned long worldSeed=91;static bool scatter;static float fraction;static C3DVector target,normalInput[3],rotationAxes[2];static float ranges[3],angles[2];static char events[128];static unsigned eventCount;
static void Event(char c){assert(eventCount<127);events[eventCount++]=c;events[eventCount]=0;}
static CThing* parentThing;static CTCPhysicsBase* physics;static NThingShot::CTCShotDefinitionDrainLife* shot;
static CThingShotDef* baseDef;static CThingDrainLifeShotDef* drainDef;static CDefinitionManager* manager;
static CDefClassBase::CSubDefInfo SubInfo;
CBaseClass::CBaseClass(){} CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){}CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CInitBaseClass::CInitBaseClass():Valid(false){}CInitBaseClass::~CInitBaseClass(){}
CBaseObject::CBaseObject():IntelligentPointer(0){}CBaseObject::~CBaseObject(){}
CBaseIntelligentPointer::CBaseIntelligentPointer():PData(0){}
CBaseIntelligentPointer::~CBaseIntelligentPointer(){assert(!PData);}
CCharString::CCharString():PStringData(0){}
CCharString::CCharString(const char*s,long n){assert(n==-1);PStringData=new CCharStringData;PStringData->Data.PData=const_cast<char*>(s);PStringData->Data.StringLength=(unsigned long)strlen(s);PStringData->NoRefs=1;++stringsLive;}
CCharString::CCharString(const CCharString&s):PStringData(s.PStringData){if(PStringData){++PStringData->NoRefs;++stringsLive;}}
CCharString::~CCharString(){if(PStringData){assert(stringsLive>0);--stringsLive;if(--PStringData->NoRefs==0)delete PStringData;}}
const char CCharString::NullChar=0;
CWideString::CWideString():PStringData(0){}CWideString::~CWideString(){assert(!PStringData);}
CTCBase::CTCBase(CThing&t):ParentThing(&t){}CTCBase::~CTCBase(){}
CTCPhysicsBase::CTCPhysicsBase(CThing&t):CTCBase(t){}CTCPhysicsBase::~CTCPhysicsBase(){}
NThingShot::CTCShotDefinitionBase::CTCShotDefinitionBase(CThing&t):CTCBase(t){}
NThingShot::CTCShotDefinitionBase::~CTCShotDefinitionBase(){}
CResource::CResource():ResourceList(0),PrevResource(0),NextResource(0),ResourceSize(0),LastUsedFrame(0){IVRefCount=1;}CResource::~CResource(){}
CDefClassBase::CDefClassBase():PDefManager(0),GlobalIndex(0),Setup(false),Template(false),DefaultValsApplied(false){}
CParentDefClassBase::CParentDefClassBase(){}CParentDefClassBase::~CParentDefClassBase(){}
CSubDefClassBase::CSubDefClassBase(){}
CDefinitionManager::~CDefinitionManager(){}CFailedAllocationHandler::~CFailedAllocationHandler(){}CResourceList::~CResourceList(){}
void CIVCountedPointeeBase::Delete()const{++deleteCalls;assert(this==drainDef);}
namespace std {template<> _Vector_base<CEnginePrimitiveHandle,allocator<CEnginePrimitiveHandle> >::~_Vector_base(){assert(!_M_start);}template<> vector<CEnginePrimitiveHandle>::~vector(){assert(!_M_start&&!_M_finish);}}
CIDrawWorld::~CIDrawWorld(){}CWorld::~CWorld(){}
static CWorld world;
CWorld&CTCBase::GetWorld()const{assert(this==shot);++worldCalls;return world;}
unsigned long*CWorld::GetWorldSeed()const{assert(this==&world);return&worldSeed;}
long CMainGameComponent::ConstantFPS=4;
C3DVector NThingShot::CTCShotDefinitionBase::GetShootAtPos()const{assert(this==shot);++shootCalls;Event('T');return target;}
C3DVector FABLE_FASTCALL NThingShot::RandomiseShotDirection(const C3DVector&v,long accuracy,unsigned long*seed){assert(accuracy==37&&seed==&worldSeed&&!randomiseCalls);++randomiseCalls;Event('R');return v;}
C3DVector&C3DVector::AccurateNormalise(){assert(normaliseCalls<3);normalInput[normaliseCalls++]=*this;Event('N');float n=(float)sqrt(X*X+Y*Y+Z*Z);assert(n>0);X/=n;Y/=n;Z/=n;return*this;}
// (CRightHandedSet(up, forward) is the header's inline body, FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE: Up +0, Forward +0xC, as retail 0x00487050.)
void CTCPhysicsBase::ResetRHSet(const CRightHandedSet&rh){assert(this==physics&&normaliseCalls==3&&!resetCalls);++resetCalls;Event('H');assert(rh.Up.X==0&&rh.Up.Y==0&&rh.Up.Z==1);assert(rh.Forward.X==1&&rh.Forward.Y==0&&rh.Forward.Z==0);}
float FABLE_FASTCALL GFFloatRandom(float range,unsigned long*seed){assert(seed==&worldSeed&&randomCalls<3);ranges[randomCalls++]=range;Event('F');return range*fraction;}
void C3DVector::Rotate(const C3DVector&axis,float angle){assert(rotateCalls<2&&X==1&&Y==0&&Z==0);rotationAxes[rotateCalls]=axis;angles[rotateCalls++]=angle;Event('O');/* Controlled external operation: orientation retained so exact rational acceleration can be checked. */}
void CTCPhysicsBase::SetAsMoving(bool moving){assert(this==physics&&moving&&!movingCalls&&resetCalls==1);++movingCalls;Event('V');}
void CTCPhysicsBase::EnableGravity(bool on){assert(this==physics&&!on&&movingCalls==1&&!gravityCalls);++gravityCalls;Event('G');}
const CDefClassBase::CSubDefInfo*CParentDefClassBase::GetPSubDefInfo(const CCharString&name)const{assert(this==baseDef&&strcmp(name.GetAsCharArray(),"CThingDrainLifeShotDef")==0);++lookupCalls;Event('D');return&SubInfo;}
CDefPointer<CDefClassBase>CDefinitionManager::GetPDefFromGlobalIndex(long index){assert(this==manager&&index==7);++defCalls;CDefPointer<CDefClassBase> result;result.Object=drainDef;drainDef->AddRef();return result;}
class TestPhysics:public CTCPhysicsBase{public:TestPhysics(CThing&t):CTCPhysicsBase(t){}ETCInterfaceType GetInterfaceType() const{abort();}CCharString GetClassTypeName() const{abort();}};
// CTCShotDefinitionDrainLife declares no constructor (only its implicit one, which cannot reach the base's CThing& constructor), so the
// test builds a base CTCShotDefinitionBase subobject with room for the derived members (+0x9C..+0xB3) and runs the row on that image
// through a qualified (non-virtual) call.
class TestShot:public NThingShot::CTCShotDefinitionBase{public:TestShot(CThing&t):CTCShotDefinitionBase(t){memset(Tail,0,sizeof(Tail));}ETCInterfaceType GetInterfaceType() const{abort();}CCharString GetClassTypeName() const{abort();}void InitMovement(){abort();}char Tail[sizeof(NThingShot::CTCShotDefinitionDrainLife)-sizeof(NThingShot::CTCShotDefinitionBase)];};
class TestShotDef:public CThingShotDef{public:void Transfer(CPersistContext&) {abort();}void Copy(const CDefClassBase*) {abort();}unsigned long GetSizeofClass() {abort();}};
class TestDrainDef:public CThingDrainLifeShotDef{public:void Transfer(CPersistContext&) {abort();}void Copy(const CDefClassBase*) {abort();}unsigned long GetSizeofClass() {abort();}};

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
bool CResource::UnloadResource() { abort(); }
unsigned long CResource::GetUnloadableMemory() const { abort(); }
bool CResource::IsResourceLoaded() const { abort(); }
void CResource::OnResourceLoaded() { abort(); }
bool CResource::CanAbortResourceLoad(long) const { abort(); }
void CResource::OnResourceLoadAborted() { abort(); }
unsigned long CResource::GetDebugMemoryInUse() const { abort(); }
void CResource::ResetDebugMemoryInUse() { abort(); }
bool CResourceList::HandleFailedAllocation(unsigned long) { abort(); }
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
const CWorldMap& CWorld::PeekWorldMap() const { abort(); }
const CProceduralTexture* CWorld::DrawGetProceduralTexture() const { abort(); }
const CIDrawWorldMap* CWorld::DrawGetWorldMap() const { abort(); }
const CIDrawEnvironment* CWorld::DrawGetEnvironment() const { abort(); }
ETCInterfaceType CTCPhysicsBase::GetInterfaceType() const { abort(); }
C2DVector CTCPhysicsBase::GetOffsetVector() const { abort(); }
C3DVector CTCPhysicsBase::GetOffsetVector3D() const { abort(); }
void CTCPhysicsBase::SetPosition(const C3DVector &) { abort(); }
void CTCPhysicsBase::ResetPosition(const C3DVector &) { abort(); }
void CTCPhysicsBase::SetPositionToNavigateTo(const C3DVector &) { abort(); }
void CTCPhysicsBase::SetForcedToMovePos(const C3DVector &,bool) { abort(); }
void CTCPhysicsBase::ResetMovementState() { abort(); }
void CTCPhysicsBase::EnablePhysics(bool) { abort(); }
bool CTCPhysicsBase::IsPhysicsEnabled() const { abort(); }
bool CTCPhysicsBase::IsGravityEnabled() const { abort(); }
void CTCPhysicsBase::EnableNavigator(bool) { abort(); }
bool CTCPhysicsBase::IsNavigationEnabled() const { abort(); }
bool CTCPhysicsBase::HasNavigationLayers() const { abort(); }
void CTCPhysicsBase::SetInteractionFlags(long flags) { assert(this==physics&&flags==0&&shootCalls==1&&!randomiseCalls&&!interactionCalls); ++interactionCalls; Event('I'); }
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
void NThingShot::CTCShotDefinitionBase::OnCollision(ECollideType type, const C3DVector& position, const C3DVector& normal, CThing* thing) { abort(); }
void NThingShot::CTCShotDefinitionBase::OnHitByMelee(CThing& hitter, CThing* weapon, const C3DVector& position, const C3DVector& direction, bool knockDown, bool blockable) { abort(); }
bool NThingShot::CTCShotDefinitionBase::GetStatToModifyOnHit(EHeroExperienceStatCategory& category) { abort(); }
float NThingShot::CTCShotDefinitionBase::GetShotDamage() const { abort(); }
long NThingShot::CTCShotDefinitionBase::GetWeaponTrailGraphic()const { abort(); }
void NThingShot::CTCShotDefinitionBase::InitMovement() { abort(); }
void NThingShot::CTCShotDefinitionBase::InitAttachedEffects() { abort(); }
void NThingShot::CTCShotDefinitionBase::UpdateMovement() { abort(); }
void NThingShot::CTCShotDefinitionBase::UpdateFacingDirection() { abort(); }
void NThingShot::CTCShotDefinitionBase::UpdateCollision(bool&) { abort(); }
void NThingShot::CTCShotDefinitionBase::UpdateEffects() { abort(); }
void NThingShot::CTCShotDefinitionBase::UpdateCreatureAwarenessOfMe() { abort(); }
void NThingShot::CTCShotDefinitionBase::OnSubShotTrigger() { abort(); }
void NThingShot::CTCShotDefinitionBase::OnPassingTarget() { abort(); }
bool NThingShot::CTCShotDefinitionBase::IsNegatedHit()const { abort(); }
bool NThingShot::CTCShotDefinitionBase::IsCreateParticleEffectOnHit()const { abort(); }

int main(){unsigned cases=0;
 for(int distance=1;distance<=4;++distance)for(int speed=1;speed<=4;++speed)for(int random=0;random<3;++random)for(int last=0;last<2;++last){
  scatter=true;fraction=random*0.5f;interactionCalls=stringsLive=worldCalls=randomiseCalls=normaliseCalls=rotateCalls=randomCalls=shootCalls=resetCalls=movingCalls=gravityCalls=lookupCalls=defCalls=deleteCalls=0;eventCount=0;events[0]=0;
  CThing parent;TestPhysics body(parent);TestShot shell(parent);NThingShot::CTCShotDefinitionDrainLife& object=*reinterpret_cast<NThingShot::CTCShotDefinitionDrainLife*>(&shell);TestShotDef def;TestDrainDef sub;CDefinitionManager definitions;
  parentThing=&parent;physics=&body;shot=&object;baseDef=&def;drainDef=&sub;manager=&definitions;parent.PhysicsTC=&body;
  body.Position=C3DVector(16,8,4);target=C3DVector(16+distance*8,8,4);object.PShotDef.Object=&def;def.AddRef();object.AccuracyPercentage=37.0f;object.Speed=-5;def.Speed=(float)(speed*8);def.PDefManager=&definitions;sub.IVRefCount=last?0:1;SubInfo.DefIndex=7;SubInfo.OriginalParentDefIndex=0;
  sub.MaxDistance=16;sub.MaxAngle=8;sub.InitialSpeed=(float)(speed*4);sub.SpeedRange=2;sub.MaxAngleToTurnDegs=0;
  assert(sizeof(TestShot)==sizeof(NThingShot::CTCShotDefinitionDrainLife));
  object.NThingShot::CTCShotDefinitionDrainLife::InitMovement();
  assert(interactionCalls==1&&normaliseCalls==3&&resetCalls==1&&movingCalls==1&&gravityCalls==1&&lookupCalls==1&&defCalls==1&&stringsLive==0);
  assert(randomiseCalls==1&&randomCalls==3&&rotateCalls==2&&worldCalls==4&&shootCalls==1);
  if(strcmp(events,"TIRNNNHDFOFOVGF")!=0){printf("events %s\n",events);abort();}
  assert(body.Velocity.X==(float)speed&&body.Velocity.Y==0&&body.Velocity.Z==0&&object.MaxAngleToTurn==0&&object.MaxCosAngleToTurn==1);
  assert(normalInput[0].X==distance*8&&normalInput[0].Y==0&&normalInput[0].Z==0);assert(normalInput[1].X==0&&normalInput[1].Y==0&&normalInput[1].Z==1);assert(normalInput[2].X==0&&normalInput[2].Y==-1&&normalInput[2].Z==0);
  float maxAngle=distance==1?4.0f:8.0f;assert(ranges[0]==2*maxAngle&&ranges[1]==maxAngle&&ranges[2]==0.5f);
  assert(rotationAxes[0].X==0&&rotationAxes[0].Y==0&&rotationAxes[0].Z==1&&rotationAxes[1].X==0&&rotationAxes[1].Y==-1&&rotationAxes[1].Z==0);
  float a=(fraction*2*maxAngle-maxAngle)/360.0f,b=(fraction*maxAngle-maxAngle)/360.0f;assert(fabs(angles[0]-a)<0.000001&&fabs(angles[1]-b)<0.000001);
  assert(fabs(object.Speed-((float)(speed*2)+0.5f*fraction))<0.000001);
  assert(sub.IVRefCount==(last?0:1)&&deleteCalls==last);object.PShotDef.Object=0;def.Release();++cases;
 }
 assert(cases==96);puts("007bd845_TEST PASS");return 0;
}