// Self-contained fixture using actual shared classes, normal lifetimes and recording callee doubles.
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
enum ESex {};enum EScriptAIPriority {};enum EAIStateGroupType {};enum ECombatZone {};
#endif
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_RETAIL_STLP_HOST_TAGS_PASS23
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCOMBATMANAGER_CTOR
#define FABLE_RETAIL_CDEFENDINGCOMBATANTINFO_PRONG
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE_CTOR_DECL
#define private public
#define protected public
// CAIStateGroup_HoverHornetQueen::Attack() -- approach a combat_man position, perform the queen primary attack, then watch the target for the configured pause.
// Leaf: ?Attack@CAIStateGroup_HoverHornetQueen@@IAEXXZ
// Retail: 0x0095BCF0, 948 bytes, thiscall.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_GFFASTONEOVERSQRT_HOST_BITS_PASS23
#define FABLE_RETAIL_C3DVECTOR_SUBTRACT_XY_GROUP
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#define FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_GROUPED
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE_OUTOFLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGBASEDEF
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_COMBATMANAGER
#define FABLE_RETAIL_CWORLDMAP_HOVERQUEEN_QUERY_PASS23
#define FABLE_RETAIL_CCOMBATMANAGER
#define FABLE_RETAIL_CCOMBATMANAGER_PREFERREDPOS
#define FABLE_RETAIL_CCOMBATMANAGER_SETATTACKERREADY
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_WASPQUEENPRIMARYATTACK_PASS23
#define FABLE_RETAIL_CWASPQUEENBATTLEDEF_PASS23
#define FABLE_RETAIL_NCREATUREAI_WASP_ATTACK_PASS23
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAISTATEGROUPBASE_COMBATPERFORMACTION
#define FABLE_RETAIL_CAISTATEGROUPBASE_AIACTIONS
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE
#define FABLE_RETAIL_CAISTATEGROUP_HOVERHORNETQUEEN_ATTACK_PASS23
#define FABLE_RETAIL_CAISTATEGROUP_HOVERMOVEMENTBASE_ATTACK_PASS23
#include "engine/fable_ai_action_enums.h"
#include "engine/C3DVector.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBaseDef.h"
#include "engine/_quarantine/CWaspQueenBattleDef.h"
#include "engine/CCreatureAction_WaspQueenPrimaryAttack.h"
#include "engine/CCombatManager.h"
#include "engine/CDefinitionManager.h"
#include "engine/CWorld.h"
#include "engine/CWorldMap.h"
#include "engine/NCreatureAI.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_HoverHornetQueen.h"


