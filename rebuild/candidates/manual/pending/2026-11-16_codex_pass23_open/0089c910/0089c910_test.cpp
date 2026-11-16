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
#endif
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_RETAIL_STLP_HOST_TAGS_PASS23
#define FABLE_RETAIL_GFFASTONEOVERSQRT_HOST_BITS_PASS23
#define FABLE_RETAIL_CTCPHYSICSBASE_ORIGINAL_CTOR_PASS23
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CGAME_CORE_PASS23
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define private public
#define protected public
// 0089c910 CGameScriptInterface::CanThingBe_Sensed_ByOtherThing
// Grade: DIFFER(1016v1016) -- 974 code bytes versus976; x87 compare/pop pair differs; shifted local targets
// Flags: /d2inlS100
// Inline-budget: /d2inlS100 expands the four radius magnitude sites (0089CA58, 0089CABB, 0089CB64, 0089CC94); plain inline leaves calls. Dot is ordinary inline.
// Leaf: ?CanThingBe_Sensed_ByOtherThing@CGameScriptInterface@@ABE_NABVCScriptThing@@0J@Z
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_DOT_ORIGINAL_SCOPE_PASS23
#define FABLE_RETAIL_C3DVECTOR_SQUARED_MAGNITUDE_ORIGINAL_EXPR_PASS23
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_SQUARE_ORIGINAL_FORMAL
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION

#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPERCEIVETHING
#define FABLE_RETAIL_CTCPERCEIVETHING_AWARENESS
#define FABLE_RETAIL_CTCPERCEIVETHING_AWARE_CALLED
#define FABLE_RETAIL_SENSED_PASS23
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCPerceiveThing.h"
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
#include "engine/CGameScriptInterface.h"
#include <math.h>
#ifdef _MSC_VER
#pragma intrinsic(cos)
#endif

bool CGameScriptInterface::CanThingBe_Sensed_ByOtherThing(const CScriptThing& thing, const CScriptThing& other_thing, long sense) const
{
    if (!thing.IsAlive() || !other_thing.IsAlive())
        return false;
    CThing* p_see_er = other_thing.GetPThing();
    CThing* p_see_ee = thing.GetPThing();
    CTCPerceiveThing* tc_perception;
    if (p_see_er->GetTC(TCI_PERCEIVE_THING, &tc_perception)) {
        const CThing* p_perceived_thing = tc_perception->GetPerceivedThing();
        if (p_perceived_thing && p_perceived_thing == p_see_ee) {
            switch (sense) {
            case 1:
                return tc_perception->IsThingLineOfSight();
            case 2:
                return tc_perception->IsThingExtendedLineOfSight();
            case 3:
                return tc_perception->IsThingSmelt();
            case 4:
                return tc_perception->IsThingHeard();
            case 5:
                return tc_perception->IsAwareOfThing();
            }
        } else {
            switch (sense) {
            case 5: {
                float sense_radius = std::max(tc_perception->GetSmellRadius(), tc_perception->GetSoundRadius());
                C3DVector from_see_er_to_see_ee=p_see_ee->GetPos() - p_see_er->GetPos();
                if (from_see_er_to_see_ee.GetSquaredMagnitude() <= sense_radius * sense_radius)
                    return true;
            }
            case 2: {
                float sight_radius = tc_perception->GetExtendedSightRadius();
                C3DVector from_see_er_to_see_ee = p_see_ee->GetPos() - p_see_er->GetPos();
                if (from_see_er_to_see_ee.GetSquaredMagnitude() <= sight_radius * sight_radius) {
                    from_see_er_to_see_ee.Normalise();
                    return cos(tc_perception->GetFOV() * (double)6.2831855f) < p_see_er->GetPhysicsTC()->GetFacingVector().Dot(from_see_er_to_see_ee);
                }
                return false;
            }
            case 1: {
                float sight_radius = tc_perception->GetSightRadius();
                C3DVector from_see_er_to_see_ee = p_see_ee->GetPos() - p_see_er->GetPos();
                if (from_see_er_to_see_ee.GetSquaredMagnitude() <= sight_radius * sight_radius) {
                    from_see_er_to_see_ee.Normalise();
                    return cos(tc_perception->GetFOV() * (double)6.2831855f) < p_see_er->GetPhysicsTC()->GetFacingVector().Dot(from_see_er_to_see_ee);
                }
                return false;
            }
            case 3: {
                float smell_radius = tc_perception->GetSmellRadius();
                C3DVector from_see_er_to_see_ee=p_see_ee->GetPos() - p_see_er->GetPos();
                return from_see_er_to_see_ee.GetSquaredMagnitude() <= smell_radius * smell_radius;
            }
            case 4: {
                float hearing_radius = tc_perception->GetSoundRadius();
                C3DVector from_see_er_to_see_ee=p_see_ee->GetPos() - p_see_er->GetPos();
                return from_see_er_to_see_ee.GetSquaredMagnitude() <= hearing_radius * hearing_radius;
            }
            }
        }
    }
    return false;
}

#include "engine/CWorld.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CPlayerManager.h"
#include "engine/CMainGameComponent.h"
#include "engine/CDisplayEngine.h"
#include "engine/CRightHandedSet.h"

#include "engine/C2DVector.h"
#include "engine/CRGBColour.h"

#include "engine/CGame.h"
#include "engine/CMainGameComponentInit.h"

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
CTCPerceiveThing::CTCPerceiveThing(CThing& thing):CTCBase(thing){}
CTCPerceiveThing::~CTCPerceiveThing(){}
CScriptThing::CScriptThing(){}
CScriptThing::~CScriptThing(){}
CGameScriptInterfaceBase::~CGameScriptInterfaceBase(){}
CGameScriptInterface::CGameScriptInterface(CMainGameComponent&,const CGameDefinitionManager&,CWorld&,CPlayerManager&,CDisplayEngine&){}
CGameScriptInterface::~CGameScriptInterface(){}
static const CThing* currentTarget;
static CThing* viewedThing;
static CThing* observerThing;
static const CScriptThing* viewedScript;
static bool viewedAlive,observerAlive;
static C3DVector viewedPos,observerPos,facing;
static float angle;
static unsigned aliveCalls,thingCalls,posCalls,facingCalls,radiusCalls;
const CThing* CTCPerceiveThing::GetPerceivedThing()const{return currentTarget;}
bool CTCPerceiveThing::IsAwareOfThing()const{return AwarenessTimer!=0.0f;}
float CTCPerceiveThing::GetSoundRadius()const{++radiusCalls;return SoundRadius;}
float CTCPerceiveThing::GetSmellRadius()const{++radiusCalls;return SmellRadius;}
float CTCPerceiveThing::GetSightRadius()const{++radiusCalls;return SightRadius;}
float CTCPerceiveThing::GetExtendedSightRadius()const{++radiusCalls;return ExtendedSightRadius;}
float CTCPerceiveThing::GetFOV()const{return angle;}
C3DVector CTCPhysicsBase::GetFacingVector()const{++facingCalls;return facing;}
CThing* CScriptThing::GetPThing()const{++thingCalls;return this==viewedScript?viewedThing:observerThing;}
bool CScriptThing::IsAlive()const{++aliveCalls;return this==viewedScript?viewedAlive:observerAlive;}
const C3DVector& CThing::GetPos()const{++posCalls;return this==viewedThing?viewedPos:observerPos;}
class FixturePhysics:public CTCPhysicsBase{public:FixturePhysics(CThing& t):CTCPhysicsBase(t){}CCharString GetClassTypeName()const{abort();}};
unsigned long GFastOneOverSqrtTable[128];

CGame::CGame():CurrentGameComponent(0),Quit(false){}CGame::~CGame(){}
CGameComponent::CGameComponent(CGame&g):Quit(false),Running(false),Game(g){}CGameComponent::~CGameComponent(){}
CMainGameComponent::CMainGameComponent(CGame&g,const CMainGameComponentInit&):CGameComponent(g){}CMainGameComponent::~CMainGameComponent(){}
CMainGameComponentInit::CMainGameComponentInit(){}CMainGameComponentInit::~CMainGameComponentInit(){}
CInitBaseClass::CInitBaseClass():Valid(false){}CInitBaseClass::~CInitBaseClass(){}
CGameDefinitionManager::CGameDefinitionManager(){}CGameDefinitionManager::~CGameDefinitionManager(){}

CDefinitionManager::~CDefinitionManager(){}
CResourceList::~CResourceList(){}

