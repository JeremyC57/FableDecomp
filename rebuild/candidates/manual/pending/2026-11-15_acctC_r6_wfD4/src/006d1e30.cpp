// CTCWife::FrameUpdate() -- the wife's per-frame update: brain swap after marriage / divorce, attitude refresh, divorce / sex-offer /
// gift / house-dressing deeds while she can see the hero, gift-giving value, marriage state in the hero's stats; while courting, the
// love attitude, falling in love (lover count, online scoreboard) and the attraction icon; finally the culling of remembered gifts
// Leaf: ?FrameUpdate@CTCWife@@UAEXXZ
// Grade: RELOCATION_MATCH (3484 bytes) -- wfD4 successors 5..8 (s8 closed it: GetParentThing() accessor, visible IS_THING_ALIVE<T> body,
//   CTCHeroStats::LogLoverAcquired / GetNumberOfLoversAcquired).
// Retail: Fable.exe (TLC, VC7.1) @ 0x006D1E30, 3484 bytes, plain `ret`, frame 0x2C; CTCWife vtable 0x0125ED5C slot 9. Ego_r.pdb 0x007072E0 /
//   FableWin.pdb 0x02485720 ?FrameUpdate@CTCWife@@UAEXXZ.
// Evidence for the spellings (EVIDENCED unless marked FITTED):
//   - the parent thing is read through the inline accessor CTCBase::GetParentThing() (FableWin.pdb ?GetParentThing@CTCBase@@QAEAAVCThing@@XZ;
//     same-TU siblings Marry 0x006D18A0 / InitWifeAtEndOfMarriageFrescoStartFadeIn 0x006CF3B0): retail evaluates it before the object
//     expression of GetThingAttitudeToPlayer (edi loaded before the GetWorld call, 0x006D1ED0) and reloads it after the member stores
//     at the PostDeed sites (0x006D20C1, 0x006D2107).
//   - falling in love: `stats->LogLoverAcquired()` and `(float)stats->GetNumberOfLoversAcquired()` (FableWin.pdb ?LogLoverAcquired@CTCHeroStats@@QAEXXZ
//     0x024884F0 in this TU's COMDAT run, /Od 12i/32b = `NumLoversAcquired++` (fwsize SAME); ?GetNumberOfLoversAcquired@CTCHeroStats@@QBEJXZ):
//     c2 keeps the count in [esp+1Ch] across the scoreboard lookup (0x006D290F) and the expression-temporary rotation after it matches.
//   - IS_THING_ALIVE<CThingPlayerCreature*> (0x0043DBD0) body visible, kept out of line (ThingAlive.h): the hero stays in ebp and the
//     callee gets a [esp+20h] copy (0x006D28B9).
//   - brain arms: two complete if/else arms (VC7.1 cross-jumps them: `push -1 ; jg ; push A ; jmp ; push B`, 0x006D1E4A); the named
//     `creature` reference before SetNewBrain (Marry's idiom; retail loads [esi+4] before the CDefPointer argument copy, 0x006D1E88) FITTED.
//   - SetMarriageState: one call after `state` set in the arms and `if (state != 0)` (c2 duplicates the tail: `mov eax,N ; push eax`
//     0x006D26FD / 0x006D2730 / 0x006D2746) FITTED; gift opinion: first term squared before the second (0x006D2488) FITTED order;
//     culling loop: named `end` / `next`, `++next` at the top, age = GetFrame() - it->second before ConstantFPS FITTED.
// Shape: IsMarriedToThePlayer's body (landed 0x006CEC80 text) is in the TU (expanded twice, as retail); the by-name GetDef<T> template
//   body via FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_INLINE (called out of line: this TU's CBrainDef COMDAT 0x006D3E80, landed text);
//   GFLimitReturn<T> is the landed 0x0057EF86 text (inlined at the opinion terms, called out of line for the married hours); std::map
//   erase(iterator) inline (STLport _tree.h: _Rebalance_for_erase 0x0042A34E, free, --count).
#define FABLE_RETAIL_CTCHEROSTATS_MARRIAGE
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_INLINE
#define FABLE_RETAIL_CTCHEROSTATS_LOVERS
#define FABLE_RETAIL_CTCHEROSTATS_SERIALISE
#define FABLE_RETAIL_CTCWIFE_MARRY
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGBUILDING
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDISPLAYENGINE
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CSUBTITLERENDERER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CTCBUYABLEHOUSE
#define FABLE_RETAIL_CTCWIFE
#define FABLE_RETAIL_CHEROMARRIAGEDEF
#define FABLE_RETAIL_CGAMECAMERAMANAGER
#define FABLE_RETAIL_CAIGAMECAMERABASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CTCPERCEIVETHING
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CHEROMARRIAGEINFO
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_SERIALISE
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_DEEDS
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_STLP_TREE_ERASE_BODY
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CDISPLAYENGINE_ENDSCREENFADEOUT
#define FABLE_RETAIL_CDISPLAYENGINE_ISSCREENFULLYFADEDOUT
#define FABLE_RETAIL_CDISPLAYENGINE_SCREENFADEOUT
#define FABLE_RETAIL_CFADEINFADEOUTBASE_OPEN
#define FABLE_RETAIL_CGAMECAMERAMANAGER_RESETCAMERA
#define FABLE_RETAIL_CGAMETIMEMANAGER_FULL
#define FABLE_RETAIL_CMAINGAMECOMPONENT_CUTSCENEFONT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_DISPLAYENGINE
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD
#define FABLE_RETAIL_CMAINGAMECOMPONENT_SERVERFRAME_OUTOFLINE
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CMAINGAMECOMPONENT_WORLD
#define FABLE_RETAIL_CMESSAGEEVENT
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS
#define FABLE_RETAIL_CMESSAGEEVENTFILTERS_ISOFTYPE_CTOR
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_FINDRECENT
#define FABLE_RETAIL_CMESSAGEEVENTMANAGER_REMOVE
#define FABLE_RETAIL_CSUBTITLERENDERER_CUTSCENEFLAGS
#define FABLE_RETAIL_CSUBTITLERENDERER_SETTEXT
#define FABLE_RETAIL_CSUBTITLERENDERER_SETTEXT_CHAR
#define FABLE_RETAIL_CTCBASE_WORLDSEED
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CTCPHYSICSBASE_STATTYPE
#define FABLE_RETAIL_CTCSOUNDPLAYER_ATTACHEDDIALOGUE
#define FABLE_RETAIL_CTCUNHOLYPOWER_CANCELFORCUTSCENE
#define FABLE_RETAIL_CTCVILLAGE_ONHEROSLEPT
#define FABLE_RETAIL_CTCVILLAGE_TCI
#define FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING
#define FABLE_RETAIL_CTHINGAICREATURE_SAVE
#define FABLE_RETAIL_CTHINGCREATUREBASE_AUTOPLACENEAR
#define FABLE_RETAIL_CTHINGCREATUREBASE_GETSEX
#define FABLE_RETAIL_CWORLD_GAMESCRIPTINTERFACE
#define FABLE_RETAIL_CWORLD_MESSAGEEVENTMANAGER
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_ISOPEN
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER_LAYOUT
#define FABLE_RETAIL_PLAYER_REMOVEMODE
#define FABLE_RETAIL_CTCPERCEIVETHING_AWARENESS
#define FABLE_RETAIL_CTCPERCEIVETHING_AWARE_CALLED
#define FABLE_RETAIL_CTHINGAICREATURE_SETNEWBRAIN
#define FABLE_RETAIL_CTHINGAICREATURE_WIFE_QUERIES
#define FABLE_RETAIL_CTCBUYABLEHOUSE_CURRENTDRESSLEVEL
#define FABLE_RETAIL_CTCBUYABLEHOUSE_FRAMEUPDATE
#define FABLE_RETAIL_CHEROMARRIAGEINFO_STATE
#define FABLE_RETAIL_CHEROMARRIAGEINFO_DEFAULT_CTOR
#define FABLE_RETAIL_COPINIONSET_SUBSCRIPT_CONST
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_GREET
#define FABLE_RETAIL_CTCPERCEIVETHING_CANSEEORSMELL
#define FABLE_RETAIL_CTCHEROSTATS_PEEKMARRIAGEINFO
#define FABLE_RETAIL_COPINIONREACTIONMANAGER_REACTIONMATCHSTRENGTH
#define FABLE_RETAIL_CTCSOUNDPLAYER_SETSPECIFICSAMPLEVOLUME
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_SETPERSONALITYOVERRIDE_INDEX
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "fable_game.h"
#include "engine/CTCWife.h"
#include "engine/CHeroMarriageDef.h"
#include "engine/CWifeDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/CThingBaseDef.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingBuilding.h"
#include "engine/CThingVillage.h"
#include "engine/CThingFilter_None.h"
#include "engine/CThingSearchTools.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCVillage.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CTCBuyableHouse.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/CWorld.h"
#include "engine/CDisplayEngine.h"
#include "engine/CPlayerGui.h"
#include "engine/CSubtitleRenderer.h"
#include "engine/NHeroInformationScreens_CManager.h"
#include "engine/CMessageEventManager.h"
#include "engine/CGameTimeManager.h"
#include "engine/CAIGameCameraBase.h"
#include "engine/CGameCameraManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CTCPerceiveThing.h"
#include "engine/CTCHeroStats.h"
#include "engine/CHeroMarriageInfo.h"
#include "engine/CTCHeroOnlineScoreboard.h"
#include "engine/CTCVolumeContainmentTracker.h"
#include "engine/CTCCreatureOpinionOfHero.h"
#include "engine/COpinionReactionManager.h"

