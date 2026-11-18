// Behaviour test for 0x0095B9E0 CAIStateGroup_HoverHornetQueen::DiveBomb(): the fixture of the landed same-TU sibling Attack 0x0095BCF0
// (real shared classes, recording doubles) with the row's opt-ins and body verbatim (its helper bodies included). Cases: not runnable at
// entry; the marker route both directions with wrap-around at both ends, the failed route reversing the direction both ways, not
// runnable after the route; the no-marker approach loop (in progress / finished / runnable stop); not runnable before the dive test;
// the 2D range exactly on 12 (no dive), just inside, height ignored; line of sight refused; the dive with the runnable stop after it
// and with the return to the current marker; the final runnable check.
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
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_GFFASTONEOVERSQRT_HOST_BITS_PASS23
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_MUTABLE
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
#define FABLE_RETAIL_CAISTATEGROUP_HOVERHORNETQUEEN_DIVEBOMB
#define FABLE_RETAIL_CCREATUREACTION_WASPQUEENDIVEATTACK_BASE
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_ONEPOINTLOS
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE2D_ORIGINAL_SUM_PASS23
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
#include "engine/CCreatureAction_WaspQueenDiveAttack.h"
#include "engine/CTCCreatureNavigation.h"

long CAIStateGroup_HoverHornetQueen::GetNextCirclePoint()
{
    long nextCirclePoint = CurrentCirclePoint + 1;
    if (nextCirclePoint == (long)CircleMarkers.size())
        nextCirclePoint = 0;
    return nextCirclePoint;
}

long CAIStateGroup_HoverHornetQueen::GetPreviousCirclePoint()
{
    long nextCirclePoint = CurrentCirclePoint - 1;
    if (nextCirclePoint == -1)
        nextCirclePoint = (long)CircleMarkers.size() - 1;
    return nextCirclePoint;
}

EAIActionReturn CAIStateGroup_HoverHornetQueen::GotoNextCirclePoint(EMovementType movement_type)
{
    return GotoCirclePoint(GetNextCirclePoint(), movement_type);
}

EAIActionReturn CAIStateGroup_HoverHornetQueen::GotoPreviousCirclePoint(EMovementType movement_type)
{
    return GotoCirclePoint(GetPreviousCirclePoint(), movement_type);
}

void CAIStateGroup_HoverHornetQueen::DiveBomb()
{
    const CTCCreatureNavigation* pTCNav;
    EAIActionReturn ret;
    C3DVector lineOfSightPos;
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }
    TimingCritical = true;
    if (CircleMarkers.size() > 0) {
        if (RouteDirection == WASP_DIRECTION_CLOCKWISE)
            ret = GotoNextCirclePoint(AI_MOVEMENT_HOVER_MEDIUM);
        else
            ret = GotoPreviousCirclePoint(AI_MOVEMENT_HOVER_MEDIUM);
        if (ret == AI_ACTION_FAILED)
            RouteDirection = (eWaspDirection)(RouteDirection == WASP_DIRECTION_CLOCKWISE);
        if (!IsStillRunnable()) {
            Terminate();
            TimingCritical = false;
            return;
        }
    } else {
        do {
            bool nav;
            ret = AIAction_MoveToThingProximity(ACTION_REPEAT_CALLBACK, PTargetThing, 10.0f, nav, AI_MOVEMENT_HOVER_MEDIUM, true, false, true, true, 0.0f);
        } while (ret == AI_ACTION_IN_PROGRESS && IsStillRunnable());
    }
    if (!IsStillRunnable()) {
        Terminate();
        return;
    }
    GetCreature().PeekTC(TCI_NAVIGATION, &pTCNav);
    C3DVector lineToTarget = PTargetThing.Get()->GetPos() - GetCreature().GetPos();
    C3DVector directionToTarget = lineToTarget;
    directionToTarget.Normalise();
    lineOfSightPos = PTargetThing.Get()->GetPos() - directionToTarget * 0.75f;
    if (lineToTarget.GetMagnitude2D() < 12.0f && pTCNav->IsPosNavLineOfSightTo(lineOfSightPos)) {
        AIAction_CombatPerformActionAndWait(CCreatureAction_WaspQueenDiveAttack(GetCreature(), *PTargetThing.Get()), false);
        if (!IsStillRunnable()) {
            TimingCritical = false;
            Terminate();
            return;
        }
        GotoCirclePoint(CurrentCirclePoint, AI_MOVEMENT_HOVER_MEDIUM);
        RouteDirection = (eWaspDirection)(RouteDirection == WASP_DIRECTION_CLOCKWISE);
    }
    TimingCritical = false;
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
CWideString::CWideString(){}
CWideString::~CWideString(){}
CIDrawWorld::~CIDrawWorld(){}
CWorld::~CWorld(){}
CCombatManager::CCombatManager(CWorld& w):World(w){}
CResource::CResource():ResourceList(0),PrevResource(0),NextResource(0),ResourceSize(0),LastUsedFrame(0){IVRefCount=1;}
CResource::~CResource(){}
CDefClassBase::CDefClassBase():PDefManager(0),GlobalIndex(0),Setup(false),Template(false),DefaultValsApplied(false){}
CParentDefClassBase::CParentDefClassBase(){}
CParentDefClassBase::~CParentDefClassBase(){}
void CIVCountedPointeeBase::Delete()const{abort();}
CAIStateGroupBase::CAIStateGroupBase():PBrain(0),Terminated(false){}
CCreatureActionBase::~CCreatureActionBase(){}