#include "engine/CCamera.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CDefString.h"
template<> bool CDefClassBase::GetSubDef<CWaspQueenBattleDef>(CDefPointer<const CWaspQueenBattleDef>*)const;
template<class T> template<class U> CIntelligentPointer<T>::CIntelligentPointer(CIntelligentPointer<U>& other){SetPointer(other.Get());}
void CAIStateGroup_HoverHornetQueen::Attack()
{
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }
    CCombatManager& combat_man = GetCreature().GetCombatManager();
    if (!GetCreature().IsInMeleeAsAttacker()) {
        CThingAICreature* creature = &GetCreature();
        combat_man.RegisterCombatant(*creature, *PTargetThing.Get(), true);
    }
    if (GetCreature().IsInMeleeAsAttacker()) {
        CThing* creature = &GetCreature();
        CWorld& world = GetWorld();
        world.GetCombatManager().SetAttackerAsReadyToAttack(static_cast<CThingCreatureBase&>(*creature), true);
    }
    EAIActionReturn ret;
    do {
        if (!PTargetThing.Get() || (PTargetThing.Get()->IsDead())) {
            Terminate();
            return;
        }
        if (CircleMarkers.size() > 0) {
            CIntelligentPointer<CThing>& first = CircleMarkers[0];
            if (!first.Get() || (first.Get()->IsDead())) {
                Terminate();
                return;
            }
        }
        if (IsStopping() || !GetCreature().IsInMeleeAsAttacker()) {
            Terminate();
            return;
        }
        if (!NCreatureAI::IsToAttackTarget(&GetCreature(), PTargetThing.Get())) {
            Terminate();
            return;
        }
        C3DVector target_pos = combat_man.GetPreferredPos(GetCreature());
        float preffered_dis = (target_pos - PTargetThing.Get()->GetPos()).GetMagnitude();
        target_pos.Z = GetWorld().GetWorldMap().GetGroundSizeZAt(target_pos);
        C3DVector direction = target_pos - PTargetThing.Get()->GetPos();
        direction.Normalise();
        target_pos = PTargetThing.Get()->GetPos() + direction * preffered_dis;
        bool nav;
        ret = AIAction_MoveToPosProximityAndFacePos(ACTION_REPEAT_CALLBACK, target_pos, 0.05f, PTargetThing.Get()->GetPos(), nav, AI_MOVEMENT_HOVER_MEDIUM);
    } while (ret == AI_ACTION_IN_PROGRESS);
    if (ret != AI_ACTION_FINISHED)
        return;
    if (!IsStillRunnable() || !GetCreature().IsInMeleeAsAttacker()) {
        Terminate();
        return;
    }
    {
        CThingAICreature* creature = &GetCreature();
        AIAction_CombatPerformActionAndWait(CCreatureAction_WaspQueenPrimaryAttack(*creature, *PTargetThing.Get()), false);
    }
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }
    {
    long pause_start_frame = GetWorld().GetFrame();
    CDefPointer<const CWaspQueenBattleDef> p_wasp_def;
    GetCreature().PeekBaseDef().GetSubDef(&p_wasp_def);
    const CWaspQueenBattleDef* d = p_wasp_def.operator->();
    if (d->NumFramesPauseBetweenAttacks > 0) {
        do {
            AIAction_LookAtThing(ACTION_REPEAT_CALLBACK, PTargetThing, 0x21, 0.0027777001f);
            if (!IsStillRunnable())
                break;
        } while (GetWorld().GetFrame() - pause_start_frame < d->NumFramesPauseBetweenAttacks);
    }
    }
    if (!IsStillRunnable())
        Terminate();
}

#undef private
#undef protected