CResource::~CResource(){}
CResource::CResource():ResourceList(0),PrevResource(0),NextResource(0),ResourceSize(0),LastUsedFrame(0){IVRefCount=1;}
CFailedAllocationHandler::~CFailedAllocationHandler(){}

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
ETCInterfaceType CTCPerceiveThing::GetInterfaceType() const { abort(); }
CCharString CTCPerceiveThing::GetClassTypeName() const { abort(); }
void CTCPerceiveThing::OnCreate() { abort(); }
void CTCPerceiveThing::FrameUpdate() { abort(); }
void CTCPerceiveThing::BulletTimeFrameUpdate() { abort(); }
bool CTCPerceiveThing::HasFrameUpdate() const { abort(); }
void CTCPerceiveThing::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&) { abort(); }
void CTCPerceiveThing::OnKill() { abort(); }
void CTCPerceiveThing::OnLevelUnload() { abort(); }
void CTCPerceiveThing::OnSetInLimbo(bool) { abort(); }
const CCharString& CScriptThing::GetName() const { abort(); }
CCharString CScriptThing::GetDefName() const { abort(); }
CCharString CScriptThing::GetDataString() const { abort(); }
void CScriptThing::SetDataString(const CCharString&) { abort(); }
C3DVector CScriptThing::GetFocalPos() const { abort(); }
const C3DVector& CScriptThing::GetPos() const { abort(); }
C3DVector CScriptThing::GetHomePos() const { abort(); }
CCharString CScriptThing::GetCurrentMapName() const { abort(); }
CCharString CScriptThing::GetHomeMapName() const { abort(); }
float CScriptThing::GetAngleXY() const { abort(); }
FableNScriptUInt64 CScriptThing::GetPThingUniqueID() const { abort(); }
EAIStateGroupType CScriptThing::GetCurrentStateGroupType() const { abort(); }
EScriptAIPriority CScriptThing::GetCurrentScriptPriority() const { abort(); }
bool CScriptThing::IsSneaking() const { abort(); }
bool CScriptThing::IsAwareOfHero() const { abort(); }
bool CScriptThing::MsgIsTriggeredBy(const CCharString&) const { abort(); }
bool CScriptThing::MsgIsKilledBy(const CCharString&) const { abort(); }
CScriptThing CScriptThing::MsgWhoKilledMe() const { abort(); }
bool CScriptThing::MsgIsUsedBy(const CCharString&) const { abort(); }
bool CScriptThing::MsgIsHitBy(const CCharString&) const { abort(); }
bool CScriptThing::MsgIsHitByWithFlourish(const CCharString&) const { abort(); }
bool CScriptThing::MsgIsHitByWithDecapitate(const CCharString&) const { abort(); }
bool CScriptThing::MsgIsHitByWithWeapon(const CCharString&, const CCharString&) const { abort(); }
bool CScriptThing::MsgIsHitByWithProjectileWeapon(const CCharString&, float&) const { abort(); }
CScriptThing CScriptThing::MsgWhoHitMe() const { abort(); }
bool CScriptThing::MsgIsTalkedToBy(const CCharString&) const { abort(); }
CScriptThing CScriptThing::MsgWhoTalkedToMe() const { abort(); }
bool CScriptThing::MsgExpressionPerformedTo(CCharString&) const { abort(); }
CScriptThing CScriptThing::MsgWhoExpressedToMe() const { abort(); }
long CScriptThing::MsgHowLongWasExpressionPerformed() const { abort(); }
bool CScriptThing::MsgIsKnockedOutBy(const CCharString&) const { abort(); }
CScriptThing CScriptThing::MsgWhoKnockedMeOut() const { abort(); }
bool CScriptThing::MsgReceivedMoney(long&) const { abort(); }
bool CScriptThing::MsgIsPresentedWithItem(CCharString&) const { abort(); }
bool CScriptThing::MsgReceivedInventoryItem() const { abort(); }
bool CScriptThing::MsgPerformedSpecialAbility(EHeroAbility) const { abort(); }
bool CScriptThing::MsgPerformedAnySpecialAbility() const { abort(); }
bool CScriptThing::MsgPerformedAnyAggressiveSpecialAbility() const { abort(); }
bool CScriptThing::MsgPerformedAnyNonAggressiveSpecialAbility() const { abort(); }
bool CScriptThing::MsgIsHitBySpecialAbilityFrom(EHeroAbility, const CCharString&) const { abort(); }
bool CScriptThing::MsgIsHitByAnySpecialAbilityFrom(const CCharString&) const { abort(); }
bool CScriptThing::MsgIsHitByAnyAggressiveSpecialAbilityFrom(const CCharString&) const { abort(); }
bool CScriptThing::MsgIsHitByAnyNonAggressiveSpecialAbilityFrom(const CCharString&) const { abort(); }
bool CScriptThing::MsgOpenedChest() const { abort(); }
bool CScriptThing::MsgHitEnemyWithMeleeWeapon() const { abort(); }
bool CScriptThing::MsgHitFriendWithMeleeWeapon() const { abort(); }
bool CScriptThing::MsgHitEnemyWithRangedWeapon() const { abort(); }
bool CScriptThing::MsgHitFriendWithRangedWeapon() const { abort(); }
bool CScriptThing::MsgHitEnemyWithBareHands() const { abort(); }
bool CScriptThing::MsgHitFriendWithBareHands() const { abort(); }
bool CScriptThing::MsgAttackedWithMeleeWeaponWithoutHittingAnything() const { abort(); }
bool CScriptThing::MsgAttackedWithBareHandsWithoutHittingAnything() const { abort(); }
bool CScriptThing::MsgTalkedToAnyone() const { abort(); }
bool CScriptThing::MsgGetThingsKilled(std::vector<unsigned long,std::allocator<unsigned long> >&) const { abort(); }
bool CScriptThing::MsgPerformedFlourish() const { abort(); }
bool CScriptThing::MsgPerformedSuccessfulFlourish() const { abort(); }
bool CScriptThing::MsgOnMoralityChanged(long&) const { abort(); }
bool CScriptThing::MsgOnCutSceneAnimEvent(CCharString&) const { abort(); }
bool CScriptThing::MsgIsKicked() const { abort(); }
bool CScriptThing::IsUnconscious() const { abort(); }
bool CScriptThing::IsUsable() const { abort(); }
bool CScriptThing::IsBeingCarriedBy(const CCharString&) const { abort(); }
bool CScriptThing::IsOpenDoor() const { abort(); }
bool CScriptThing::IsSummonedCreature() const { abort(); }
void CScriptThing::SetAsUsable(bool) { abort(); }
void CScriptThing::SetFriendsWithEverythingFlag(bool) { abort(); }
bool CScriptThing::GetActivationTriggerStatus() const { abort(); }
void CScriptThing::SetActivationTriggerStatus(bool) { abort(); }
void CScriptThing::SetToKillOnLevelUnload(bool) { abort(); }
void CScriptThing::UpdateThingAttachment() { abort(); }
void CScriptThing::IncrementScriptCounter() { abort(); }
void CScriptThing::DecrementScriptCounter() { abort(); }
long CScriptThing::GetScriptCounter() { abort(); }
bool CScriptThing::IsDead() const { abort(); }
bool CScriptThing::IsNull() const { abort(); }
bool CScriptThing::IsEqualTo(const CScriptThing&) const { abort(); }
void CGameScriptInterface::Error(const CCharString&, const CCharString&, const CCharString&, unsigned long) const { abort(); }
void CGameScriptInterface::TraceMessage(const CCharString&) const { abort(); }
void CGameScriptInterface::Validate() const { abort(); }
void CGameScriptInterface::SetDebugCameraType(long) const { abort(); }
void CGameScriptInterface::DeactivateBoastUi() const { abort(); }
bool CGameScriptInterface::IsXbox() const { abort(); }
void CGameScriptInterface::NewScriptFrame() const { abort(); }
bool CGameScriptInterface::StartScriptingEntity(const CScriptThing&, CScriptGameResourceObjectScriptedThingBase&, EScriptAIPriority) { abort(); }
bool CGameScriptInterface::IsEntityUnderScriptedControl(const CScriptThing&) const { abort(); }
bool CGameScriptInterface::IsActiveThreadTerminating() const { abort(); }
bool CGameScriptInterface::IsLevelLoaded(const CCharString&) const { abort(); }
bool CGameScriptInterface::IsRegionLoaded(const CCharString&) const { abort(); }
bool CGameScriptInterface::IsRegionLoadedAndPreloaded(const CCharString&) const { abort(); }
bool CGameScriptInterface::IsRegionDefLoaded(const CCharString&) const { abort(); }
const CCharString& CGameScriptInterface::GetRegionName() const { abort(); }
bool CGameScriptInterface::MsgIsLevelLoaded(const CCharString&) const { abort(); }
bool CGameScriptInterface::MsgIsLevelUnloaded(const CCharString&) const { abort(); }
bool CGameScriptInterface::MsgOnLevelLoaded(std::list<CCharString,std::allocator<CCharString> >&) const { abort(); }
bool CGameScriptInterface::MsgOnLevelUnloaded(std::list<CCharString,std::allocator<CCharString> >&) const { abort(); }
bool CGameScriptInterface::MsgIsRegionLoaded(const CCharString&) const { abort(); }
bool CGameScriptInterface::MsgIsRegionUnloaded(const CCharString&) const { abort(); }
bool CGameScriptInterface::MsgOnRegionLoaded(CCharString&) const { abort(); }
bool CGameScriptInterface::MsgOnRegionUnloaded(CCharString&) const { abort(); }
bool CGameScriptInterface::MsgOnRegionPreunload(CCharString&) const { abort(); }
bool CGameScriptInterface::MsgOnQuestCompleted(const CCharString&) const { abort(); }
bool CGameScriptInterface::MsgOnAnyQuestCompleted(CCharString&) const { abort(); }
bool CGameScriptInterface::MsgOnQuestFailed(const CCharString&) const { abort(); }
bool CGameScriptInterface::MsgOnQuestCompletedBeforeScreenShown(const CCharString&) const { abort(); }
bool CGameScriptInterface::MsgOnQuestFailedBeforeScreenShown(const CCharString&) const { abort(); }
bool CGameScriptInterface::MsgOnQuestAccepted(CCharString&) const { abort(); }
bool CGameScriptInterface::MsgOnFeatAccepted(long&) const { abort(); }
bool CGameScriptInterface::MsgIsBoastMade(long) const { abort(); }
bool CGameScriptInterface::MsgOnBoastMade(long&, CCharString&) const { abort(); }
bool CGameScriptInterface::MsgOnBoastsMade(std::vector<std::pair<long,CCharString>,std::allocator<std::pair<long,CCharString> > >&) const { abort(); }
void CGameScriptInterface::RemoveBoastMessage() const { abort(); }
bool CGameScriptInterface::IsQuestStartScreenActive(CCharString&) const { abort(); }
bool CGameScriptInterface::MsgOnLeavingQuestStartScreen(CCharString&) const { abort(); }
bool CGameScriptInterface::MsgOnLeavingExperienceSpendingScreen() const { abort(); }
long CGameScriptInterface::MsgIsQuestionAnsweredYesOrNo() const { abort(); }
bool CGameScriptInterface::MsgIsGameInfoClickedPast() const { abort(); }
bool CGameScriptInterface::MsgIsTutorialClickedPast() const { abort(); }
bool CGameScriptInterface::MsgIsActionModeButtonPressed() const { abort(); }
bool CGameScriptInterface::MsgOnExpressionPerformed(CCharString&) const { abort(); }
bool CGameScriptInterface::MsgIsCutSceneSkipped() const { abort(); }
void CGameScriptInterface::RemoveAllCutSceneSkippedMessages() const { abort(); }
bool CGameScriptInterface::MsgOnHeroHairTypeChanged(EClothingCoversArea, CCharString&) const { abort(); }
bool CGameScriptInterface::MsgOnHeroUsedTeleporter(CCharString&) const { abort(); }
bool CGameScriptInterface::MsgOnHeroUsedGuildSeal() const { abort(); }
bool CGameScriptInterface::MsgOnGameSavedManually() const { abort(); }
bool CGameScriptInterface::MsgOnHeroSlept(bool&) const { abort(); }
bool CGameScriptInterface::MsgOnHeroFiredRangedWeapon() const { abort(); }
bool CGameScriptInterface::MsgOnHeroCastSpell(EHeroAbility&) const { abort(); }
bool CGameScriptInterface::MsgOnHeroPickedPocket(CScriptThing&) const { abort(); }
bool CGameScriptInterface::MsgOnHeroPickedLock(CScriptThing&) const { abort(); }
bool CGameScriptInterface::MsgOnFishingGameFinished(CScriptThing&) const { abort(); }
bool CGameScriptInterface::MsgOnTavernGameFinished(const CScriptThing&) const { abort(); }
bool CGameScriptInterface::MsgOnHeroRewardedWithItemsFrom(CCharString&) const { abort(); }
bool CGameScriptInterface::MsgOnChestOpeningCancelled() const { abort(); }
void CGameScriptInterface::DontPopulateNextLoadedRegion() const { abort(); }
void CGameScriptInterface::GetWanderingPopulationScriptDefNameInCurrentRegion(CCharString&) const { abort(); }
void CGameScriptInterface::GetWanderingPopulationScriptDefNameInRegion(const CCharString&, CCharString&) const { abort(); }
bool CGameScriptInterface::IsHeroAllowedHenchmenInCurrentRegion() const { abort(); }
bool CGameScriptInterface::IsHeroAllowedHenchmenInRegion(const CCharString&) const { abort(); }
void CGameScriptInterface::PostAddScriptedEntities() const { abort(); }
bool CGameScriptInterface::IsPlayerHoldingUnsheatheRangedWeaponButton() const { abort(); }
bool CGameScriptInterface::IsPlayerHoldingLockTargetButton() const { abort(); }
bool CGameScriptInterface::IsPlayerHoldingFireRangedWeaponButton() const { abort(); }
bool CGameScriptInterface::IsPlayerHoldingFirstPersonTargetingButton() const { abort(); }
bool CGameScriptInterface::IsHeroInProjectileWeaponMode() const { abort(); }
CScriptThing& CGameScriptInterface::GetHero() { abort(); }
CScriptThing CGameScriptInterface::GetHeroTargetedThing() const { abort(); }
CScriptThing CGameScriptInterface::GetThingWithScriptName(const CCharString&, const CCharString&) const { abort(); }
CScriptThing CGameScriptInterface::GetThingWithScriptName(const CCharString&) const { abort(); }
CScriptThing CGameScriptInterface::GetRandomThingWithScriptName(const CCharString&) const { abort(); }
long CGameScriptInterface::GetAllThingsWithScriptName(const CCharString&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { abort(); }
long CGameScriptInterface::GetAllCreaturesInAreaWithScriptName(const CCharString&, const C3DVector&, float, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { abort(); }
CScriptThing CGameScriptInterface::GetNearestWithScriptName(const CScriptThing&, const CCharString&) const { abort(); }
CScriptThing CGameScriptInterface::GetFurthestWithScriptName(const CScriptThing&, const CCharString&) const { abort(); }
long CGameScriptInterface::GetAllThingsWithDefNameByDistanceFrom(const CScriptThing&, const CCharString&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { abort(); }
long CGameScriptInterface::GetAllThingsWithDefName(const CCharString&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { abort(); }
CScriptThing CGameScriptInterface::GetNearestWithDefName(const CScriptThing&, const CCharString&) const { abort(); }
CScriptThing CGameScriptInterface::GetFurthestWithDefName(const CScriptThing&, const CCharString&) const { abort(); }
CScriptThing CGameScriptInterface::GetThingWithUID(unsigned long long) const { abort(); }
long CGameScriptInterface::GetAllCreaturesExcludingHero(std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { abort(); }
long CGameScriptInterface::GetAllThingsInLevel(const CCharString&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { abort(); }
bool CGameScriptInterface::IsThingWithThisUIDAlive(unsigned long long) const { abort(); }
long CGameScriptInterface::RegisterTimer() { abort(); }
void CGameScriptInterface::DeregisterTimer(long) { abort(); }
void CGameScriptInterface::SetTimer(long, long) { abort(); }
long CGameScriptInterface::GetTimer(long) const { abort(); }
CScriptThing CGameScriptInterface::CreateCreature(const CCharString&, const C3DVector&, const CCharString&, bool) const { abort(); }
CScriptThing CGameScriptInterface::CreateCreatureNearby(const CCharString&, const C3DVector&, float, const CCharString&, bool) const { abort(); }
CScriptThing CGameScriptInterface::CreateCreatureOnEntity(const CCharString&, const CScriptThing&, const CCharString&) const { abort(); }
CScriptThing CGameScriptInterface::TurnCreatureInto(const CScriptThing&, const CCharString&) const { abort(); }
void CGameScriptInterface::SetCreatureCreationDelayFrames(long) { abort(); }
void CGameScriptInterface::ResetCreatureCreationDelayFrames() { abort(); }
CScriptThing CGameScriptInterface::CreateObject(const CCharString&, const C3DVector&, const CCharString&) const { abort(); }
CScriptThing CGameScriptInterface::CreateObject(const CCharString&, const C3DVector&, float, const CCharString&) const { abort(); }
CScriptThing CGameScriptInterface::CreateObjectOnEntity(const CCharString&, const CScriptThing&, const CCharString&) const { abort(); }
CScriptThing CGameScriptInterface::CreateEffect(const CCharString&, const CScriptThing&, const CCharString&, const CCharString&, bool, bool) const { abort(); }
CScriptThing CGameScriptInterface::CreateEffect(const CCharString&, const C3DVector&, const CCharString&, float, bool, bool) const { abort(); }
CScriptThing CGameScriptInterface::CreateLight(const C3DVector&, const CRGBColour&, const CCharString&, float, float, bool) const { abort(); }
CScriptThing CGameScriptInterface::CreateExperienceOrb(const C3DVector&, long) const { abort(); }
CScriptThing CGameScriptInterface::CreateExplosion(const CCharString&, const C3DVector&, const C3DVector*, CCharString) const { abort(); }
CScriptThing CGameScriptInterface::CreatePhysicalBarrier(float, const C3DVector&, const C3DVector*, CCharString) const { abort(); }
CScriptThing CGameScriptInterface::CreateRumble(const C3DVector&, float, float, CCharString) const { abort(); }
void CGameScriptInterface::ClearAllRumbles() const { abort(); }
void CGameScriptInterface::RemoveThing(const CScriptThing&, bool, bool) const { abort(); }
void CGameScriptInterface::ShowOnScreenMessage(const CCharString&, float) const { abort(); }
void CGameScriptInterface::ShowOnScreenMessage(const CCharString&, const CCharString&, const CCharString&, float) const { abort(); }
void CGameScriptInterface::ShowOnScreenMessage(const C2DVector&, const CCharString&, const CRGBColour&, const CCharString&) const { abort(); }
void CGameScriptInterface::AddScreenMessage(const CCharString&, ETextGroupSelectionMethod) const { abort(); }
void CGameScriptInterface::AddScreenTitleMessage(const CCharString&, float, bool) const { abort(); }
void CGameScriptInterface::GiveHeroYesNoQuestion(const CCharString&, const CCharString&, const CCharString&, const CCharString&, bool) const { abort(); }
void CGameScriptInterface::DisplayGameInfo(const CCharString&) const { abort(); }
void CGameScriptInterface::DisplayGameInfoText(const CWideString&) const { abort(); }
bool CGameScriptInterface::IsSafeToDisplayGameInfo() const { abort(); }
bool CGameScriptInterface::DisplayTutorial(ETutorialCategory) const { abort(); }
bool CGameScriptInterface::IsTutorialSystemEnabled() const { abort(); }
void CGameScriptInterface::GiveHeroWeapon(const CCharString&, bool) const { abort(); }
void CGameScriptInterface::GiveHeroObject(const CCharString&, long, bool) const { abort(); }
void CGameScriptInterface::SetWeaponAsHerosActiveWeapon(const CCharString&) const { abort(); }
void CGameScriptInterface::GiveHeroItem(const CScriptThing&) const { abort(); }
bool CGameScriptInterface::GiveHeroItemsFromContainer(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::TakeObjectFromHero(const CCharString&) const { abort(); }
void CGameScriptInterface::GiveHeroGold(long) const { abort(); }
long CGameScriptInterface::GetHeroGold() const { abort(); }
void CGameScriptInterface::GiveHeroExperience(long) const { abort(); }
void CGameScriptInterface::SetHeroAbleToGainExperience(bool) const { abort(); }
void CGameScriptInterface::SheatheHeroWeapons() const { abort(); }
void CGameScriptInterface::SetHeroWillAsUsable(bool) const { abort(); }
void CGameScriptInterface::SetHeroWeaponsAsUsable(bool) const { abort(); }
void CGameScriptInterface::SetWeaponOutCrimeEnabled(bool) const { abort(); }
void CGameScriptInterface::SetGuardsIgnoreCrimes(bool) const { abort(); }
void CGameScriptInterface::RemoveAllHeroWeapons() const { abort(); }
bool CGameScriptInterface::IsReportedOrUnreportedCrimeKnown(const CScriptThing&) const { abort(); }
void CGameScriptInterface::ConfiscateAllHeroItems() const { abort(); }
void CGameScriptInterface::ConfiscateAllHeroWeapons() const { abort(); }
void CGameScriptInterface::ConfiscateItemsOfTypeFromHero(const CCharString&) const { abort(); }
void CGameScriptInterface::ReturnAllConfiscatedItemsToHero() const { abort(); }
bool CGameScriptInterface::GiveHeroTutorial(ETutorialCategory) const { abort(); }
void CGameScriptInterface::MakeHeroCarryItemInHand(const CCharString&) const { abort(); }
void CGameScriptInterface::MakeHeroCarryItemInHand(const CScriptThing&, bool, bool) const { abort(); }
void CGameScriptInterface::AddTattooToHero(const CCharString&) const { abort(); }
void CGameScriptInterface::GiveHeroAbility(EHeroAbility, bool) const { abort(); }
bool CGameScriptInterface::IsPlayerZTargetingThing(const CScriptThing&) const { abort(); }
bool CGameScriptInterface::IsPlayerCreatureBlocking() const { abort(); }
bool CGameScriptInterface::IsPlayerCreatureReadyToFireProjectileWeapon(float&) const { abort(); }
long CGameScriptInterface::GetPlayerCreatureCombatMultiplier() const { abort(); }
long CGameScriptInterface::GetPlayerCreatureCombatMultiplierRunningNumHits() const { abort(); }
void CGameScriptInterface::ResetPlayerCreatureCombatMultiplier() const { abort(); }
bool CGameScriptInterface::IsPlayerCreatureFlourishEnabled() const { abort(); }
void CGameScriptInterface::SetPlayerCreatureOnlyTarget(const CScriptThing&) const { abort(); }
void CGameScriptInterface::ResetPlayerCreatureOnlyTarget() const { abort(); }
void CGameScriptInterface::RespawnHero(bool) const { abort(); }
void CGameScriptInterface::GiveHeroMorality(float) const { abort(); }
float CGameScriptInterface::GetHeroMorality() const { abort(); }
EMorality CGameScriptInterface::GetHeroMoralityCategory() const { abort(); }
void CGameScriptInterface::ChangeHeroMoralityDueToTheft() const { abort(); }
void CGameScriptInterface::ChangeHeroMoralityDueToPicklock() const { abort(); }
void CGameScriptInterface::GiveHeroRenownPoints(long) const { abort(); }
long CGameScriptInterface::GetHeroRenownLevel() const { abort(); }
bool CGameScriptInterface::IsHeroRenownLevelFull() const { abort(); }
void CGameScriptInterface::IncreaseHeroRenownLevel() const { abort(); }
long CGameScriptInterface::GetHeroStrengthLevel() const { abort(); }
long CGameScriptInterface::GetHeroSkillLevel() const { abort(); }
long CGameScriptInterface::GetHeroWillLevel() const { abort(); }
long CGameScriptInterface::GetHeroStatLevel(EHeroTrainableStatType) const { abort(); }
long CGameScriptInterface::GetHeroStatMax(EHeroTrainableStatType) const { abort(); }
void CGameScriptInterface::SetHeroAge(float) const { abort(); }
float CGameScriptInterface::GetHeroAge() const { abort(); }
void CGameScriptInterface::SetHeroAsTeenager(bool) const { abort(); }
void CGameScriptInterface::SetHeroAsApprentice(bool) const { abort(); }
float CGameScriptInterface::GetDistanceHeroCanBeHeardFrom() const { abort(); }
long CGameScriptInterface::GetHeroRoughExperienceLevel() const { abort(); }
long CGameScriptInterface::GetHeroExperienceAvailableToSpend() const { abort(); }
float CGameScriptInterface::GetHeroFatness() const { abort(); }
float CGameScriptInterface::GetHeroScariness() const { abort(); }
float CGameScriptInterface::GetHeroAttractiveness() const { abort(); }
float CGameScriptInterface::GetHeroWillEnergyLevel() const { abort(); }
void CGameScriptInterface::SetHeroWillEnergyLevel(float) const { abort(); }
void CGameScriptInterface::SetHeroWillEnergyAsAbleToRefill(bool) const { abort(); }
long CGameScriptInterface::GetNumberOfItemsOfTypeInInventory(const CCharString&) const { abort(); }
bool CGameScriptInterface::IsObjectInThingsPossession(const CCharString&, const CScriptThing&) const { abort(); }
bool CGameScriptInterface::IsHeroHandLampLit() const { abort(); }
void CGameScriptInterface::SetHeroHandLampAsLit(bool) const { abort(); }
bool CGameScriptInterface::IsWearingClothingItem(const CScriptThing&, const CCharString&) const { abort(); }
bool CGameScriptInterface::IsHeroNaked() const { abort(); }
void CGameScriptInterface::RemoveHeroClothing() const { abort(); }
void CGameScriptInterface::SetHeroAsWearing(const CCharString&) const { abort(); }
void CGameScriptInterface::ChangeHeroHairstyle(const CCharString&) const { abort(); }
void CGameScriptInterface::RemoveHeroHairstyle() const { abort(); }
bool CGameScriptInterface::IsWearingHairstyle(CScriptThing&, const CCharString&) const { abort(); }
bool CGameScriptInterface::IsPlayerCarryingItemOfType(const CCharString&) const { abort(); }
bool CGameScriptInterface::IsPlayerWieldingWeapon() const { abort(); }
bool CGameScriptInterface::IsEntityWieldingWeapon(const CScriptThing&) const { abort(); }
bool CGameScriptInterface::IsEntityWieldingMeleeWeapon(const CScriptThing&) const { abort(); }
bool CGameScriptInterface::IsEntityWieldingRangedWeapon(const CScriptThing&) const { abort(); }
bool CGameScriptInterface::GetPreviouslyWieldedMeleeWeaponName(CCharString&) const { abort(); }
bool CGameScriptInterface::GetPreviouslyWieldedRangedWeaponName(CCharString&) const { abort(); }
bool CGameScriptInterface::IsEntityAbleToAttack(const CScriptThing&) const { abort(); }
CScriptThing CGameScriptInterface::EntityGetThingInPrimarySlot(const CScriptThing&) const { abort(); }
void CGameScriptInterface::ApplyHeroPenaltyForDeath() const { abort(); }
void CGameScriptInterface::GiveHeroTitle(const CCharString&) const { abort(); }
EHeroTitle CGameScriptInterface::GetHeroTitle() const { abort(); }
void CGameScriptInterface::EntitySetAsMarryable(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::EntitySetAsAbleToRegionFollowWhenMarried(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::EntityForceMarriageToHero(const CScriptThing&, bool) const { abort(); }
bool CGameScriptInterface::IsEntityMarriedToHero(const CScriptThing&) const { abort(); }
bool CGameScriptInterface::IsEntityMarriable(const CScriptThing&) const { abort(); }
bool CGameScriptInterface::GetHeroHasMarried() const { abort(); }
bool CGameScriptInterface::GetHeroHasCurrentMarriage() const { abort(); }
bool CGameScriptInterface::GetHeroHasDivorcedMarriage() const { abort(); }
bool CGameScriptInterface::GetHeroHasChildren() const { abort(); }
bool CGameScriptInterface::GetHeroHasMurderedWife() const { abort(); }
bool CGameScriptInterface::IsHeroChild() const { abort(); }
void CGameScriptInterface::CancelHeroTeleportEffects() const { abort(); }
long CGameScriptInterface::GetNumberOfTimesHeroHasHadSex() const { abort(); }
void CGameScriptInterface::SetNumberOfTimesHeroHasHadSex(long) const { abort(); }
void CGameScriptInterface::SetHeroAsHavingHadSex(bool) const { abort(); }
void CGameScriptInterface::SetHeroAsHavingHadGaySex(bool) const { abort(); }
void CGameScriptInterface::GiveThingHeroRewardItem(const CScriptThing&, const CCharString&, const CCharString&) const { abort(); }
void CGameScriptInterface::GiveThingItemInHand(const CScriptThing&, const CCharString&, bool) const { abort(); }
void CGameScriptInterface::GiveThingItemInSlot(const CScriptThing&, const CCharString&, const CCharString&) const { abort(); }
void CGameScriptInterface::GiveHeroExpression(const CCharString&, long, bool) const { abort(); }
bool CGameScriptInterface::HeroHasExpression(const CCharString&) const { abort(); }
bool CGameScriptInterface::IsHeroPerformingExpression(const CCharString&) const { abort(); }
bool CGameScriptInterface::IsDPadButtonHeldForExpression(const CCharString&) const { abort(); }
void CGameScriptInterface::EntityFollowThing(const CScriptThing&, const CScriptThing&, float, bool) const { abort(); }
void CGameScriptInterface::EntityStopFollowing(const CScriptThing&) const { abort(); }
bool CGameScriptInterface::GetFollowingEntityList(const CScriptThing&, std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { abort(); }
bool CGameScriptInterface::GetHeroSummonedCreaturesList(std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { abort(); }
bool CGameScriptInterface::GetPerceivingHeroEntityList(std::vector<CScriptThing,std::allocator<CScriptThing> >&) const { abort(); }
bool CGameScriptInterface::IsEntityFollowingHero(const CScriptThing&) const { abort(); }
void CGameScriptInterface::EntitySetAsAllowedToFollowHero(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::EntitySetAsAllowedToChangeRegionFollowingState(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::EntitySetAsRespondingToFollowAndWaitExpressions(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::EntitySetAsMirroringHeroEnemyRelationsWhileFollowing(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::TeleportAllFollowersToHeroPosition() const { abort(); }
void CGameScriptInterface::EntityTeleportToHeroPosition(const CScriptThing&) const { abort(); }
void CGameScriptInterface::SendEntityEvent(NEntityEvents::EEventType, const CScriptThing&, const CScriptThing&, CThing*) const { abort(); }
float CGameScriptInterface::GetWaterHeightAtPosition(const C3DVector&) const { abort(); }
bool CGameScriptInterface::IsFishingSpotEnabled(const CScriptThing&) const { abort(); }
void CGameScriptInterface::DisableFishingSpot(const CScriptThing&) const { abort(); }
void CGameScriptInterface::UpdateFishWeight(const CScriptThing&) const { abort(); }
float CGameScriptInterface::GetBestFishWeight(const CCharString&) const { abort(); }
void CGameScriptInterface::HeroGoFishing(bool) const { abort(); }
long CGameScriptInterface::GetHeroFishingLevel() const { abort(); }
void CGameScriptInterface::HeroGoDigging() const { abort(); }
void CGameScriptInterface::HeroStopDigging() const { abort(); }
void CGameScriptInterface::HeroPlayOracleMinigame() const { abort(); }
bool CGameScriptInterface::IsHeroPlayingOracleMinigame() const { abort(); }
bool CGameScriptInterface::HasHeroWonOracleMinigame() const { abort(); }
void CGameScriptInterface::HeroPlayFireheartMinigame() const { abort(); }
void CGameScriptInterface::HeroQuitFireheartMinigame() const { abort(); }
bool CGameScriptInterface::HasHeroForceQuitFireheartMinigame() const { abort(); }
float CGameScriptInterface::GetHeroHealth() const { abort(); }
float CGameScriptInterface::GetHeroHealthMax() const { abort(); }
float CGameScriptInterface::GetHeroHealthPercentage() const { abort(); }
long CGameScriptInterface::GetHeroWillEnergy() const { abort(); }
long CGameScriptInterface::GetHeroWillEnergyMax() const { abort(); }
long CGameScriptInterface::GetHeroWillEnergyPercentage() const { abort(); }
void CGameScriptInterface::ChangeHeroHealthBy(float, bool, bool) const { abort(); }
float CGameScriptInterface::GetHealth(const CScriptThing&) const { abort(); }
void CGameScriptInterface::ModifyThingHealth(const CScriptThing&, float, bool) const { abort(); }
void CGameScriptInterface::EntitySetMaxHealth(const CScriptThing&, float, bool) const { abort(); }
void CGameScriptInterface::SetThingAsKilled(const CScriptThing&) const { abort(); }
void CGameScriptInterface::GiveHeroNewQuestObjective(const CCharString&, unsigned long) const { abort(); }
void CGameScriptInterface::TellHeroQuestObjectiveCompleted(unsigned long) const { abort(); }
void CGameScriptInterface::TellHeroQuestObjectiveFailed(unsigned long) const { abort(); }
void CGameScriptInterface::AddQuestRegion(const CCharString&, const CCharString&) const { abort(); }
void CGameScriptInterface::SetQuestWorldMapOffset(const CCharString&, const C2DCoordI&) const { abort(); }
bool CGameScriptInterface::IsHeroOnQuest() const { abort(); }
void CGameScriptInterface::HeroReceiveMessageFromGuildMaster(const CCharString&, const CCharString&, bool, bool) const { abort(); }
void CGameScriptInterface::SetGuildMasterMessages(bool) const { abort(); }
void CGameScriptInterface::ActivateQuest(const CCharString&) const { abort(); }
void CGameScriptInterface::ActivateMultipleQuests(const CArray<CCharString>&) const { abort(); }
void CGameScriptInterface::ActivateQuestWithoutLoadingResources(const CCharString&) const { abort(); }
void CGameScriptInterface::ActivateMultipleQuestsWithoutLoadingResources(const CArray<CCharString>&) const { abort(); }
void CGameScriptInterface::DeactivateQuest(const CCharString&, unsigned long) const { abort(); }
void CGameScriptInterface::DeactivateQuestLater(const CCharString&, unsigned long) const { abort(); }
void CGameScriptInterface::PrepareQuestsWhenFinalQuestIsActivated(const CCharString&) const { abort(); }
void CGameScriptInterface::PrepareQuestsWhenFinalQuestIsCompleted() const { abort(); }
bool CGameScriptInterface::IsQuestActive(const CCharString&) const { abort(); }
bool CGameScriptInterface::IsQuestRegistered(const CCharString&) const { abort(); }
bool CGameScriptInterface::IsQuestCompleted(const CCharString&) const { abort(); }
bool CGameScriptInterface::IsQuestFailed(const CCharString&) const { abort(); }
void CGameScriptInterface::SetQuestAsCompleted(const CCharString&, bool, bool, bool) const { abort(); }
void CGameScriptInterface::SetQuestAsFailed(const CCharString&, bool, const CWideString&, bool) const { abort(); }
void CGameScriptInterface::SetQuestAsPersistent(const CCharString&, bool) const { abort(); }
const CCharString& CGameScriptInterface::GetExclusiveQuestScriptName() const { abort(); }
void CGameScriptInterface::AddQuestCard(const CCharString&, const CCharString&, bool, bool) const { abort(); }
void CGameScriptInterface::RemoveQuestCardFromGuild(const CCharString&) const { abort(); }
void CGameScriptInterface::RemoveQuestCardFromHero(const CCharString&) const { abort(); }
void CGameScriptInterface::GiveHeroQuestCardDirectly(const CCharString&, const CCharString&, bool) const { abort(); }
void CGameScriptInterface::SetQuestCardObjective(const CCharString&, const CCharString&, const CCharString&, const CCharString&) const { abort(); }
void CGameScriptInterface::SetQuestCardGoldReward(const CCharString&, long) const { abort(); }
void CGameScriptInterface::SetQuestCardRenownReward(const CCharString&, long) const { abort(); }
void CGameScriptInterface::RemoveAllAvailableQuestCardsFromGuild() const { abort(); }
void CGameScriptInterface::FailAllActiveQuests() const { abort(); }
void CGameScriptInterface::GetAllActiveQuestInfo(std::vector<CCharString,std::allocator<CCharString> >&, std::vector<CCharString,std::allocator<CCharString> >&) const { abort(); }
void CGameScriptInterface::AddFeatCard(long, const CCharString&, const CCharString&) const { abort(); }
void CGameScriptInterface::AddBoast(const CCharString&, long, long, long, bool, const CCharString&, long) const { abort(); }
void CGameScriptInterface::RemoveBoast(long, const CCharString&) const { abort(); }
void CGameScriptInterface::SetBoastAsFailed(long, const CCharString&) const { abort(); }
void CGameScriptInterface::SetBoastAsCompleted(long, const CCharString&) const { abort(); }
bool CGameScriptInterface::IsBoastTaken(long, const CCharString&) const { abort(); }
void CGameScriptInterface::AddLogBookEntry(const CWideString&, const CWideString&, const CWideString&, long) const { abort(); }
void CGameScriptInterface::KickOffQuestStartScreen(const CCharString&, bool, bool) const { abort(); }
void CGameScriptInterface::KickOffDeathScreen() const { abort(); }
void CGameScriptInterface::KickOffCreditsScreen(CCharString&) const { abort(); }
void CGameScriptInterface::SetPreferredQuickAccessItem(const CCharString&, long, long) const { abort(); }
CCharString CGameScriptInterface::GetDeathRecoveryMarkerName() const { abort(); }
void CGameScriptInterface::SetDeathRecoveryMarkerName(const CCharString&) const { abort(); }
void CGameScriptInterface::ResetDeathRecoveryMarkerNameToDefault() const { abort(); }
bool CGameScriptInterface::IsToFailQuestOnDeath() const { abort(); }
void CGameScriptInterface::SetWhetherToFailQuestOnDeath(bool) const { abort(); }
void CGameScriptInterface::ResetWhetherToFailQuestOnDeathToDefault() const { abort(); }
CScriptThing CGameScriptInterface::GetMostRecentValidUsedTarget() const { abort(); }
CCharString CGameScriptInterface::GetMostRecentValidUsedTargetName() const { abort(); }
void CGameScriptInterface::DisplayQuestInfo(bool) const { abort(); }
void CGameScriptInterface::SetQuestInfoName(const char*) const { abort(); }
void CGameScriptInterface::SetQuestInfoText(const char*) const { abort(); }
long CGameScriptInterface::AddQuestInfoBar(float, float, const CRGBColour&, const CRGBColour&, const CCharString&, const CCharString&, float) const { abort(); }
long CGameScriptInterface::AddQuestInfoBarHealth(const CScriptThing&, const CRGBColour&, const CCharString&, float) const { abort(); }
long CGameScriptInterface::AddQuestInfoTimer(const CTimer&, const CCharString&, float) const { abort(); }
long CGameScriptInterface::AddQuestInfoCounter(const CCounter&, const CCharString&, float) const { abort(); }
long CGameScriptInterface::AddQuestInfoCounter(const CCharString&, long, float) const { abort(); }
long CGameScriptInterface::AddQuestInfoCounterList(const CCharString&, long, float) const { abort(); }
long CGameScriptInterface::AddQuestInfoTick(const CCharString&, bool, float) const { abort(); }
long CGameScriptInterface::AddQuestInfoTick(EGameAction, bool, float) const { abort(); }
void CGameScriptInterface::UpdateQuestInfoBar(long, float, float, float) const { abort(); }
void CGameScriptInterface::ChangeQuestInfoBarColour(long, const CRGBColour&, const CRGBColour&) const { abort(); }
void CGameScriptInterface::UpdateQuestInfoTimer(long, float) const { abort(); }
void CGameScriptInterface::UpdateQuestInfoCounter(long, long, long) const { abort(); }
void CGameScriptInterface::UpdateQuestInfoCounterList(long, long, long) const { abort(); }
void CGameScriptInterface::UpdateQuestInfoTick(long, bool) const { abort(); }
void CGameScriptInterface::RemoveQuestInfoElement(long) const { abort(); }
void CGameScriptInterface::RemoveAllQuestInfoElements() const { abort(); }
void CGameScriptInterface::DisplayTime(bool) const { abort(); }
void CGameScriptInterface::DisplayMoneyBag(bool) const { abort(); }
void CGameScriptInterface::DisplayMiniGameInfo(bool, EMiniGameType) const { abort(); }
void CGameScriptInterface::UpdateMiniGameInfoBar(float) const { abort(); }
bool CGameScriptInterface::IsEntityPickPocketable(const CScriptThing&) const { abort(); }
bool CGameScriptInterface::IsEntityPickLockable(const CScriptThing&) const { abort(); }
bool CGameScriptInterface::IsEntityStealable(const CScriptThing&) const { abort(); }
void CGameScriptInterface::EntitySetAsPickPocketed(const CScriptThing&) const { abort(); }
void CGameScriptInterface::EntitySetAsPickLocked(const CScriptThing&) const { abort(); }
void CGameScriptInterface::EntitySetAsStolen(const CScriptThing&) const { abort(); }
void CGameScriptInterface::MiniMapAddMarker(const CScriptThing&, const CCharString&) const { abort(); }
void CGameScriptInterface::MiniMapSetMarkerGraphic(const CScriptThing&, const CCharString&) const { abort(); }
void CGameScriptInterface::MiniMapRemoveMarker(const CScriptThing&) const { abort(); }
void CGameScriptInterface::MiniMapRemoveAllMarkers() const { abort(); }
void CGameScriptInterface::MiniMapAllowRouteBetweenRegions(const CCharString&, const CCharString&, bool) const { abort(); }
void CGameScriptInterface::MiniMapSetAsEnabled(bool) const { abort(); }
void CGameScriptInterface::EntitySetAsHiddenOnMiniMap(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::SetHUDEnabled(bool) const { abort(); }
bool CGameScriptInterface::TextEntryExists(const CCharString&) const { abort(); }
CCharString CGameScriptInterface::GetValidTextEntryNameWithAttitude(const CScriptThing&, const CCharString&) const { abort(); }
void CGameScriptInterface::SetThingHasInformation(const CScriptThing&, bool, bool, bool) const { abort(); }
void CGameScriptInterface::ClearThingHasInformation(const CScriptThing&) const { abort(); }
void CGameScriptInterface::EntitySetWillBeUsingNarrator(const CScriptThing&, const CCharString&) const { abort(); }
void CGameScriptInterface::EntityResetAsPureAINarrator(const CScriptThing&) const { abort(); }
long CGameScriptInterface::AddNewConversation(const CScriptThing&, bool, bool) const { abort(); }
void CGameScriptInterface::AddPersonToConversation(long, const CScriptThing&) const { abort(); }
void CGameScriptInterface::AddLineToConversation(long, const CCharString&, bool, const CScriptThing&, const CScriptThing&) const { abort(); }
void CGameScriptInterface::RemoveConversation(long, bool) const { abort(); }
bool CGameScriptInterface::IsConversationActive(long) const { abort(); }
void CGameScriptInterface::PlayAVIMovie(const CCharString&) const { abort(); }
void CGameScriptInterface::StartMovieSequence(const CCharString&, CScriptGameResourceObjectMovieBase&) { abort(); }
void CGameScriptInterface::FixMovieSequenceCamera(bool) const { abort(); }
void CGameScriptInterface::FadeScreenOutUntilNextCallToFadeScreenIn(float, float) { abort(); }
bool CGameScriptInterface::FadeScreenOut(float, float, CRGBColour) { abort(); }
void CGameScriptInterface::FadeScreenIn() { abort(); }
bool CGameScriptInterface::IsScreenFadingOut() const { abort(); }
void CGameScriptInterface::Pause(float) const { abort(); }
void CGameScriptInterface::EndCutFade() { abort(); }
void CGameScriptInterface::EndLetterBox() { abort(); }
void CGameScriptInterface::PauseAllNonScriptedEntities(bool) { abort(); }
void CGameScriptInterface::PauseAllEntities(bool) { abort(); }
void CGameScriptInterface::SetAllowScreenFadingOnNextRegionChange(bool) { abort(); }
void CGameScriptInterface::SetAllowScreenFadingIfAlreadyFaded(bool) { abort(); }
void CGameScriptInterface::SetToKeepHeroAbilitiesDuringCutscenes(bool) const { abort(); }
void CGameScriptInterface::SetToDisplayTutorialsDuringCutscenes(bool) const { abort(); }
void CGameScriptInterface::SetCutsceneMode(bool, bool) const { abort(); }
bool CGameScriptInterface::IsInCutscene() const { abort(); }
void CGameScriptInterface::SetCutsceneSkippable(bool) const { abort(); }
void CGameScriptInterface::SetCutsceneSkippableWhilePaused(bool) const { abort(); }
void CGameScriptInterface::SetCutsceneActionMode(bool, const CCharString&) const { abort(); }
void CGameScriptInterface::PreloadNewScene(float) const { abort(); }
void CGameScriptInterface::StartProgressDisplay() const { abort(); }
void CGameScriptInterface::StopProgressDisplay() const { abort(); }
void CGameScriptInterface::SetScreenMessagesEnabled(bool) const { abort(); }
bool CGameScriptInterface::IsHeroControlledByPlayer() const { abort(); }
bool CGameScriptInterface::IsInMovieSequence() const { abort(); }
void CGameScriptInterface::CancelAbilitiesForCutscene() const { abort(); }
void CGameScriptInterface::ResumeAbilitiesForCutscene() const { abort(); }
void CGameScriptInterface::CancelUsingAbility(EHeroAbility) const { abort(); }
void CGameScriptInterface::SetAbilityAvailability(EHeroAbility, bool) const { abort(); }
void CGameScriptInterface::SetEnvironmentalEffectsAlwaysUpdate(bool) const { abort(); }
void CGameScriptInterface::SetDeadCreaturesAndExperienceOrbsAndDropBagsAsHidden(bool) const { abort(); }
void CGameScriptInterface::RemoveDeadCreature(const CCharString&) const { abort(); }
void CGameScriptInterface::CameraSetCameraPreloadFlag(bool) const { abort(); }
void CGameScriptInterface::CameraCircleAroundThing(const CScriptThing&, const C3DVector&, float) const { abort(); }
void CGameScriptInterface::CameraCircleAroundPos(const C3DVector&, const C3DVector&, float) const { abort(); }
void CGameScriptInterface::CameraMoveToPosAndLookAtPos(const C3DVector&, const C3DVector&, float) const { abort(); }
void CGameScriptInterface::CameraMoveToPosAndLookAtThing(const C3DVector&, const CScriptThing&, float) const { abort(); }
void CGameScriptInterface::CameraMoveBetweenLookingAt(const C3DVector&, const C3DVector&, const C3DVector&, float, float) const { abort(); }
void CGameScriptInterface::CameraMoveBetweenLookingAt(const C3DVector&, const C3DVector&, const CScriptThing&, float, float) const { abort(); }
void CGameScriptInterface::CameraMoveBetweenLookFromAndLookTo(const C3DVector&, const C3DVector&, const C3DVector&, const C3DVector&, float) const { abort(); }
void CGameScriptInterface::CameraUseCameraPoint(const CCharString&, const CScriptThing&, float, long, long) const { abort(); }
void CGameScriptInterface::CameraUseCameraPoint(const CScriptThing&, const CScriptThing&, float, long, long) const { abort(); }
void CGameScriptInterface::CameraUseCameraPoint(const CCharString&, const C3DVector*, const CRightHandedSet*, float, long, long) const { abort(); }
void CGameScriptInterface::CameraUseCameraPoint(const CScriptThing&, const C3DVector*, const CRightHandedSet*, float, long, long) const { abort(); }
void CGameScriptInterface::CameraDoConversation(const CScriptThing&, const CScriptThing&, ECameraOp, bool) const { abort(); }
void CGameScriptInterface::CameraDefault() { abort(); }
void CGameScriptInterface::CameraResetToViewBehindHero(float) { abort(); }
bool CGameScriptInterface::IsCameraInScriptedMode() const { abort(); }
void CGameScriptInterface::CameraUseScreenEffect(float, float, float) const { abort(); }
void CGameScriptInterface::CameraCancelScreenEffect() const { abort(); }
bool CGameScriptInterface::IsCameraPosOnScreen(const C3DVector&) const { abort(); }
float CGameScriptInterface::GetGameAngleXY() const { abort(); }
void CGameScriptInterface::CameraEarthquakeIntensityAtPos(const C3DVector&, float, float) const { abort(); }
void CGameScriptInterface::CameraShake(float, float) const { abort(); }
void CGameScriptInterface::OpenDoor(const CScriptThing&) const { abort(); }
void CGameScriptInterface::CloseDoor(const CScriptThing&) const { abort(); }
void CGameScriptInterface::OpenHouseDoors(const CScriptThing&) const { abort(); }
void CGameScriptInterface::CloseHouseDoors(const CScriptThing&) const { abort(); }
void CGameScriptInterface::JamDoor(const CScriptThing&) const { abort(); }
void CGameScriptInterface::SetDoorTriggerType(const CScriptThing&, EDoorTriggerType) const { abort(); }
void CGameScriptInterface::OverrideAutomaticHouseLocking(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::SetHouseOwnedByPlayer(const CScriptThing&, bool, bool) const { abort(); }
void CGameScriptInterface::SetBuyableHouseAsScripted(const CScriptThing&, bool) const { abort(); }
bool CGameScriptInterface::IsChestOpen(const CScriptThing&) const { abort(); }
bool CGameScriptInterface::OpenChest(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::CloseChest(const CScriptThing&) const { abort(); }
long CGameScriptInterface::GetNumberOfKeysNeededToUnlockChest(const CScriptThing&, CCharString&) const { abort(); }
void CGameScriptInterface::DisplayLockedChestMessage(const CScriptThing&) const { abort(); }
void CGameScriptInterface::SetTrophyAsMountable(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::SetVillageLimbo(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::SetCreatureNotReload(const CScriptThing&) const { abort(); }
bool CGameScriptInterface::IsSleepingTime(const CScriptThing&) const { abort(); }
void CGameScriptInterface::EnableGuards(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::EnableVillagerDefTypes(const CScriptThing&, bool, const CCharString&) const { abort(); }
CScriptThing CGameScriptInterface::TryToRespawnDefNamed(const CScriptThing&, const CCharString&, const C3DVector&) const { abort(); }
void CGameScriptInterface::ClearHeroEnemyOfGuards(const CScriptThing&) const { abort(); }
void CGameScriptInterface::SetThingAsUsable(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::SetThingHomeBuilding(const CScriptThing&, const CScriptThing&) const { abort(); }
void CGameScriptInterface::SetVillageAttitude(const CScriptThing&, EScriptVillageAttitude) const { abort(); }
void CGameScriptInterface::AddCrimeCommitted(const CScriptThing&, ECrime, bool, const CScriptThing*, const CScriptThing*, EOpinionPostDeedType) const { abort(); }
void CGameScriptInterface::GiveThingBestEnemyTarget(const CScriptThing&, const CScriptThing&) const { abort(); }
void CGameScriptInterface::ClearThingBestEnemyTarget(const CScriptThing&) const { abort(); }
void CGameScriptInterface::EntitySetInLimbo(const CScriptThing&, bool, bool) const { abort(); }
bool CGameScriptInterface::IsEntityInLimbo(const CScriptThing&) const { abort(); }
bool CGameScriptInterface::EntityGetShotStrikePos(const CScriptThing&, C3DVector&) const { abort(); }
void CGameScriptInterface::EntitySetNegateAllHits(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::EntitySetEvadeAllHits(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::EntitySetAbleToBeEngagedInCombat(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::EntitySetAlwaysBlockAttacksFromThing(const CScriptThing&, const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::EntitySetAttackThingImmediately(const CScriptThing&, const CScriptThing&, bool, bool) const { abort(); }
void CGameScriptInterface::EntitySetCombatType(const CScriptThing&, const CCharString&) const { abort(); }
void CGameScriptInterface::EntityResetCombatTypeToDefault(const CScriptThing&) const { abort(); }
void CGameScriptInterface::EntitySetMaxNumberOfAttackers(const CScriptThing&, long) const { abort(); }
void CGameScriptInterface::EntityClearMaxNumberOfAttackers(const CScriptThing&) const { abort(); }
void CGameScriptInterface::EntityAttachToScript(const CScriptThing&, const CCharString&) const { abort(); }
void CGameScriptInterface::EntitySetCombatAbility(const CScriptThing&, float) const { abort(); }
void CGameScriptInterface::EntitySetRangedTarget(const CScriptThing&, const CScriptThing&) const { abort(); }
void CGameScriptInterface::EntityClearRangedTarget(const CScriptThing&) const { abort(); }
void CGameScriptInterface::EntitySetTargetable(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::EntitySetTargetingType(const CScriptThing&, ETargetingType) const { abort(); }
void CGameScriptInterface::EntitySetTargetingValidTargetWithoutLOS(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::EntityTeleportToThing(const CScriptThing&, const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::EntityTeleportToPosition(const CScriptThing&, const C3DVector&, float, bool, bool) const { abort(); }
void CGameScriptInterface::EntitySetFacingAngle(const CScriptThing&, float, bool) const { abort(); }
void CGameScriptInterface::EntitySetFacingAngleTowardsThing(const CScriptThing&, const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::EntitySetPerceptionVariables(const CScriptThing&, float, float, float) const { abort(); }
void CGameScriptInterface::SetThingPersistent(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::EntitySetThingAsWantingMoney(const CScriptThing&, long) const { abort(); }
void CGameScriptInterface::EntitySetAppearanceMorphSeed(const CScriptThing&, long) const { abort(); }
void CGameScriptInterface::SetEntityAsRegionFollowing(const CScriptThing&, const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::SetEntityAsFollowingHeroThroughTeleporters(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::EntitySetAppearanceSeed(const CScriptThing&, unsigned long) const { abort(); }
void CGameScriptInterface::EntityGetAppearanceSeed(const CScriptThing&, unsigned long&) const { abort(); }
void CGameScriptInterface::EntitySetAsForSale(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::EntitySetStockItemPrice(const CScriptThing&, long) const { abort(); }
long CGameScriptInterface::EntityGetStockItemPrice(const CScriptThing&) const { abort(); }
void CGameScriptInterface::EntityPlayObjectAnimation(const CScriptThing&, const CCharString&, bool) const { abort(); }
void CGameScriptInterface::EntitySetMaxRunningSpeed(const CScriptThing&, float) const { abort(); }
void CGameScriptInterface::EntitySetMaxWalkingSpeed(const CScriptThing&, float) const { abort(); }
void CGameScriptInterface::EntityResetMaxRunningSpeed(const CScriptThing&) const { abort(); }
void CGameScriptInterface::EntityResetMaxWalkingSpeed(const CScriptThing&) const { abort(); }
void CGameScriptInterface::EntityAttachToVillage(const CScriptThing&, const CScriptThing&) const { abort(); }
void CGameScriptInterface::EntitySetAsSittingOnFloor(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::EntitySetAsHavingBoundHands(CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::EntitySetAsRemoveAllMovementBlockingModes(CScriptThing&) const { abort(); }
void CGameScriptInterface::EntitySetAsScared(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::EntitySetAsDrunk(CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::EntityForceToLookAtThing(const CScriptThing&, const CScriptThing&) const { abort(); }
void CGameScriptInterface::EntityForceToLookAtCamera(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::EntityForceToLookAtNothing(const CScriptThing&) const { abort(); }
void CGameScriptInterface::EntityResetForceToLookAt(const CScriptThing&) const { abort(); }
void CGameScriptInterface::EntitySetShotAccuracyPercentage(const CScriptThing&, long) const { abort(); }
CScriptThing CGameScriptInterface::EntityGetStandingOnThing(const CScriptThing&) const { abort(); }
CScriptThing CGameScriptInterface::EntityGetStandingInsideBuilding(const CScriptThing&) const { abort(); }
void CGameScriptInterface::EntityDropGenericBox(const CScriptThing&) const { abort(); }
void CGameScriptInterface::EntitySheatheWeapons(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::EntityUnsheatheWeapons(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::EntityUnsheatheMeleeWeapon(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::EntityUnsheatheRangedWeapon(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::EntitySetAlpha(const CScriptThing&, float, bool) const { abort(); }
void CGameScriptInterface::EntitySetAsDrawable(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::EntitySetCutsceneBehaviour(const CScriptThing&, ECutsceneBehaviour) const { abort(); }
ESex CGameScriptInterface::EntityGetSex(const CScriptThing&) const { abort(); }
void CGameScriptInterface::EntitySetAsAbleToWalkThroughSolidObjects(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::EntitySetAsRespondToHit(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::EntitySetAsDamageable(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::EntitySetAsKillable(const CScriptThing&, bool, bool) const { abort(); }
void CGameScriptInterface::EntitySetAsLocked(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::EntityDecapitate(const CScriptThing&) const { abort(); }
void CGameScriptInterface::EntityGiveGold(const CScriptThing&, long) const { abort(); }
void CGameScriptInterface::EntitySetAllowBossPhaseChanges(const CScriptThing&, bool) const { abort(); }
long CGameScriptInterface::EntityGetBossPhase(const CScriptThing&) const { abort(); }
void CGameScriptInterface::EntitySetBossPhase(const CScriptThing&, long) const { abort(); }
void CGameScriptInterface::EntityResetCreatureMode(const CScriptThing&) const { abort(); }
bool CGameScriptInterface::EntitySetAsReceivingEvents(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::EntitySetAsToAddToComboMultiplierWhenHit(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::EntitySetAsToAddToStatChangesWhenHit(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::EntityLeaveCombatStance(const CScriptThing&) const { abort(); }
void CGameScriptInterface::EntitySetAsUseMovementInActions(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::EntitySetAsDisplayingEmoteIcon(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::EntitySetAsCollidableToThings(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::EntityEnableGravity(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::EntitySetLightAsOn(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::EntityFadeOut(const CScriptThing&, float) const { abort(); }
void CGameScriptInterface::EntityFadeIn(const CScriptThing&, float) const { abort(); }
void CGameScriptInterface::EntityBeginLoadingAnimation(const CScriptThing&, const CCharString&) const { abort(); }
void CGameScriptInterface::EntityBeginLoadingBasicAnimations(const CScriptThing&) const { abort(); }
bool CGameScriptInterface::EntityCastForcePush(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::EntityCastLightningAtTarget(const CScriptThing&, const CScriptThing&) const { abort(); }
void CGameScriptInterface::BeginLoadingMesh(const CCharString&) const { abort(); }
bool CGameScriptInterface::EntityWillTeleportToArea(const CScriptThing&, C3DVector, float, float) const { abort(); }
void CGameScriptInterface::EntityStartScreamerSuperAttackThing(const CScriptThing&, const CScriptThing&) const { abort(); }
void CGameScriptInterface::EntityEndScreamerSuperAttackThing(const CScriptThing&) const { abort(); }
void CGameScriptInterface::SetHeroGuideToShowQuestCardsWhenSpokenTo(bool) { abort(); }
void CGameScriptInterface::SetLightColour(const CScriptThing&, const CRGBColour&) const { abort(); }
void CGameScriptInterface::CreatureGeneratorSetFamily(const CScriptThing&, const CCharString&) const { abort(); }
void CGameScriptInterface::CreatureGeneratorTrigger(const CScriptThing&) const { abort(); }
void CGameScriptInterface::CreatureGeneratorSetAlwaysCreateCreaturesOnTrigger(const CScriptThing&, bool) const { abort(); }
bool CGameScriptInterface::CreatureGeneratorIsDepleted(const CScriptThing&) const { abort(); }
bool CGameScriptInterface::CreatureGeneratorIsDestroyed(const CScriptThing&) const { abort(); }
void CGameScriptInterface::CreatureGeneratorSetGeneratedCreatureScriptName(const CScriptThing&, const CCharString&) const { abort(); }
void CGameScriptInterface::CreatureGeneratorSetNumTriggers(const CScriptThing&, long) const { abort(); }
long CGameScriptInterface::CreatureGeneratorGetNumGeneratedCreatures(const CScriptThing&) const { abort(); }
bool CGameScriptInterface::CreatureGeneratorAreAllCreaturesAlive(const CScriptThing&) const { abort(); }
void CGameScriptInterface::CreatureGeneratorAddTriggerer(const CScriptThing&, const CScriptThing&) const { abort(); }
void CGameScriptInterface::CreatureGeneratorRemoveTriggerer(const CScriptThing&, const CScriptThing&) const { abort(); }
void CGameScriptInterface::SetCreatureGeneratorEnabled(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::SetCreatureGeneratorsEnabled(const CCharString&, bool) const { abort(); }
void CGameScriptInterface::SetCreatureGeneratorsEnabledDuringScript(const CCharString&, bool) const { abort(); }
void CGameScriptInterface::SetCreatureGeneratorsCreatureGroupAsEnabled(ECreatureGroup, bool) const { abort(); }
bool CGameScriptInterface::IsCreatureGenerationEnabledForRegion(const CCharString&) const { abort(); }
bool CGameScriptInterface::IsCreatureFlying(const CScriptThing&) const { abort(); }
void CGameScriptInterface::SetTeleporterAsActive(const CScriptThing&, bool) const { abort(); }
bool CGameScriptInterface::IsTeleporterActive(const CScriptThing&) const { abort(); }
void CGameScriptInterface::SetTeleportingAsActive(bool) const { abort(); }
bool CGameScriptInterface::IsTeleportingActive() const { abort(); }
void CGameScriptInterface::SetRegionExitAsActive(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::SetRegionEntranceAsActive(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::SetRegionTextDisplayAsActive(bool) const { abort(); }
void CGameScriptInterface::SetHeroSleepingAsEnabled(bool) const { abort(); }
bool CGameScriptInterface::IsHeroSleepingEnabled() const { abort(); }
void CGameScriptInterface::SetExperienceSpendingAsEnabled(bool) const { abort(); }
void CGameScriptInterface::SetMoralityChangingAsEnabled(bool) const { abort(); }
void CGameScriptInterface::SetSummonerDeathExplosionAffectsHero(bool) const { abort(); }
CScriptThing CGameScriptInterface::GetNearestEnabledDiggingSpot(const CScriptThing&) const { abort(); }
bool CGameScriptInterface::IsDiggingSpotEnabled(const CScriptThing&) const { abort(); }
bool CGameScriptInterface::IsDiggingSpotHidden(const CScriptThing&) const { abort(); }
void CGameScriptInterface::SetDiggingSpotAsHidden(const CScriptThing&, bool) const { abort(); }
bool CGameScriptInterface::CheckForCameraMessage(const CCharString&) const { abort(); }
void CGameScriptInterface::WaitForCameraMessage(const CCharString&) const { abort(); }
void CGameScriptInterface::SetThingAsConscious(const CScriptThing&, bool, const CCharString&) const { abort(); }
void CGameScriptInterface::SetFireToThing(const CScriptThing&) const { abort(); }
void CGameScriptInterface::ExtinguishFiresOnThing(const CScriptThing&) const { abort(); }
bool CGameScriptInterface::IsThingOnFire(const CScriptThing&) const { abort(); }
void CGameScriptInterface::AddItemToContainer(const CScriptThing&, const CCharString&) const { abort(); }
void CGameScriptInterface::RemoveItemFromContainer(const CScriptThing&, const CCharString&) const { abort(); }
void CGameScriptInterface::EntitySetDeathContainerAsEnabled(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::GetItemDefNamesFromContainer(const CScriptThing&, std::vector<CCharString,std::allocator<CCharString> >&) const { abort(); }
void CGameScriptInterface::SetCreatureBrain(const CScriptThing&, const CCharString&) const { abort(); }
void CGameScriptInterface::EntitySetStategroupEnabled(const CScriptThing&, const CCharString&, bool) const { abort(); }
void CGameScriptInterface::EntitySetAllStategroupsEnabled(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::EntitySetCombatEnabled(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::EntitySetSleepEnabled(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::EntitySetOpinionReactionsEnabled(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::EntitySetDeedReactionsEnabled(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::DebugGetAllTextEntriesForTargetedThing(std::set<unsigned long,std::less<unsigned long>,std::allocator<unsigned long> >&) const { abort(); }
void CGameScriptInterface::EntitySetThingAsEnemyOfThing(const CScriptThing&, const CScriptThing&) const { abort(); }
void CGameScriptInterface::EntityUnsetThingAsEnemyOfThing(const CScriptThing&, const CScriptThing&) const { abort(); }
void CGameScriptInterface::EntitySetThingAsAllyOfThing(const CScriptThing&, const CScriptThing&) const { abort(); }
void CGameScriptInterface::EntityUnsetThingAsAllyOfThing(const CScriptThing&, const CScriptThing&) const { abort(); }
void CGameScriptInterface::EntitySetInFaction(const CScriptThing&, const CCharString&) const { abort(); }
void CGameScriptInterface::SetFactionAsAlliedToFaction(const CCharString&, const CCharString&) const { abort(); }
void CGameScriptInterface::SetFactionAsNeutralToFaction(const CCharString&, const CCharString&) const { abort(); }
void CGameScriptInterface::SetFactionAsEnemyToFaction(const CCharString&, const CCharString&) const { abort(); }
bool CGameScriptInterface::AreEntitiesEnemies(const CScriptThing&, const CScriptThing&) const { abort(); }
EOpinionAttitudeType CGameScriptInterface::GetNextInOpinionAttitudeGraph(EOpinionAttitudeType) const { abort(); }
void CGameScriptInterface::GetOpinionAttitudeAsString(EOpinionAttitudeType, CCharString&) const { abort(); }
EOpinionAttitudeType CGameScriptInterface::EntityGetOpinionAttitudeToPlayer(const CScriptThing&) const { abort(); }
void CGameScriptInterface::EntityGetOpinionAttitudeToPlayerAsString(const CScriptThing&, CCharString&) const { abort(); }
float CGameScriptInterface::EntityGetOpinionOfPlayer(const CScriptThing&, EOpinion) const { abort(); }
void CGameScriptInterface::EntitySetOpinionReactionMask(const CScriptThing&, long) const { abort(); }
void CGameScriptInterface::EntitySetOpinionReactionMask(const CScriptThing&, const CCharString&) const { abort(); }
void CGameScriptInterface::EntitySetOpinionDeedMask(const CScriptThing&, long) const { abort(); }
void CGameScriptInterface::EntitySetOpinionDeedMask(const CScriptThing&, const CCharString&) const { abort(); }
void CGameScriptInterface::EntitySetOpinionDeedTypeEnabled(const CScriptThing&, EOpinionDeedType, bool) const { abort(); }
void CGameScriptInterface::EntitySetOpinionAttitudeEnabled(const CScriptThing&, EOpinionAttitudeType, bool) const { abort(); }
void CGameScriptInterface::EntitySetOpinionReactionEnabled(const CScriptThing&, EOpinionReactionType, bool) const { abort(); }
void CGameScriptInterface::EntitySetPersonalityOverride(const CScriptThing&, const CCharString&) const { abort(); }
void CGameScriptInterface::EntitySetPersonalityOverride(const CScriptThing&, long) const { abort(); }
void CGameScriptInterface::EntityClearPersonalityOverride(const CScriptThing&) const { abort(); }
void CGameScriptInterface::EntitySetAsOpinionSource(const CScriptThing&, const CCharString&) const { abort(); }
void CGameScriptInterface::EntitySetAsOpinionSource(const CScriptThing&, long) const { abort(); }
void CGameScriptInterface::EntityUnsetAsOpinionSource(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::OpinionSourceSetAsExclusive(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::OpinionSourceSetAsAttentionGrabbing(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::EntityPostOpinionDeedToAll(const CScriptThing&, EOpinionDeedType) const { abort(); }
void CGameScriptInterface::EntityPostOpinionDeedToRecipient(const CScriptThing&, EOpinionDeedType, const CScriptThing&) const { abort(); }
void CGameScriptInterface::EntityPostOpinionDeedToRecipientVillage(const CScriptThing&, EOpinionDeedType, const CScriptThing&) const { abort(); }
long CGameScriptInterface::EntityPostOpinionDeedKeepSearchingForWitnesses(const CScriptThing&, EOpinionDeedType, const CScriptThing&) const { abort(); }
void CGameScriptInterface::RemoveOpinionDeedStillSearchingForWitnesses(const CScriptThing&, long) const { abort(); }
bool CGameScriptInterface::IsDeedWitnessed(long) const { abort(); }
bool CGameScriptInterface::CanThingBe_Seen_ByOtherThing(const CScriptThing&, const CScriptThing&) const { abort(); }
bool CGameScriptInterface::CanThingBe_NearlySeen_ByOtherThing(const CScriptThing&, const CScriptThing&) const { abort(); }
bool CGameScriptInterface::CanThingBe_Smelled_ByOtherThing(const CScriptThing&, const CScriptThing&) const { abort(); }
bool CGameScriptInterface::CanThingBe_Heard_ByOtherThing(const CScriptThing&, const CScriptThing&) const { abort(); }
bool CGameScriptInterface::IsThingAwareOfOtherThingInAnyWay(const CScriptThing&, const CScriptThing&) const { abort(); }
void CGameScriptInterface::EntitySetAsAwareOfThing(const CScriptThing&, const CScriptThing&) const { abort(); }
void CGameScriptInterface::EntitySetSoundRadius(const CScriptThing&, float) const { abort(); }
void CGameScriptInterface::EntitySetSmellRadius(const CScriptThing&, float) const { abort(); }
void CGameScriptInterface::EntitySetSightRadius(const CScriptThing&, float) const { abort(); }
void CGameScriptInterface::EntitySetExtendedSightRadius(const CScriptThing&, float) const { abort(); }
void CGameScriptInterface::EntitySetGiveUpChaseRadius(const CScriptThing&, float) const { abort(); }
float CGameScriptInterface::EntityGetHearingRadius(const CScriptThing&) const { abort(); }
bool CGameScriptInterface::ManuallyTriggerTrap(const CScriptThing&) const { abort(); }
bool CGameScriptInterface::ManuallyResetTrap(const CScriptThing&) const { abort(); }
void CGameScriptInterface::SetTimeOfDay(float) const { abort(); }
long CGameScriptInterface::GetTimeOfDay() const { abort(); }
void CGameScriptInterface::SetTimeAsStopped(bool, long&) const { abort(); }
void CGameScriptInterface::FastForwardTimeTo(float, float) const { abort(); }
bool CGameScriptInterface::IsTimeOfDayBetween(long, long) const { abort(); }
long CGameScriptInterface::GetDayOfWeek() const { abort(); }
long CGameScriptInterface::GetDayCount() const { abort(); }
long CGameScriptInterface::GetWorldFrame() const { abort(); }
long CGameScriptInterface::GetConstantFPS() const { abort(); }
CCharString CGameScriptInterface::GetActiveQuestName() const { abort(); }
void CGameScriptInterface::TransitionToTheme(const CCharString&, float) const { abort(); }
void CGameScriptInterface::ResetToDefaultTheme(float) const { abort(); }
void CGameScriptInterface::TransitionToThemeAllInternals(const CCharString&, float) const { abort(); }
void CGameScriptInterface::ResetToDefaultThemeAllInternals(float) const { abort(); }
void CGameScriptInterface::TransitionToThemeExternals(const CCharString&, float) const { abort(); }
void CGameScriptInterface::ResetToDefaultThemeExternals(float) const { abort(); }
void CGameScriptInterface::SetEnvironmentThemeWeightAllChannels(const CCharString&, float) const { abort(); }
void CGameScriptInterface::SetEnvironmentThemeWeightAllInternals(const CCharString&, float) const { abort(); }
void CGameScriptInterface::SetEnvironmentThemeWeightExternals(const CCharString&, float) const { abort(); }
void CGameScriptInterface::SetSoundThemesAsEnabledForRegion(const CCharString&, bool) const { abort(); }
void CGameScriptInterface::SetAllSoundsAsMuted(bool) const { abort(); }
void* CGameScriptInterface::RadialBlurFadeTo(float, C3DVector, float, C3DVector, float, float, float) const { abort(); }
void* CGameScriptInterface::RadialBlurFadeTo(float, float, float, float, float, float, float) const { abort(); }
void CGameScriptInterface::RadialBlurFadeOut(float, void*) const { abort(); }
bool CGameScriptInterface::IsRadialBlurFadeActive() const { abort(); }
void CGameScriptInterface::CancelRadialBlurFade() const { abort(); }
void CGameScriptInterface::RadialBlurSetCenterWorldPos(void*, const C3DVector&) const { abort(); }
void* CGameScriptInterface::DisplacementMonochromeEffectColourFadeTo(float, const CRGBFloatColour&) const { abort(); }
void CGameScriptInterface::DisplacementMonochromeEffectColourFadeOut(float, void*) const { abort(); }
void* CGameScriptInterface::ScreenFilterFadeTo(float, float, float, float, float, const CRGBFloatColour&, const std::vector<CScreenFilterSThingByPass,std::allocator<CScreenFilterSThingByPass> >&) const { abort(); }
void CGameScriptInterface::ScreenFilterFadeOut(float, void*) const { abort(); }
void CGameScriptInterface::SetThingAndCarriedItemsNotAffectedByScreenFilter(CScriptThing&, void*) const { abort(); }
void CGameScriptInterface::UnSetThingAndCarriedItemsNotAffectedByScreenFilter(CScriptThing&) const { abort(); }
bool CGameScriptInterface::IsGiftRomantic(const CCharString&) const { abort(); }
bool CGameScriptInterface::IsGiftFriendly(const CCharString&) const { abort(); }
bool CGameScriptInterface::IsGiftOffensive(const CCharString&) const { abort(); }
bool CGameScriptInterface::IsThingABed(const CScriptThing&) const { abort(); }
bool CGameScriptInterface::IsThingAChest(const CScriptThing&) const { abort(); }
bool CGameScriptInterface::IsThingADoor(const CScriptThing&) const { abort(); }
bool CGameScriptInterface::IsThingSmashable(const CScriptThing&) const { abort(); }
bool CGameScriptInterface::IsThingSearchable(const CScriptThing&) const { abort(); }
void CGameScriptInterface::ApplyScriptBrush(const CCharString&) const { abort(); }
void CGameScriptInterface::EnableDecals(bool) const { abort(); }
unsigned long CGameScriptInterface::PlayCriteriaSoundOnThing(const CScriptThing&, const CCharString&) const { abort(); }
unsigned long CGameScriptInterface::PlaySoundOnThing(const CScriptThing&, const CCharString&) const { abort(); }
unsigned long CGameScriptInterface::PlaySoundAtPos(const C3DVector&, const CCharString&) const { abort(); }
unsigned long CGameScriptInterface::Play2DSound(const CCharString&) const { abort(); }
bool CGameScriptInterface::IsSoundPlaying(unsigned long) const { abort(); }
void CGameScriptInterface::EnableSounds(bool) const { abort(); }
void CGameScriptInterface::StopSound(unsigned long) const { abort(); }
void CGameScriptInterface::OverrideMusic(EMusicSetType, bool, bool) const { abort(); }
void CGameScriptInterface::StopOverrideMusic(bool) const { abort(); }
void CGameScriptInterface::CacheMusicSet(EMusicSetType) const { abort(); }
void CGameScriptInterface::EnableDangerMusic(bool) const { abort(); }
bool CGameScriptInterface::IsDangerMusicEnabled() const { abort(); }
void CGameScriptInterface::StartCountdownTimer(float) const { abort(); }
float CGameScriptInterface::GetCountdownTimer() const { abort(); }
void CGameScriptInterface::AutoSaveCheckPoint() const { abort(); }
void CGameScriptInterface::AutoSaveQuestStart() const { abort(); }
void CGameScriptInterface::AutoSave() const { abort(); }
void CGameScriptInterface::SetSavingAsEnabled(bool) const { abort(); }
bool CGameScriptInterface::IsSavingEnabled() const { abort(); }
void CGameScriptInterface::SetSaveGameMarkerPos(const C3DVector&) const { abort(); }
void CGameScriptInterface::ResetToFrontEnd() const { abort(); }
void CGameScriptInterface::SetGuildSealRecallLocation(const C3DVector&, float) const { abort(); }
C3DVector CGameScriptInterface::GetGuildSealRecallPos() const { abort(); }
float CGameScriptInterface::GetGuildSealRecallAngleXY() const { abort(); }
void CGameScriptInterface::SetReadableObjectText(const CScriptThing&, const CWideString&) const { abort(); }
void CGameScriptInterface::SetReadableObjectTextTag(const CScriptThing&, const CCharString&) const { abort(); }
CWideString CGameScriptInterface::GetFormattedString(const CCharString&, const std::vector<CWideString,std::allocator<CWideString> >&) const { abort(); }
CWideString CGameScriptInterface::GetTextString(const CCharString&) const { abort(); }
void CGameScriptInterface::AddRumourCategory(const CCharString&) const { abort(); }
void CGameScriptInterface::AddNewRumourToCategory(const CCharString&, const CCharString&) const { abort(); }
void CGameScriptInterface::RemoveRumourCategory(const CCharString&) const { abort(); }
void CGameScriptInterface::SetCategoryActivity(const CCharString&, bool) const { abort(); }
void CGameScriptInterface::AddGossipVillage(const CCharString&, const CCharString&) const { abort(); }
void CGameScriptInterface::AddGossipFactionToCategory(const CCharString&, const CCharString&) const { abort(); }
void CGameScriptInterface::SetIsGossipForPlayer(const CCharString&, bool) const { abort(); }
void CGameScriptInterface::SetIsGossipForPlayer(CCharString, bool) { abort(); }
void CGameScriptInterface::UpdateOnlineScore_Archery(float) const { abort(); }
void CGameScriptInterface::UpdateOnlineScore_ChickenKick(float) const { abort(); }
void CGameScriptInterface::UpdateOnlineScore_ChapelOrTemple(float) const { abort(); }
void CGameScriptInterface::UpdateOnlineScore_FishingCompo(float) const { abort(); }
void CGameScriptInterface::UpdateScore_FishingCompetition(float) const { abort(); }
float CGameScriptInterface::GetBestTimePairs() const { abort(); }
float CGameScriptInterface::GetBestTimeSorting() const { abort(); }
long CGameScriptInterface::GetBestScoreBlackjack() const { abort(); }
long CGameScriptInterface::GetBestScoreCoinGolfOakVale() const { abort(); }
long CGameScriptInterface::GetBestScoreCoinGolfSnowSpire() const { abort(); }
long CGameScriptInterface::GetBestScoreShoveHaPenny() const { abort(); }
float CGameScriptInterface::GetBestTimeGuessTheAddition() const { abort(); }
bool CGameScriptInterface::IsHeroInTavernGame() const { abort(); }
long CGameScriptInterface::GetNumHousesOwned() const { abort(); }
void CGameScriptInterface::StartSneaking() const { abort(); }
long CGameScriptInterface::GetStealDuration(const CScriptThing&) const { abort(); }
void CGameScriptInterface::SetUseableByHero(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::SetOwnedByHero(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::SetTavernTableAvailableForUse(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::SetIsThingTurncoatable(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::SetIsThingForcePushable(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::SetIsThingLightningable(const CScriptThing&, bool) const { abort(); }
void CGameScriptInterface::SetIsThingEpicSpellable(const CScriptThing&, bool) const { abort(); }
bool CGameScriptInterface::IsThingTurncoated(const CScriptThing&) const { abort(); }
void CGameScriptInterface::AddCreatureScriptedMode(const CScriptThing&, const CCharString&) const { abort(); }
void CGameScriptInterface::RemoveCreatureScriptedMode(const CScriptThing&) const { abort(); }
void CGameScriptInterface::ForceShipsVisible() const { abort(); }
bool CGameScriptInterface::GetSleepingPositionAndOrientationFromBed(const CScriptThing&, const CScriptThing&, C3DVector&, C3DVector&) { abort(); }
void CGameScriptInterface::SetBedAvailability(const CScriptThing&, bool) { abort(); }
void CGameScriptInterface::RepopulateVillage(const CScriptThing&) const { abort(); }
void CGameScriptInterface::SmashAllWindowsWithinRadiusOfPoint(const C3DVector&, float) { abort(); }
CScriptThing CGameScriptInterface::SetResidency(const CScriptThing&, bool) { abort(); }
void CGameScriptInterface::SetThankingPhrase(CScriptThing, unsigned long) { abort(); }
unsigned long CGameScriptInterface::GetThankingPhrase(CScriptThing) { abort(); }
void CGameScriptInterface::ResetThankingPhrase(CScriptThing) { abort(); }
void CGameScriptInterface::SetIgnoringPhrase(CScriptThing, unsigned long) { abort(); }
unsigned long CGameScriptInterface::GetIgnoringPhrase(CScriptThing) { abort(); }
void CGameScriptInterface::ResetIgnoringPhrase(CScriptThing) { abort(); }
void CGameScriptInterface::SetWanderCentrePoint(CScriptThing, C3DVector) { abort(); }
C3DVector CGameScriptInterface::GetWanderCentrePoint(CScriptThing) { abort(); }
void CGameScriptInterface::ResetWanderCentrePoint(CScriptThing) { abort(); }
void CGameScriptInterface::SetWanderMinDistance(CScriptThing, float) { abort(); }
float CGameScriptInterface::GetWanderMinDistance(CScriptThing) { abort(); }
void CGameScriptInterface::ResetWanderMinDistance(CScriptThing) { abort(); }
void CGameScriptInterface::SetWanderMaxDistance(CScriptThing, float) { abort(); }
float CGameScriptInterface::GetWanderMaxDistance(CScriptThing) { abort(); }
void CGameScriptInterface::ResetWanderMaxDistance(CScriptThing) { abort(); }
void CGameScriptInterface::SetGossipCounter(CScriptThing, long) { abort(); }
long CGameScriptInterface::GetGossipCounter(CScriptThing) { abort(); }
void CGameScriptInterface::ResetGossipCounter(CScriptThing) { abort(); }
void CGameScriptInterface::SetMaxGossipPhrase(CScriptThing, long) { abort(); }
long CGameScriptInterface::GetMaxGossipPhrase(CScriptThing) { abort(); }
void CGameScriptInterface::ResetMaxGossipPhrase(CScriptThing) { abort(); }
void CGameScriptInterface::SetWarningPhrase(CScriptThing, unsigned long) { abort(); }
unsigned long CGameScriptInterface::GetWarningPhrase(CScriptThing) { abort(); }
void CGameScriptInterface::ResetWarningPhrase(CScriptThing) { abort(); }
void CGameScriptInterface::SetBeerRequestPhrase(CScriptThing, unsigned long) { abort(); }
unsigned long CGameScriptInterface::GetBeerRequestPhrase(CScriptThing) { abort(); }
void CGameScriptInterface::ResetBeerRequestPhrase(CScriptThing) { abort(); }
void CGameScriptInterface::SetScriptingStateGroup(CScriptThing, EScriptingStateGroups) { abort(); }
EScriptingStateGroups CGameScriptInterface::GetScriptingStateGroup(CScriptThing) { abort(); }
void CGameScriptInterface::ResetScriptingStateGroup(CScriptThing) { abort(); }
void CGameScriptInterface::SetMaxHeroReactionDistance(CScriptThing, float) { abort(); }
float CGameScriptInterface::GetMaxHeroReactionDistance(CScriptThing) { abort(); }
void CGameScriptInterface::ResetMaxHeroReactionDistance(CScriptThing) { abort(); }
void CGameScriptInterface::SetActionFrequency(CScriptThing, long) { abort(); }
long CGameScriptInterface::GetActionFrequency(CScriptThing) { abort(); }
void CGameScriptInterface::ResetActionFrequency(CScriptThing) { abort(); }
void CGameScriptInterface::SetActionFrequencyVariation(CScriptThing, float) { abort(); }
float CGameScriptInterface::GetActionFrequencyVariation(CScriptThing) { abort(); }
void CGameScriptInterface::ResetActionFrequencyVariation(CScriptThing) { abort(); }
void CGameScriptInterface::SetAction(CScriptThing, CCharString) { abort(); }
CCharString CGameScriptInterface::GetAction(CScriptThing) { abort(); }
void CGameScriptInterface::ResetAction(CScriptThing) { abort(); }
void CGameScriptInterface::SetFaceHeroForAction(CScriptThing, bool) { abort(); }
bool CGameScriptInterface::GetFaceHeroForAction(CScriptThing) { abort(); }
void CGameScriptInterface::ResetFaceHeroForAction(CScriptThing) { abort(); }
void CGameScriptInterface::SetTargetName(CScriptThing, CCharString) { abort(); }
CCharString CGameScriptInterface::GetTargetName(CScriptThing) { abort(); }
void CGameScriptInterface::ResetTargetName(CScriptThing) { abort(); }
void CGameScriptInterface::SetFollowDistance(CScriptThing, float) { abort(); }
float CGameScriptInterface::GetFollowDistance(CScriptThing) { abort(); }
void CGameScriptInterface::ResetFollowDistance(CScriptThing) { abort(); }
void CGameScriptInterface::SetAttackHeroOnSight(CScriptThing, bool) { abort(); }
bool CGameScriptInterface::GetAttackHeroOnSight(CScriptThing) { abort(); }
void CGameScriptInterface::ResetAttackHeroOnSight(CScriptThing) { abort(); }
void CGameScriptInterface::SetTimeToSpendHarassingHero(CScriptThing, long) { abort(); }
long CGameScriptInterface::GetTimeToSpendHarassingHero(CScriptThing) { abort(); }
void CGameScriptInterface::ResetTimeToSpendHarassingHero(CScriptThing) { abort(); }
void CGameScriptInterface::SetCombatNearbyEnemyFleeingBreakOffRange(CScriptThing, float) { abort(); }
float CGameScriptInterface::GetCombatNearbyEnemyFleeingBreakOffRange(CScriptThing) { abort(); }
void CGameScriptInterface::ResetCombatNearbyEnemyFleeingBreakOffRange(CScriptThing) { abort(); }
void CGameScriptInterface::SetCombatNearbyBreakOffRange(CScriptThing, float) { abort(); }
float CGameScriptInterface::GetCombatNearbyBreakOffRange(CScriptThing) { abort(); }
void CGameScriptInterface::ResetCombatNearbyBreakOffRange(CScriptThing) { abort(); }
void CGameScriptInterface::SetStealStealableItems(CScriptThing, bool) { abort(); }
bool CGameScriptInterface::GetStealStealableItems(CScriptThing) { abort(); }
void CGameScriptInterface::ResetStealStealableItems(CScriptThing) { abort(); }
void CGameScriptInterface::SetRecoverStealableItems(CScriptThing, bool) { abort(); }
bool CGameScriptInterface::GetRecoverStealableItems(CScriptThing) { abort(); }
void CGameScriptInterface::ResetRecoverStealableItems(CScriptThing) { abort(); }
void CGameScriptInterface::SetTakeStealableItemToRandomDestination(CScriptThing, bool) { abort(); }
bool CGameScriptInterface::GetTakeStealableItemToRandomDestination(CScriptThing) { abort(); }
void CGameScriptInterface::ResetTakeStealableItemToRandomDestination(CScriptThing) { abort(); }
void CGameScriptInterface::SetKillSelfAndStealableItemAfterReachingDestination(CScriptThing, bool) { abort(); }
bool CGameScriptInterface::GetKillSelfAndStealableItemAfterReachingDestination(CScriptThing) { abort(); }
void CGameScriptInterface::ResetKillSelfAndStealableItemAfterReachingDestination(CScriptThing) { abort(); }
void CGameScriptInterface::SetAllowedToFollow(CScriptThing, bool) { abort(); }
bool CGameScriptInterface::GetAllowedToFollow(CScriptThing) { abort(); }
void CGameScriptInterface::ResetAllowedToFollow(CScriptThing) { abort(); }
void CGameScriptInterface::SetTableName(CScriptThing, CCharString) { abort(); }
CCharString CGameScriptInterface::GetTableName(CScriptThing) { abort(); }
void CGameScriptInterface::ResetTableName(CScriptThing) { abort(); }
void CGameScriptInterface::SetSeatName(CScriptThing, CCharString) { abort(); }
CCharString CGameScriptInterface::GetSeatName(CScriptThing) { abort(); }
void CGameScriptInterface::ResetSeatName(CScriptThing) { abort(); }
void CGameScriptInterface::SetDisableHeadLooking(CScriptThing, bool) { abort(); }
bool CGameScriptInterface::GetDisableHeadLooking(CScriptThing) { abort(); }
void CGameScriptInterface::ResetDisableHeadLooking(CScriptThing) { abort(); }
void CGameScriptInterface::SetIsPushableByHero(CScriptThing, bool) { abort(); }
bool CGameScriptInterface::GetIsPushableByHero(CScriptThing) { abort(); }
void CGameScriptInterface::ResetIsPushableByHero(CScriptThing) { abort(); }
void CGameScriptInterface::SetLookForFiniteTime(CScriptThing, bool) { abort(); }
bool CGameScriptInterface::GetLookForFiniteTime(CScriptThing) { abort(); }
void CGameScriptInterface::ResetLookForFiniteTime(CScriptThing) { abort(); }
void CGameScriptInterface::SetAvoidRegionExits(CScriptThing, bool) { abort(); }
bool CGameScriptInterface::GetAvoidRegionExits(CScriptThing) { abort(); }
void CGameScriptInterface::ResetAvoidRegionExits(CScriptThing) { abort(); }
void CGameScriptInterface::SetTargetingDistanceOffset(CScriptThing, float) { abort(); }
float CGameScriptInterface::GetTargetingDistanceOffset(CScriptThing) { abort(); }
void CGameScriptInterface::ResetTargetingDistanceOffset(CScriptThing) { abort(); }
void CGameScriptInterface::SetPlayerUsingMeleeDummies(bool) { abort(); }
bool CGameScriptInterface::GetPlayerUsingMeleeDummies() { abort(); }
void CGameScriptInterface::SetPlayerUsingRangedDummies(bool) { abort(); }
bool CGameScriptInterface::GetPlayerUsingRangedDummies() { abort(); }
void CGameScriptInterface::SetPlayerUsingWillDummies(bool) { abort(); }
bool CGameScriptInterface::GetPlayerUsingWillDummies() { abort(); }
void CGameScriptInterface::SetCheapHeadLooking(bool) { abort(); }
bool CGameScriptInterface::GetCheapHeadLooking() { abort(); }
void CGameScriptInterface::SetQuitTavernGame(bool) { abort(); }
bool CGameScriptInterface::GetQuitTavernGame() { abort(); }
void CGameScriptInterface::SetPrizeTavernTable(bool) { abort(); }
bool CGameScriptInterface::GetPrizeTavernTable() { abort(); }
void CGameScriptInterface::SetBettingActive(bool) { abort(); }
bool CGameScriptInterface::GetBettingActive() { abort(); }
void CGameScriptInterface::SetBettingAccept(bool) { abort(); }
bool CGameScriptInterface::GetBettingAccept() { abort(); }
void CGameScriptInterface::SetBettingAmount(long) { abort(); }
long CGameScriptInterface::GetBettingAmount() { abort(); }
void CGameScriptInterface::SetCountBetMoneyDown(bool) { abort(); }
bool CGameScriptInterface::GetCountBetMoneyDown() { abort(); }
void CGameScriptInterface::SetSpotTheAdditionBeaten(bool) { abort(); }
bool CGameScriptInterface::GetSpotTheAdditionBeaten() { abort(); }
void CGameScriptInterface::SetGlobalTargetingDistanceOffset(float) { abort(); }
float CGameScriptInterface::GetGlobalTargetingDistanceOffset() { abort(); }
void CGameScriptInterface::SetTradingPriceMult(float) { abort(); }
float CGameScriptInterface::GetTradingPriceMult() { abort(); }
void CGameScriptInterface::SetBoastingEnabled(bool) { abort(); }
bool CGameScriptInterface::GetBoastingEnabled() { abort(); }
void CGameScriptInterface::SetActiveGossipCategories(CCharString, bool) { abort(); }
bool* CGameScriptInterface::GetActiveGossipCategories(CCharString) { abort(); }
const std::map<CCharString,bool,std::less<CCharString>,std::allocator<std::pair<CCharString const,bool> > >& CGameScriptInterface::GetActiveGossipCategories() { abort(); }
int CGameScriptInterface::GetActiveGossipCategoriesSize() { abort(); }
void CGameScriptInterface::ClearActiveGossipCategories() { abort(); }
bool* CGameScriptInterface::GetIsGossipForPlayer(CCharString) { abort(); }
const std::map<CCharString,bool,std::less<CCharString>,std::allocator<std::pair<CCharString const,bool> > >& CGameScriptInterface::GetIsGossipForPlayer() { abort(); }
int CGameScriptInterface::GetIsGossipForPlayerSize() { abort(); }
void CGameScriptInterface::ClearIsGossipForPlayer() { abort(); }
void CGameScriptInterface::SetGossip(CCharString, CCharString, long) { abort(); }
CCharString CGameScriptInterface::GetGossip(CCharString, long) { abort(); }
const std::vector<CCharString,std::allocator<CCharString> >* CGameScriptInterface::GetGossip(CCharString) { abort(); }
int CGameScriptInterface::GetGossipSize(CCharString) { abort(); }
void CGameScriptInterface::ClearGossip(CCharString) { abort(); }
void CGameScriptInterface::RemoveGossip(CCharString) { abort(); }
void CGameScriptInterface::AddGossip(CCharString, CCharString) { abort(); }
void CGameScriptInterface::AddGossip(CCharString) { abort(); }
void CGameScriptInterface::SetGossipVillages(CCharString, CCharString, long) { abort(); }
CCharString CGameScriptInterface::GetGossipVillages(CCharString, long) { abort(); }
const std::vector<CCharString,std::allocator<CCharString> >* CGameScriptInterface::GetGossipVillages(CCharString) { abort(); }
int CGameScriptInterface::GetGossipVillagesSize(CCharString) { abort(); }
void CGameScriptInterface::ClearGossipVillages(CCharString) { abort(); }
void CGameScriptInterface::RemoveGossipVillages(CCharString) { abort(); }
void CGameScriptInterface::AddGossipVillages(CCharString, CCharString) { abort(); }
void CGameScriptInterface::AddGossipVillages(CCharString) { abort(); }
void CGameScriptInterface::SetGossipFactions(CCharString, CCharString, long) { abort(); }
CCharString CGameScriptInterface::GetGossipFactions(CCharString, long) { abort(); }
const std::vector<CCharString,std::allocator<CCharString> >* CGameScriptInterface::GetGossipFactions(CCharString) { abort(); }
int CGameScriptInterface::GetGossipFactionsSize(CCharString) { abort(); }
void CGameScriptInterface::ClearGossipFactions(CCharString) { abort(); }
void CGameScriptInterface::RemoveGossipFactions(CCharString) { abort(); }
void CGameScriptInterface::AddGossipFactions(CCharString, CCharString) { abort(); }
void CGameScriptInterface::AddGossipFactions(CCharString) { abort(); }
void CGameScriptInterface::SetTrapAsActive(const CScriptThing&, bool) const { abort(); }
bool CMainGameComponent::Run(CGameComponent**) { abort(); }
void CMainGameComponent::ChangeTextureColourDepth(long) { abort(); }
void CMainGameComponent::OnPreDeviceReset() { abort(); }
bool CMainGameComponent::OnPostDeviceReset() { abort(); }
void CGameComponent::Init() { abort(); }
void CGameComponent::SetQuit() { abort(); }
void CGameDefinitionManager::InitAndCompile() { abort(); }
bool CResourceList::HandleFailedAllocation(unsigned long) { abort(); }
bool CResource::UnloadResource() { abort(); }
unsigned long CResource::GetUnloadableMemory() const { abort(); }
bool CResource::IsResourceLoaded() const { abort(); }
void CResource::OnResourceLoaded() { abort(); }
bool CResource::CanAbortResourceLoad(long) const { abort(); }
void CResource::OnResourceLoadAborted() { abort(); }
unsigned long CResource::GetDebugMemoryInUse() const { abort(); }
void CResource::ResetDebugMemoryInUse() { abort(); }
void CIVCountedPointeeBase::Delete() const { abort(); }
static unsigned cases;
int main(){
 CGame game;CMainGameComponentInit init;CMainGameComponent component(game,init);CGameDefinitionManager defs;CWorld world;CPlayerManager players;CDisplayEngine display;
 CGameScriptInterface script(component,defs,world,players,display);
 CThing observed,observer;CScriptThing a,b;
 FixturePhysics physics(observer);CTCPerceiveThing perception(observer);
 viewedThing=&observed;observerThing=&observer;viewedScript=&a;
 observer.PhysicsTC=&physics;
 typedef std::pair<ETCInterfaceType,CTCBase*> Entry;
 Entry entries[2];entries[0].first=TCI_PERCEIVE_THING;entries[0].second=&perception;entries[1].first=(ETCInterfaceType)999;entries[1].second=0;
 observer.Components._M_start=entries;observer.Components._M_finish=entries+1;observer.Components._M_end_of_storage._M_data=entries+2;observer.Components.Dirty=false;
 memset(observer.UsingComponents._M_w,0,sizeof(observer.UsingComponents._M_w));observer.UsingComponents._M_w[TCI_PERCEIVE_THING/(8*sizeof(unsigned long))]|=1UL<<(TCI_PERCEIVE_THING%(8*sizeof(unsigned long)));
 observerPos=C3DVector(0,0,0);viewedPos=C3DVector(1,0,0);facing=C3DVector(1,0,0);angle=0.25f;
 perception.SoundRadius=2;perception.SmellRadius=1;perception.SightRadius=2;perception.ExtendedSightRadius=4;
 for(unsigned i=0;i<128;++i){unsigned int word=((i<64?128U:127U)<<23)|((i&63U)<<17);float value;memcpy(&value,&word,4);value=(float)(1.0/sqrt(value));memcpy(&word,&value,4);GFastOneOverSqrtTable[i]=(word&0x00ffffffU)>>15;}
 // All cached senses, including false/true values and invalid selectors; no geometric callees.
 for(unsigned bits=0;bits<32;++bits)for(long sense=-1;sense<=7;++sense){
  viewedAlive=observerAlive=true;currentTarget=&observed;
  perception.LineOfSight=(bits&1)!=0;perception.ExtendedLineOfSight=(bits&2)!=0;perception.Smelt=(bits&4)!=0;perception.Heard=(bits&8)!=0;perception.AwarenessTimer=(bits&16)?1.0f:0.0f;
  aliveCalls=thingCalls=posCalls=facingCalls=radiusCalls=0;
  bool expected=sense>=1&&sense<=5&&((bits>>(sense-1))&1)!=0;
  assert(script.CanThingBe_Sensed_ByOtherThing(a,b,sense)==expected);assert(aliveCalls==2&&thingCalls==2&&posCalls==0&&radiusCalls==0&&facingCalls==0);++cases;
 }
 // Radius boundaries and front/back cone directions, with no cached target or a different target.
 for(unsigned cached=0;cached<2;++cached)for(unsigned far=0;far<5;++far)for(unsigned back=0;back<2;++back)for(long sense=1;sense<=5;++sense){
  viewedAlive=observerAlive=true;currentTarget=cached?&observer:0;
  const float distances[]={0.5f,1,2,4,8};float distance=distances[far];viewedPos=C3DVector(back?-distance:distance,0,0);
  bool expected=false;
  if(sense==3)expected=distance<=1;
  else if(sense==4)expected=distance<=2;
  else if(sense==5&&distance<=2)expected=true;
  else expected=distance<=(sense==1?2.0f:4.0f)&&!back;
  aliveCalls=thingCalls=posCalls=facingCalls=radiusCalls=0;
  assert(script.CanThingBe_Sensed_ByOtherThing(a,b,sense)==expected);assert(aliveCalls==2&&thingCalls==2);++cases;
 }
 for(unsigned dead=0;dead<3;++dead)for(long sense=0;sense<=6;++sense){viewedAlive=(dead&1)!=0;observerAlive=(dead&2)!=0;aliveCalls=thingCalls=posCalls=facingCalls=radiusCalls=0;assert(!script.CanThingBe_Sensed_ByOtherThing(a,b,sense));assert(thingCalls==0&&posCalls==0&&facingCalls==0&&radiusCalls==0);++cases;}
 viewedAlive=observerAlive=true;memset(observer.UsingComponents._M_w,0,sizeof(observer.UsingComponents._M_w));for(long sense=0;sense<=6;++sense){assert(!script.CanThingBe_Sensed_ByOtherThing(a,b,sense));++cases;}
 observer.Components._M_start=observer.Components._M_finish=observer.Components._M_end_of_storage._M_data=0;
 printf("0089c910_TEST PASS\n");fprintf(stderr,"%u cases\n",cases);return 0;
}