enum EOpinionReactionType;
float __fastcall GFFloatRandom(float range, unsigned long* seed);                                   // 0x004978A0
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#include "engine/ThingAlive.h"   // IS_THING_ALIVE<CThingPlayerCreature*> COMDAT 0x0043DBD0, body visible, kept out of line
template <class T> const T& __fastcall GFLimitReturn(const T& value, const T& lo, const T& hi)
{
    return value < lo ? lo : value > hi ? hi : value;
}
extern const unsigned long INVALID_SAMPLE_ID;                                // 0x0129D25C
// CThingAICreature::GetHomeBuilding (inline in retail; the header leaves its body to the includer, as landed 0x00958840 / 0x00946400).
inline CThingBuilding* CThingAICreature::GetHomeBuilding() const { return HomeBuilding.Get(); }
// Same-TU body of the out-of-line query 0x006CEC80 (landed text, CTCWife.cpp): FrameUpdate expands it twice.
bool CTCWife::IsMarriedToThePlayer() const { return FrameGotMarriedToThePlayer <= CWorld::GetFrame() ? true : false; }

void CTCWife::FrameUpdate()
{
    if (NeedsToChangeBrain) {
        if (IsMarriedToThePlayer()) {
            CDefPointer<const CBrainDef> brain;
            GetDefinitionManager().GetDef("BRAIN_WIFE", &brain);
            CThingAICreature& creature = static_cast<CThingAICreature&>(GetParentThing());
            creature.SetNewBrain(brain);
            NeedsToChangeBrain = false;
        } else {
            CDefPointer<const CBrainDef> brain;
            GetDefinitionManager().GetDef("BRAIN_DIVORCED_WIFE", &brain);
            CThingAICreature& creature = static_cast<CThingAICreature&>(GetParentThing());
            creature.SetNewBrain(brain);
            NeedsToChangeBrain = false;
        }
    }
    if (static_cast<CThingAICreature*>(&GetParentThing())->IsBrainUpdateZoneActiveThisFrame()) {
        if (AttitudeUpdateCounter % 3 == 0) {
            OpinionAttitude = GetWorld().GetOpinionReactionManager().GetThingAttitudeToPlayer(GetParentThing());
            AttitudeUpdateCounter = 0;
        }
        ++AttitudeUpdateCounter;
    }
    long frame = CWorld::GetFrame();
    if (IsMarriedToThePlayer()) {
        SetAttractionIcon(true, 0.5f, 0xff);
        if (OpinionAttitude == (EOpinionAttitudeType)17) {
            if (FrameEnteredAttitudeHate == 0x7fffffff) {
                FrameEnteredAttitudeHate = GetWorld().GetFrame();
                FrameLastGaveDivorceWarning = (long)(FrameEnteredAttitudeHate - CMainGameComponent::GetConstantFPS() * PHeroMarriageDef->SecsInHateBetweenDivorceWarnings);
            } else if (!DivorceDeedPending &&
                       (float)(GetWorld().GetFrame() - FrameEnteredAttitudeHate) > CMainGameComponent::GetConstantFPS() * PHeroMarriageDef->SecsInHateBeforeDivorceOccurs) {
                DivorceDeedPending = true;
            } else if (!DivorceWarningPending &&
                       (float)(GetWorld().GetFrame() - FrameLastGaveDivorceWarning) > CMainGameComponent::GetConstantFPS() * PHeroMarriageDef->SecsInHateBetweenDivorceWarnings) {
                DivorceWarningPending = true;
            }
        } else {
            FrameEnteredAttitudeHate = 0x7fffffff;
        }
        CThingPlayerCreature* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        CTCPerceiveThing* perceive;
        if (hero && !hero->IsDead() && GetParentThing().GetTC(&perceive)) {
            if (perceive->GetPerceivedThing() == hero && perceive->IsAwareOfThing()) {
                CTCHeroOpinionDeedLog* deed_log;
                hero->GetTC(&deed_log);
                if (DivorceDeedPending) {
                    DivorceDeedPending = false;
                    deed_log->PostDeed((EOpinionDeedType)0x59, hero->GetPos(), 0, static_cast<CThingAICreature*>(&GetParentThing()), (EOpinionPostDeedType)1, false, false, 0, false, 0);
                    return;
                }
                if (DivorceWarningPending) {
                    DivorceWarningPending = false;
                    FrameLastGaveDivorceWarning = GetWorld().GetFrame();
                    deed_log->PostDeed((EOpinionDeedType)0x58, hero->GetPos(), 0, static_cast<CThingAICreature*>(&GetParentThing()), (EOpinionPostDeedType)1, false, false, 0, false, 0);
                }
                const CTCVolumeContainmentTracker* tracker;
                hero->PeekTC(&tracker);
                if (OpinionAttitude == (EOpinionAttitudeType)13 && tracker->GetPThingInside() &&
                    tracker->GetPThingInside() == static_cast<CThingAICreature*>(&GetParentThing())->GetHomeBuilding()) {
                    if (FrameEnteredLoveWithHusbandPresentAtHome == 0x7fffffff) {
                        FrameEnteredLoveWithHusbandPresentAtHome = GetWorld().GetFrame();
                        FrameLastGaveSexOffer = (long)(FrameEnteredLoveWithHusbandPresentAtHome - CMainGameComponent::GetConstantFPS() * PHeroMarriageDef->SecsInLoveAndHusbandPresentBetweenSexOffers);
                    } else if ((float)(GetWorld().GetFrame() - FrameEnteredLoveWithHusbandPresentAtHome) > CMainGameComponent::GetConstantFPS() * PHeroMarriageDef->SecsInLoveAndHusbandPresentBeforeSexOffered &&
                               (float)(GetWorld().GetFrame() - FrameLastGaveSexOffer) > CMainGameComponent::GetConstantFPS() * PHeroMarriageDef->SecsInLoveAndHusbandPresentBetweenSexOffers) {
                        FrameLastGaveSexOffer = GetWorld().GetFrame();
                        deed_log->PostDeed((EOpinionDeedType)0x5a, hero->GetPos(), 0, static_cast<CThingAICreature*>(&GetParentThing()), (EOpinionPostDeedType)1, false, false, 0, false, 0);
                    }
                } else {
                    FrameEnteredLoveWithHusbandPresentAtHome = 0x7fffffff;
                }
                if ((float)(frame - FrameLastAwareOfHusband) > CMainGameComponent::GetConstantFPS() * PHeroMarriageDef->SecondsTimeSinceSeeingYouMinThreshold) {
                    deed_log->PostDeed((EOpinionDeedType)0x51, hero->GetPos(), 0, static_cast<CThingAICreature*>(&GetParentThing()), (EOpinionPostDeedType)1, false, false, 0, false, 0);
                    FrameLastAwareOfHusband = frame;
                }
                CThingAICreature* wife = static_cast<CThingAICreature*>(&GetParentThing());
                const CTCBuyableHouse* house;
                if (wife->GetHomeBuilding() && wife->GetHomeBuilding()->PeekTC(&house) && hero->PeekTC(&tracker) &&
                    tracker->GetPThingInside() == wife->GetHomeBuilding() &&
                    house->GetCurrentDressLevel() > HouseDressingLevelLastCommentedOn) {
                    deed_log->PostDeed((EOpinionDeedType)0x56, hero->GetPos(), 0, static_cast<CThingAICreature*>(&GetParentThing()), (EOpinionPostDeedType)1, false, false, 0, false, 0);
                    HouseDressingLevelLastCommentedOn = house->GetCurrentDressLevel();
                }
                if (perceive->CanSeeOrSmellPerceivedThing() && frame > FrameToCheckAppearanceChanges) {
                    CheckAppearanceChanges(*hero);
                    FrameToCheckAppearanceChanges = frame + PHeroMarriageDef->SecondsBetweenCheckingPlayersAppearance * CMainGameComponent::GetConstantFPS();
                }
            } else {
                FrameEnteredLoveWithHusbandPresentAtHome = 0x7fffffff;
            }
            C3DVector offset = hero->GetPos() - GetParentThing().GetPos();
            float distance = (float)sqrt(offset.GetSquaredMagnitude2D());
            if (distance < perceive->GetExtendedSightRadius())
                FrameLastAwareOfHusband = frame;
        }
        CTCCreatureOpinionOfHero* opinion = 0;
        GetParentThing().GetTC(&opinion);
        long gift_period = PHeroMarriageDef->SecondsBetweenEvaluatingGiftOpinion * CMainGameComponent::GetConstantFPS();
        if (frame - FrameLastEvaluatedGiftOpinion > gift_period) {
            float a = 1.0f - GFLimitReturn(opinion->RelativeOpinion[3], -1.0f, 1.0f);
            float distance_squared = a * a;
            float b = 1.0f - GFLimitReturn(opinion->RelativeOpinion[4], -1.0f, 1.0f);
            distance_squared += b * b;
            float distance = (float)sqrt(distance_squared);
            while (frame - FrameLastEvaluatedGiftOpinion > gift_period) {
                FrameLastEvaluatedGiftOpinion += gift_period;
                GiftGivingOpinionDistanceFromMax = (distance + GiftGivingOpinionDistanceFromMax) * 0.5f;
            }
        }
        if (GiftToGiveDef <= 0 &&
            frame - FrameLastConsideredGivingGift > PHeroMarriageDef->SecondsBetweenGiftGivingOpportunities * CMainGameComponent::GetConstantFPS()) {
            float closeness = (PHeroMarriageDef->MaxDistanceFromMaxStatsThatGiftsCanBeGiven - GiftGivingOpinionDistanceFromMax) / PHeroMarriageDef->MaxDistanceFromMaxStatsThatGiftsCanBeGiven;
            if (closeness > 0.0f) {
                float weight = closeness + closeness - closeness * closeness;
                if (GiftGivingPriceValue > 0 && GFFloatRandom(1.0f, GetWorld().GetWorldSeed()) < PHeroMarriageDef->ChanceThatGiftWillBeGiven)
                    FindAppropriateGift();
                GiftGivingPriceValue += PHeroMarriageDef->AmountOfGoldPerHourOfMarriageToIncreaseGiftGivingPriceValuePerHourOfPlay *
                    GFLimitReturn(GetNumHoursMarriedFor(), 1.0f, PHeroMarriageDef->MaxNumHoursOfMarriageToIncreaseGiftGivingPriceValue) /
                    (3600.0f / PHeroMarriageDef->SecondsBetweenGiftGivingOpportunities) * weight;
                if (GiftGivingPriceValue > 0)
                    GiftGivingPriceValue = (long)(GiftGivingPriceValue * PHeroMarriageDef->MultiplierForGiftGivingValueEveryOpportunity);
            }
            FrameLastConsideredGivingGift = CWorld::GetFrame();
        }
        if (hero && !hero->IsDead()) {
            CTCHeroStats* stats;
            hero->GetTC(&stats);
            CHeroMarriageInfo info;
            if (stats->PeekMarriageInfo(GetParentThing().GetUniqueID(), info) && info.State != (EMarriageState)4) {
                EMarriageState state = (EMarriageState)0;
                if ((float)(CWorld::GetFrame() - FrameGotMarriedToThePlayer) < CMainGameComponent::GetConstantFPS() * PHeroMarriageDef->SecondsJustMarriedDuration) {
                    JustMarried = true;
                    state = (EMarriageState)1;
                } else {
                    JustMarried = false;
                    switch (OpinionAttitude) {
                    case 13: case 14:
                        state = (EMarriageState)3;
                        break;
                    case 15: case 16: case 17:
                        state = (EMarriageState)2;
                        break;
                    }
                }
                if (state != (EMarriageState)0)
                    stats->SetMarriageState(static_cast<CThingAICreature*>(&GetParentThing()), state);
            }
        }
    } else {
        CThingAICreature* wife = static_cast<CThingAICreature*>(&GetParentThing());
        if (wife->IsCourtable()) {
            long love_period = PHeroMarriageDef->SecondsBetweenEvaluatingLoveAttitude * CMainGameComponent::GetConstantFPS();
            long elapsed = frame - FrameLastEvaluatedLoveAttitude;
            if (elapsed > love_period) {
                float strength = PeekWorld().PeekOpinionReactionManager().GetReactionMatchStrength((EOpinionReactionType)0xb, *wife);
                if (IsCourtableAndNotBlockingIt()) {
                    if (strength > LoveAttitudeValue)
                        LoveAttitudeValue = (LoveAttitudeValue * 9.0f + strength) * 0.1f;
                    else
                        LoveAttitudeValue = strength;
                } else {
                    LoveAttitudeValue = 0.0f;
                }
                FrameLastEvaluatedLoveAttitude += love_period;
                elapsed -= love_period;
                if (elapsed > love_period) {
                    if (!ReceivedWeddingRing)
                        LoveAttitudeValue -= elapsed * (1.0f / (CMainGameComponent::GetConstantFPS() * PHeroMarriageDef->SecondsOutOfTheRegionForLoveAttitudeToDecayOneToZero));
                    FrameLastEvaluatedLoveAttitude = frame;
                }
                if (LoveAttitudeValue >= PHeroMarriageDef->ThresholdForLoveAttitudeOverride) {
                    CTCCreatureOpinionOfHero* opinion = 0;
                    GetParentThing().GetTC(&opinion);
                    opinion->ForcedAttitude = (EOpinionAttitudeType)12;
                    if (!HasBeenInLoveWithPlayer) {
                        HasBeenInLoveWithPlayer = true;
                        CThingPlayerCreature* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
                        CTCHeroStats* stats;
                        if (IS_THING_ALIVE(hero) && hero->GetTC(&stats)) {
                            stats->LogLoverAcquired();
                            CTCHeroOnlineScoreboard* scoreboard;
                            hero->GetTC(&scoreboard);
                            scoreboard->UpdateScoreForActivity((CTCHeroOnlineScoreboard::EOnlineScoreboardActivity)0xb, (float)stats->GetNumberOfLoversAcquired(), false);
                        }
                        opinion->SetPersonalityOverride(PHeroMarriageDef->FianceeOpinionPersonalityDef, false);
                        CTCSoundPlayer* sound_player;
                        if (GetParentThing().GetTC(&sound_player)) {
                            unsigned long sample = sound_player->PlayAttachedCriteria(PHeroMarriageDef->FallenInLoveSoundCriteria, true, false, false, -1);
                            if (sample != INVALID_SAMPLE_ID)
                                sound_player->SetSpecificSampleVolume(sample, PHeroMarriageDef->FallenInLoveSoundGain);
                        }
                    }
                } else {
                    CTCCreatureOpinionOfHero* opinion = 0;
                    GetParentThing().GetTC(&opinion);
                    if (opinion->ForcedAttitude == (EOpinionAttitudeType)12)
                        opinion->ForcedAttitude = (EOpinionAttitudeType)18;
                }
            }
            const CTCCreatureOpinionOfHero* opinion = 0;
            GetParentThing().PeekTC(&opinion);
            if (GetParentThing().HasTC(TCI_EMOTEICON)) {
                if (LoveAttitudeValue > 0.0f && opinion->GreetedFlag) {
                    float attraction = LoveAttitudeValue / PHeroMarriageDef->ThresholdForLoveAttitudeOverride;
                    if (attraction > 1.0f)
                        attraction = 1.0f;
                    long alpha = 0xff;
                    if (attraction == 1.0f) {
                        alpha = (CWorld::GetFrame() * 4) % 0x1ff;
                        if (alpha > 0xff)
                            alpha = 0x1ff - alpha;
                    }
                    SetAttractionIcon(true, attraction * 0.5f, (unsigned char)alpha);
                } else {
                    SetAttractionIcon(false, 1.0f, 0xff);
                }
            }
        }
    }
    if (CWorld::GetFrame() - FrameLastCulledGiftsReceived > PHeroMarriageDef->FramesBetweenCullingOldGifts) {
        FrameLastCulledGiftsReceived = CWorld::GetFrame();
        std::map<long,long>::iterator it = GiftsReceived.begin();
        std::map<long,long>::iterator end = GiftsReceived.end();
        std::map<long,long>::iterator next = it;
        while (it != end) {
            ++next;
            long age = CWorld::GetFrame() - it->second;
            if (age > PHeroMarriageDef->SecondsThatGiftsReceivedWillBeRemembered * CMainGameComponent::GetConstantFPS())
                GiftsReceived.erase(it);
            it = next;
        }
    }
}