// Other virtual methods abort if called unexpectedly.
void CBaseObject::Validate() const { abort(); }
void CClass::Save(CCharString&) { abort(); }
void CThingGameObject::OnHit(CHitParameters&) { abort(); }
void CThingCreatureBase::OnHit(CHitParameters&) { abort(); }
bool CThingCreatureBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) { abort(); }
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
const CWorldMap& CWorld::PeekWorldMap() const { abort(); }
const CProceduralTexture* CWorld::DrawGetProceduralTexture() const { abort(); }
const CIDrawWorldMap* CWorld::DrawGetWorldMap() const { abort(); }
const CIDrawEnvironment* CWorld::DrawGetEnvironment() const { abort(); }
void CAIStateGroupBase::Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name) { abort(); }
void CAIStateGroupBase::OnActivate() { abort(); }
bool CAIStateGroupBase::InitCreaturePosition() { abort(); }
bool CAIStateGroupBase::AllowPriorityOverlap() { abort(); }
bool CAIStateGroupBase::IsStillValid() const { abort(); }
const CCharString* CAIStateGroupBase::GetDebugText() const { abort(); }
bool CAIStateGroupBase::IsTimingCritical() const { abort(); }
EAIStateGroupType CAIStateGroupBase::GetStateGroupType() const { abort(); }
void CAIStateGroupBase::Cleanup() { abort(); }
void CAIStateGroupBase::OnReplace() { abort(); }
bool CAIStateGroupBase::IsRunnable() { abort(); }
void CAIStateGroupBase::InitStateGroup(const CWorld& world, CThingAICreature& creature) { abort(); }
void CCreatureActionBase::EditUpdate(float) { abort(); }
void CCreatureActionBase::FinishAction() { abort(); }
bool CCreatureActionBase::IsPlayingAnimation() const { abort(); }
long CCreatureActionBase::GetAnimDelayTime() const { abort(); }
long CCreatureActionBase::GetAnimationIndex() const { abort(); }
long CCreatureActionBase::GetAnimationTransitionInTime() const { abort(); }
long CCreatureActionBase::GetAnimationTransitionOutTime() const { abort(); }
unsigned long CCreatureActionBase::GetAnimExtraFlags() const { abort(); }
void CCreatureActionBase::Initialise() { abort(); }
bool CCreatureActionBase::IsStrikeEvent(long) const { abort(); }
bool CCreatureActionBase::IsStrikeEventActive() const { abort(); }
bool CCreatureActionBase::IsStrikeAction() const { abort(); }
bool CCreatureActionBase::IsPartOfComboChain() const { abort(); }
bool CCreatureActionBase::IsStrikeEventFinishedBeingActive() const { abort(); }
bool CCreatureActionBase::IsVitalStrike() const { abort(); }
bool CCreatureActionBase::IsKnockdownStrike() const { abort(); }
bool CCreatureActionBase::IsBlockCounter() const { abort(); }
bool CCreatureActionBase::IsMeleeFlourish() const { abort(); }
bool CCreatureActionBase::IsActionWithStrikeEventHelperName() const { abort(); }
CCharString CCreatureActionBase::GetStrikeEventHelperName() const { abort(); }
bool CCreatureActionBase::IsOverrideableByActionOfSamePriority() const { abort(); }
void CCreatureActionBase::OnFrameUpdate(float) { abort(); }
bool CCreatureActionBase::IsQueuable() const { abort(); }
bool CCreatureActionBase::IsActionReadyForInterruption() const { abort(); }
bool CCreatureActionBase::IsActionReadyForInterruptionByMovement(const C3DVector &) const { abort(); }
bool CCreatureActionBase::IsBlockable() const { abort(); }
bool CCreatureActionBase::IsAffectedByHaste() const { abort(); }
bool CCreatureActionBase::IsAffectedByHeroStats() const { abort(); }
bool CCreatureActionBase::IsPhysicsPushableByHero() const { abort(); }
bool CCreatureActionBase::IsAlwaysPhysicsPushableBySelfAvoidance() const { abort(); }
bool CCreatureActionBase::IsAlwaysUseAnimationMovement() const { abort(); }
bool CCreatureActionBase::IsClearingMovementVector() const { abort(); }
bool CCreatureActionBase::IsToHaveEmptyHands() const { abort(); }
bool CCreatureActionBase::IsToReplaceItemIfRemoved() const { abort(); }
bool CCreatureActionBase::IsAllowingAnimationMovementToMoveSpeakAndWaitCameras() const { abort(); }
bool CCreatureActionBase::IsPreventingQuickAccessItemUsage() const { abort(); }
void CCreatureActionBase::SetHitter(const CThing &) { abort(); }
void CCreatureActionBase::SetHitDirection(C3DVector) { abort(); }
void CCreatureActionBase::SetHitPosition(C3DVector) { abort(); }
CTCWeaponTrail * CCreatureActionBase::GetWeaponTrailUsedInStrikeAction(long &) const { abort(); }
float CCreatureActionBase::GetTotalTimeSeconds() const { abort(); }
bool CCreatureActionBase::IsToEvadeAllHits() const { abort(); }
bool CCreatureActionBase::IsToCauseRecoil() const { abort(); }
bool CCreatureActionBase::IsAbleToBeAttacked() const { abort(); }
bool CCreatureActionBase::IsPlayHitResponse() const { abort(); }
bool CCreatureActionBase::IsPreventingNormalMovement() const { abort(); }
bool CCreatureActionBase::IsPreventingLookTurning() const { abort(); }
void CCreatureActionBase::OnBlowBlocked(bool) { abort(); }
void CCreatureActionBase::OnFollowUpActionFailedToAdd() { abort(); }
void CCreatureActionBase::UpdateActionRunningTime() { abort(); }
bool CCreatureActionBase::IsDecapitatingAttack() const { abort(); }
bool CCreatureActionBase::PreventSneaking() const { abort(); }
void CCreatureActionBase::FrameUpdate() { abort(); }
bool CCreatureActionBase::IsUsingAnimationMovement() const { abort(); }
bool CCreatureActionBase::IsUsingForcedMovement() const { abort(); }
bool CCreatureActionBase::IsMovementUsingPhysics() const { abort(); }
void CCreatureActionBase::UpdateActionMoveToPos() { abort(); }
void CCreatureActionBase::InitialiseActionMovement() { abort(); }
float CCreatureActionBase::GetFrameUpdateTime() const { abort(); }
float CCreatureActionBase::GetActionSpeedParameter() const { abort(); }
float CCreatureActionBase::GetActionStartRunningTimeSeconds() const { abort(); }
bool CCreatureActionBase::OverrideFaceWithDefault() const { abort(); }
bool CCreatureActionBase::AllowsCombinationAnims() const { abort(); }
bool CCreatureActionBase::CanAnimationTransitionOut() const { abort(); }
void CCreatureActionBase::OnAnimationLoaded() { abort(); }
bool CCreatureActionBase::WaitForMeshToLoad() const { abort(); }
void CCombatActionBase::PerformAction() { abort(); }
CCreatureActionBase* CCombatActionBase::Clone() const { abort(); }