unsigned long GFastOneOverSqrtTable[128];
static void BuildTable()
{
    for (int i = 0; i < 128; ++i) {
        double m = ((i & 63) + 0.5) / 64.0, v = (i & 64) ? (1.0 + m) : 2.0 * (1.0 + m);
        double f = 2.0 / sqrt(v) - 1.0;
        long t = (long)(f * 256.0); if (t > 255) t = 255; if (t < 0) t = 0;
        GFastOneOverSqrtTable[i] = (unsigned long)t;
    }
}
static int g_bad=0;
static void check(bool ok, int line) { if (!ok) { printf("check failed line %d\n", line); ++g_bad; } }
static char events[256]; static unsigned int eventCount;
static void event(char c) { if (eventCount<255) { events[eventCount++]=c; events[eventCount]=0; } }
static const char* runnable; static int runnableCalls;
static class TestQueen* g_queen;
static bool termTiming[8]; static int terms;
static long gotoN[8]; static int gotoMt[8]; static int gotos; static EAIActionReturn gotoRet[8];
static EAIActionReturn moveRet[8]; static int moves; static bool moveArgsOk;
static const CThing* moveThing;
static C3DVector targetPos, queenPos, losPos; static bool losRet; static int losCalls; static const void* losThis;
static int actions, performs, deaths; static void* actCreature; static void* actEnemy; static const void* performArg; static bool performWait;
static CThing* g_target; static CThing* g_creature;
static unsigned char navTC[0x40];
struct SPair { long key; CTCBase* value; };
static SPair navPair;
CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >::const_iterator
CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >::LowerBound(const ETCInterfaceType& key) const
{
    check(key==TCI_NAVIGATION, __LINE__);
    check((const char*)this==(const char*)g_creature+0x44, __LINE__);
    navPair.key=TCI_NAVIGATION; navPair.value=(CTCBase*)navTC; return (const_iterator)&navPair;
}
const C3DVector& CThing::GetPos() const { if (this==g_target) return targetPos; check(this==g_creature, __LINE__); return queenPos; }
bool CTCCreatureNavigation::IsPosNavLineOfSightTo(const C3DVector& pos) const { event('L'); ++losCalls; losThis=this; losPos=pos; return losRet; }
void CAIStateGroupBase::Terminate() { event('T'); termTiming[terms++]=((CAIStateGroup_HoverHornetQueen*)this)->TimingCritical; }
bool CAIStateGroup_HoverHornetQueen::IsStillRunnable() { event('R'); char c=runnable[runnableCalls]; if (c) ++runnableCalls; return c!='0'; }
EAIActionReturn CAIStateGroup_HoverHornetQueen::GotoCirclePoint(long n, EMovementType mt)
{ event('G'); check(TimingCritical, __LINE__); gotoN[gotos]=n; gotoMt[gotos]=mt; return gotoRet[gotos++]; }
EAIActionReturn CAIStateGroupBase::AIAction_MoveToThingProximity(EAIActionRepeat repeat, CIntelligentPointer<const CThing> thing, float proximity,
    bool& reached, EMovementType movement, bool a, bool b, bool c, bool d, float e)
{
    event('M'); moveThing=thing.Get(); reached=true;
    if (!(repeat==ACTION_REPEAT_CALLBACK && proximity==10.0f && movement==AI_MOVEMENT_HOVER_MEDIUM && a && !b && c && d && e==0.0f)) moveArgsOk=false;
    return moveRet[moves++];
}
CCreatureAction_WaspQueenDiveAttack::CCreatureAction_WaspQueenDiveAttack(CThingCreatureBase& creature, CThingCreatureBase& enemy)
{ event('A'); ++actions; actCreature=&creature; actEnemy=&enemy; }
CCombatActionBase::~CCombatActionBase() { event('X'); ++deaths; }
EAIActionReturn CAIStateGroupBase::AIAction_CombatPerformActionAndWait(const CCombatActionBase& action, bool wait)
{ event('W'); ++performs; performArg=&action; performWait=wait; return AI_ACTION_FINISHED; }
CWorldMap& CWorld::GetWorldMap() { abort(); }
CBaseObject* CBaseIntelligentPointer::GetPItem()const{return reinterpret_cast<CBaseObject*>(PData);}
void CBaseIntelligentPointer::SetPItem(CBaseObject* p){PData=reinterpret_cast<CBaseObjectPointer*>(p);}
template CThingCreatureBase* CIntelligentPointer<CThingCreatureBase>::Get() const;
template void CIntelligentPointer<CThing>::SetPointer(const CThing*);
class TestQueen : public CAIStateGroup_HoverHornetQueen { public: void Update() { abort(); } };
static void reset(const char* run)
{
    eventCount=0; events[0]=0; runnable=run; runnableCalls=0; terms=gotos=moves=losCalls=actions=performs=deaths=0; moveArgsOk=true; moveThing=0;
    for (int i=0;i<8;++i) { gotoRet[i]=AI_ACTION_FINISHED; moveRet[i]=AI_ACTION_FINISHED; }
    losRet=true; losPos=C3DVector(0,0,0); performArg=0; performWait=true;
    g_queen->TimingCritical=false;
}
static bool close(float a, float b) { return fabs(a-b)<0.01f; }
int main()
{
    BuildTable();
    CThingAICreature queenCreature, target; g_target=&target; g_creature=&queenCreature;
    ((unsigned long*)((char*)&queenCreature+0x20))[0] |= 1UL<<TCI_NAVIGATION;
    CAIBrain brain; brain.Creature=&queenCreature; TestQueen queen; g_queen=&queen; queen.PBrain=&brain; queen.PTargetThing.Set(&target);
    queenPos=C3DVector(10,20,30); targetPos=C3DVector(30,20,0);   // 2D distance 20: no dive
    // not runnable at entry
    reset("0"); queen.DiveBomb(); check(strcmp(events,"RT")==0 && termTiming[0]==false && queen.TimingCritical==false, __LINE__);
    // markers: clockwise from 2 of 4 -> 3, finished: direction kept; out of range, final check passes
    CThing marker;
    for (int i=0;i<4;++i) queen.CircleMarkers.push_back(CIntelligentPointer<CThing>(&marker));
    queen.CurrentCirclePoint=2; queen.RouteDirection=CAIStateGroup_HoverHornetQueen::WASP_DIRECTION_CLOCKWISE;
    reset("1111"); queen.DiveBomb();
    check(strcmp(events,"RGRRR")==0 && gotoN[0]==3 && gotoMt[0]==AI_MOVEMENT_HOVER_MEDIUM && queen.RouteDirection==0 && queen.TimingCritical==false, __LINE__);
    // clockwise wrap 3 -> 0, route failed: direction reversed; then not runnable: Terminate with the timing still critical, then cleared
    queen.CurrentCirclePoint=3; reset("10"); gotoRet[0]=AI_ACTION_FAILED; queen.DiveBomb();
    check(strcmp(events,"RGRT")==0 && gotoN[0]==0 && queen.RouteDirection==1 && termTiming[0]==true && queen.TimingCritical==false, __LINE__);
    // anticlockwise 0 -> wraps to 3; in progress is not failure
    queen.CurrentCirclePoint=0; reset("1111"); gotoRet[0]=AI_ACTION_IN_PROGRESS; queen.DiveBomb(); check(gotoN[0]==3 && gotoMt[0]==AI_MOVEMENT_HOVER_MEDIUM && queen.RouteDirection==1, __LINE__);
    // anticlockwise 2 -> 1, failed: back to clockwise
    queen.CurrentCirclePoint=2; reset("1111"); gotoRet[0]=AI_ACTION_FAILED; queen.DiveBomb(); check(gotoN[0]==1 && queen.RouteDirection==0, __LINE__);
    // clockwise 0 -> 1 (no wrap)
    queen.CurrentCirclePoint=0; reset("1111"); queen.DiveBomb(); check(gotoN[0]==1, __LINE__);
    // runnable after the route but not at the dive test: Terminate, timing left critical (retail 0x0095BAD9 -> 0x0095BCDC)
    reset("110"); queen.DiveBomb(); check(strcmp(events,"RGRRT")==0 && termTiming[0]==true && queen.TimingCritical==true, __LINE__);
    // no markers: approach loop, in progress twice then finished
    queen.CircleMarkers.clear();
    reset("11111"); moveRet[0]=AI_ACTION_IN_PROGRESS; moveRet[1]=AI_ACTION_IN_PROGRESS; queen.DiveBomb();
    check(strcmp(events,"RMRMRMRR")==0 && moveArgsOk && moveThing==&target && terms==0, __LINE__);
    // in progress, then not runnable: loop stops; failed result ends the loop at once
    reset("100"); moveRet[0]=AI_ACTION_IN_PROGRESS; queen.DiveBomb(); check(strcmp(events,"RMRRT")==0, __LINE__);
    reset("111"); moveRet[0]=AI_ACTION_FAILED; queen.DiveBomb(); check(strcmp(events,"RMRR")==0, __LINE__);
    // exactly 12 on the ground plane: no line-of-sight test, no dive
    targetPos=C3DVector(22,20,0); reset("111"); queen.DiveBomb(); check(losCalls==0 && actions==0 && strcmp(events,"RMRR")==0, __LINE__);
    // just inside 12 with a large height difference: line of sight refused
    targetPos=C3DVector(21.99f,20,-50); reset("111"); losRet=false; queen.DiveBomb();
    check(losCalls==1 && losThis==navTC && actions==0 && strcmp(events,"RMRLR")==0, __LINE__);
    {   // the point 0.75 short of the target along the 3D line
        float dx=21.99f-10, dz=-80, len=(float)sqrt(dx*dx+dz*dz);
        check(close(losPos.X,21.99f-0.75f*dx/len) && close(losPos.Y,20.0f) && close(losPos.Z,-50.0f-0.75f*dz/len), __LINE__);
    }
    // in range, in sight: dive, then not runnable -> timing cleared before Terminate
    queen.CircleMarkers.push_back(CIntelligentPointer<CThing>(&marker));
    targetPos=C3DVector(10,26,30); queen.CurrentCirclePoint=0; queen.RouteDirection=CAIStateGroup_HoverHornetQueen::WASP_DIRECTION_CLOCKWISE;
    reset("1110"); queen.DiveBomb();
    check(strcmp(events,"RGRRLAWXRT")==0 && actCreature==&queenCreature && actEnemy==&target && performWait==false, __LINE__);
    check(termTiming[0]==false && actions==1 && deaths==1 && queen.RouteDirection==0, __LINE__);
    check(close(losPos.X,10.0f) && close(losPos.Y,25.25f) && close(losPos.Z,30.0f), __LINE__);
    // dive, still runnable: back to the current marker, direction reversed, final check fails -> Terminate
    queen.CurrentCirclePoint=0; reset("11110"); queen.DiveBomb();
    check(strcmp(events,"RGRRLAWXRGRT")==0 && gotoN[1]==0 && gotoMt[1]==AI_MOVEMENT_HOVER_MEDIUM && queen.RouteDirection==1 && termTiming[0]==false, __LINE__);
    // anticlockwise with one marker: 0 -> 0, the dive flips the direction back to clockwise, final check passes
    queen.RouteDirection=CAIStateGroup_HoverHornetQueen::WASP_DIRECTION_ANTICLOCKWISE; reset("11111"); queen.DiveBomb();
    check(gotoN[0]==0 && queen.RouteDirection==0 && terms==0 && queen.TimingCritical==false, __LINE__);
    // audit e11: dive with three markers from marker 1: the return goes to the CURRENT marker (1), not the next (2) or previous (0)
    queen.CircleMarkers.clear();
    for (int i=0;i<3;++i) queen.CircleMarkers.push_back(CIntelligentPointer<CThing>(&marker));
    queen.CurrentCirclePoint=1; queen.RouteDirection=CAIStateGroup_HoverHornetQueen::WASP_DIRECTION_CLOCKWISE; reset("11111"); queen.DiveBomb();
    check(strcmp(events,"RGRRLAWXRGR")==0 && gotoN[0]==2 && gotoN[1]==1 && gotoMt[1]==AI_MOVEMENT_HOVER_MEDIUM && queen.RouteDirection==1, __LINE__);
    // the helpers directly: next / previous with and without wrap, Goto* passing n and the movement type through
    queen.CircleMarkers.clear();
    for (int i=0;i<3;++i) queen.CircleMarkers.push_back(CIntelligentPointer<CThing>(&marker));
    queen.CurrentCirclePoint=0; check(queen.GetNextCirclePoint()==1 && queen.GetPreviousCirclePoint()==2, __LINE__);
    queen.CurrentCirclePoint=1; check(queen.GetNextCirclePoint()==2 && queen.GetPreviousCirclePoint()==0, __LINE__);
    queen.CurrentCirclePoint=2; check(queen.GetNextCirclePoint()==0 && queen.GetPreviousCirclePoint()==1, __LINE__);
    // audit e11: the wraps are equality tests (retail cmp/jne): an out-of-range point is not wrapped
    queen.CurrentCirclePoint=3; check(queen.GetNextCirclePoint()==4, __LINE__);
    queen.CurrentCirclePoint=-1; check(queen.GetPreviousCirclePoint()==-2, __LINE__);
    queen.CurrentCirclePoint=2;
    reset("1"); queen.TimingCritical=true; gotoRet[0]=AI_ACTION_FAILED; gotoRet[1]=AI_ACTION_IN_PROGRESS;
    check(queen.GotoNextCirclePoint(AI_MOVEMENT_HOVER_FAST)==AI_ACTION_FAILED && gotoN[0]==0 && gotoMt[0]==AI_MOVEMENT_HOVER_FAST, __LINE__);
    check(queen.GotoPreviousCirclePoint(AI_MOVEMENT_HOVER_SLOW)==AI_ACTION_IN_PROGRESS && gotoN[1]==1 && gotoMt[1]==AI_MOVEMENT_HOVER_SLOW, __LINE__);
    if (g_bad==0) puts("0095b9e0_TEST PASS");
    return g_bad;
}