const float DELTA_NOTIONAL_ZERO=0.0001f;
unsigned long GFastOneOverSqrtTable[128];
static CWorld* testWorld=0;
static CCombatManager* testCombat=0;
static CWorldMap testMap;
static C3DVector targetPosition(0,0,0),preferredPosition(4,0,3),movePosition;
static bool melee=true,stopping=false,attackAllowed=true;
static int runnableRemaining=-1;
static EAIActionReturn movementResult=AI_ACTION_FINISHED;
static int movementRepeats=0;
static long frame=100;
static unsigned int terminations=0,registrations=0,readyCalls=0,moveCalls=0,actionCalls=0,actionDeaths=0,lookCalls=0,subdefCalls=0;
static CWaspQueenBattleDef* testDef=0;
static char events[256];static unsigned int eventCount=0;static bool loseMeleeAfterMove=false,checkReleaseAtFinalGate=false;
static void event(char c){assert(eventCount<255);events[eventCount++]=c;events[eventCount]=0;}
CBaseClass::CBaseClass(){}
CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){}
CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CBaseObject::CBaseObject():IntelligentPointer(0){}
CBaseObject::~CBaseObject(){}
CBaseIntelligentPointer::CBaseIntelligentPointer():PData(0){}
CBaseIntelligentPointer::~CBaseIntelligentPointer(){}
CBaseObject* CBaseIntelligentPointer::GetPItem()const{return reinterpret_cast<CBaseObject*>(PData);}
void CBaseIntelligentPointer::SetPItem(CBaseObject* p){PData=reinterpret_cast<CBaseObjectPointer*>(p);}
CCharString::CCharString():PStringData(0){}
CCharString::~CCharString(){}
CWideString::CWideString(){}
CWideString::~CWideString(){}
CIDrawWorld::~CIDrawWorld(){}
CWorld::~CWorld(){}
CWorldMap& CWorld::GetWorldMap(){return testMap;}
long __fastcall CWorld::GetFrame(){event('F');long result=frame++;if(result>=103)checkReleaseAtFinalGate=true;return result;}
CCombatManager::CCombatManager(CWorld& w):World(w){}
CCombatManager& CThingCreatureBase::GetCombatManager(){assert(testCombat);return *testCombat;}
bool CThingCreatureBase::IsInMeleeAsAttacker()const{return melee;}
void CCombatManager::RegisterCombatant(CThingCreatureBase&,CThingCreatureBase&,bool value){assert(value);event('C');++registrations;melee=true;}
void CCombatManager::SetAttackerAsReadyToAttack(const CThingCreatureBase&,bool value){assert(value);event('D');++readyCalls;}
C3DVector CCombatManager::GetPreferredPos(const CThingCreatureBase&)const{return preferredPosition;}
const C3DVector& CThing::GetPos()const{return targetPosition;}
float CWorldMap::GetGroundSizeZAt(const C3DVector&)const{return 0.0f;}
namespace NCreatureAI {bool __fastcall IsToAttackTarget(CThingAICreature*,const CThingCreatureBase*){event('K');return attackAllowed;}}
CResource::CResource():ResourceList(0),PrevResource(0),NextResource(0),ResourceSize(0),LastUsedFrame(0){IVRefCount=1;}
CResource::~CResource(){}
CDefClassBase::CDefClassBase():PDefManager(0),GlobalIndex(0),Setup(false),Template(false),DefaultValsApplied(false){}
CParentDefClassBase::CParentDefClassBase(){}
CParentDefClassBase::~CParentDefClassBase(){}
void CIVCountedPointeeBase::Delete()const{abort();}
class TestBaseDef:public CThingBaseDef {public:void Transfer(CPersistContext&){}void Copy(const CDefClassBase*){}unsigned long GetSizeofClass(){return sizeof(*this);}};
class TestWaspDef:public CWaspQueenBattleDef {public:void Transfer(CPersistContext&){}void Copy(const CDefClassBase*){}unsigned long GetSizeofClass(){return sizeof(*this);}};
template<> bool CDefClassBase::GetSubDef<CWaspQueenBattleDef>(CDefPointer<const CWaspQueenBattleDef>* out)const{
 event('B');++subdefCalls;if(testDef->NumFramesPauseBetweenAttacks<=0)checkReleaseAtFinalGate=true;assert(!out->Object);out->Object=testDef;testDef->AddRef();return true;
}
CAIStateGroupBase::CAIStateGroupBase():PBrain(0),Terminated(false){}
CWorld& CAIStateGroupBase::GetWorld()const{assert(testWorld);return *testWorld;}
bool CAIStateGroupBase::IsStopping()const{event('S');return stopping;}
void CAIStateGroupBase::Terminate(){event('T');++terminations;Terminated=true;}
bool CAIStateGroup_HoverHornetQueen::IsStillRunnable(){event('R');if(checkReleaseAtFinalGate)assert(testDef->IVRefCount==1);if(runnableRemaining<0)return true;if(runnableRemaining==0)return false;--runnableRemaining;return true;}
EAIActionReturn CAIStateGroup_HoverMovementBase::AIAction_MoveToPosProximityAndFacePos(EAIActionRepeat repeat,const C3DVector& pos,float proximity,const C3DVector& facing,bool& nav,EMovementType movement){
 assert(repeat==ACTION_REPEAT_CALLBACK&&proximity==0.05f&&movement==AI_MOVEMENT_HOVER_MEDIUM);assert(&facing==&targetPosition);
 event('M');movePosition=pos;++moveCalls;nav=true;if(loseMeleeAfterMove)melee=false;
 if(movementRepeats>0){--movementRepeats;return AI_ACTION_IN_PROGRESS;}return movementResult;
}
CCreatureAction_WaspQueenPrimaryAttack::CCreatureAction_WaspQueenPrimaryAttack(CThingCreatureBase&,CThing&){event('A');++actionCalls;}
CCreatureActionBase::~CCreatureActionBase(){}
CCombatActionBase::~CCombatActionBase(){event('X');++actionDeaths;}
EAIActionReturn CAIStateGroupBase::AIAction_CombatPerformActionAndWait(const CCombatActionBase&,bool wait){event('W');assert(!wait);return AI_ACTION_FINISHED;}
EAIActionReturn CAIStateGroupBase::AIAction_LookAtThing(EAIActionRepeat repeat,CIntelligentPointer<const CThing> thing,long frames,float speed){
 assert(repeat==ACTION_REPEAT_CALLBACK&&thing.Get()!=0&&frames==0x21&&speed==0.0027777001f);event('L');++lookCalls;return AI_ACTION_FINISHED;
}
class TestQueen:public CAIStateGroup_HoverHornetQueen {public:void Update(){abort();}};
static bool close(float a,float b){return fabs(a-b)<0.0001f;}
static void reset(){eventCount=0;events[0]=0;loseMeleeAfterMove=false;checkReleaseAtFinalGate=false;terminations=registrations=readyCalls=moveCalls=actionCalls=actionDeaths=lookCalls=subdefCalls=0;melee=true;stopping=false;attackAllowed=true;runnableRemaining=-1;movementResult=AI_ACTION_FINISHED;movementRepeats=0;frame=100;}
int main(){
 memset(GFastOneOverSqrtTable,0,sizeof(GFastOneOverSqrtTable));GFastOneOverSqrtTable[64]=256;
 CWorld world;testWorld=&world;CCombatManager combat(world);testCombat=&combat;world.PThingCombatManager_ptr=&combat;
 TestBaseDef baseDef;TestWaspDef waspDef;testDef=&waspDef;waspDef.NumFramesPauseBetweenAttacks=3;
 CThingAICreature queenCreature,target;queenCreature.PBaseDef.Object=&baseDef;baseDef.AddRef();target.Dead=false;
 CAIBrain brain;brain.Creature=&queenCreature;TestQueen queen;queen.PBrain=&brain;queen.PTargetThing.Set(&target);
 reset();runnableRemaining=0;queen.Attack();assert(terminations==1&&moveCalls==0);
 reset();queen.PTargetThing.Set(0);queen.Attack();assert(terminations==1&&moveCalls==0);queen.PTargetThing.Set(&target);
 reset();target.Dead=true;queen.Attack();assert(terminations==1&&moveCalls==0);target.Dead=false;
 reset();stopping=true;queen.Attack();assert(terminations==1&&moveCalls==0);
 reset();attackAllowed=false;queen.Attack();assert(terminations==1&&moveCalls==0);
 CThing marker;marker.Dead=false;queen.CircleMarkers.push_back(CIntelligentPointer<CThing>(&marker));
 reset();queen.CircleMarkers[0].Set(0);queen.Attack();assert(terminations==1&&moveCalls==0);queen.CircleMarkers[0].Set(&marker);
 reset();marker.Dead=true;queen.Attack();assert(terminations==1&&moveCalls==0);marker.Dead=false;
 reset();melee=false;movementRepeats=2;queen.Attack();
 assert(registrations==1&&readyCalls==1&&moveCalls==3&&actionCalls==1&&actionDeaths==1&&lookCalls==3&&terminations==0&&subdefCalls==1);
 assert(strcmp(events,"RCDSKMSKMSKMRAWXRFBLRFLRFLRFR")==0);
 assert(close(movePosition.X,5)&&close(movePosition.Y,0)&&close(movePosition.Z,0));assert(waspDef.IVRefCount==1);
 reset();movementRepeats=2;loseMeleeAfterMove=true;queen.Attack();assert(moveCalls==1&&actionCalls==0&&terminations==1);
 reset();movementResult=AI_ACTION_FAILED;queen.Attack();assert(moveCalls==1&&actionCalls==0&&terminations==0&&subdefCalls==0);
 reset();runnableRemaining=1;queen.Attack();assert(moveCalls==1&&actionCalls==0&&terminations==1);
 reset();runnableRemaining=2;queen.Attack();assert(actionCalls==1&&actionDeaths==1&&terminations==1&&subdefCalls==0);
 reset();runnableRemaining=3;queen.Attack();assert(actionCalls==1&&lookCalls==1&&terminations==1&&waspDef.IVRefCount==1);
 reset();waspDef.NumFramesPauseBetweenAttacks=0;queen.Attack();assert(actionCalls==1&&lookCalls==0&&subdefCalls==1&&waspDef.IVRefCount==1);
 puts("0095bcf0_TEST PASS");return 0;
}