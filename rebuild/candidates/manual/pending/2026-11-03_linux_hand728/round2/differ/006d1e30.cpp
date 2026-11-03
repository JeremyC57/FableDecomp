// CTCWife::FrameUpdate() -- the wife's per-frame update: brain swap after marriage / divorce, attitude refresh, divorce / sex-offer /
// gift / house-dressing deeds while she can see the hero, gift-giving value, marriage state in the hero's stats; while courting, the
// love attitude, falling in love and the attraction icon; finally the culling of remembered gifts
// Leaf: ?FrameUpdate@CTCWife@@UAEXXZ
// Grade: DIFFER (3464 v 3484 bytes, 369 masked diff lines; chunk 728 round 2, 10 compiled attempts fu1..fu10 + final regrades). NOT landable.
// Retail: Fable.exe (TLC, VC7.1) @ 0x006D1E30, 3484 bytes, plain `ret`; CTCWife vtable 0x0125ED5C slot 9. Ego_r.pdb 0x007072E0 /
//   FableWin.pdb 0x02485720 ?FrameUpdate@CTCWife@@UAEXXZ.
// Residue (control flow and calls match; register allocation / frame): retail keeps 0 in ebx from the prologue (brain init, null
//   tests) and the hero in ebp for the whole married block (frame 0x2C); this build spills the hero ([esp+10h], frame 0x30). The brain
//   name: retail pushes each literal in its own arm (`push -1; jg; push A; jmp; push B; call ctor`), the ternary here selects into a
//   register (if/else and CCharString-ternary variants tried). The gift-opinion block: retail passes each OpinionValues entry to
//   GFLimitReturn through a stack temporary (an rvalue: a by-value float accessor, likely the inline
//   ?GetRelativeOpinion@CTCCreatureOpinionOfHero@@QBEMW4EOpinion@@_N@Z whose body is unproven) and squares the first term before
//   computing the second. SetMarriageState's state goes through eax (`mov eax,N; push eax`) in retail.
// Shape: IsMarriedToThePlayer's body is in the TU (expanded twice, as retail); CDefinitionManager::GetDef<T>(name) template body in the
//   TU (the CBrainDef copy 0x006D3E80 is this TU's COMDAT), so the brain def stays in edi; GFLimitReturn<T> is the landed inline
//   template (inlined at the opinion terms, called out of line 0x0057EF86 for the married hours); IS_THING_ALIVE<T> declared only
//   (called 0x0043DBD0); std::map erase(iterator) inline (STLport _tree.h: _Rebalance_for_erase 0x0042A34E, free, --count).
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
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // ??$IS_THING_ALIVE@PAVCThingPlayerCreature@@@@YI_NABQAVCThingPlayerCreature@@@Z 0x0043DBD0
template <class T> inline const T& __fastcall GFLimitReturn(const T& value, const T& low, const T& high)
{
    if (value < low)
        return low;
    if (value > high)
        return high;
    return value;
}
extern const unsigned long INVALID_SAMPLE_ID;                                // 0x0129D25C
// The member template CDefinitionManager::GetDef<T>(name) (header text; retail instantiates the CBrainDef copy in this TU: 0x006D3E80,
// after the CTCWife bodies). Its body in the TU lets VC7.1 see the out pointer is not kept, so the brain def stays in a register.
template <class T> bool CDefinitionManager::GetDef(const CCharString& name, CDefPointer<const T>* out) const
{
    CDefPointer<const CDefClassBase> def = GetPDefFromInstantiationName(name);
    if (!def.IsNull()) {
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
// Same-TU body of the out-of-line query 0x006CEC80 (CTCWife.cpp): FrameUpdate expands it twice.
bool CTCWife::IsMarriedToThePlayer() const { return FrameGotMarriedToThePlayer <= CWorld::GetFrame(); }

void CTCWife::FrameUpdate()
{
    if (NeedsToChangeBrain) {
        CDefPointer<const CBrainDef> brain;
        GetDefinitionManager().GetDef(IsMarriedToThePlayer() ? "BRAIN_WIFE" : "BRAIN_DIVORCED_WIFE", &brain);
        static_cast<CThingAICreature*>(ParentThing)->SetNewBrain(brain);
        NeedsToChangeBrain = false;
    }
    if (static_cast<CThingAICreature*>(ParentThing)->IsBrainUpdateZoneActiveThisFrame()) {
        if (AttitudeUpdateCounter % 3 == 0) {
            OpinionAttitude = GetWorld().GetOpinionReactionManager().GetThingAttitudeToPlayer(*ParentThing);
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
        if (hero && !hero->IsDead() && ParentThing->GetTC(&perceive)) {
            if (perceive->GetPerceivedThing() == hero && perceive->IsAwareOfThing()) {
                CTCHeroOpinionDeedLog* deed_log;
                hero->GetTC(&deed_log);
                if (DivorceDeedPending) {
                    DivorceDeedPending = false;
                    deed_log->PostDeed((EOpinionDeedType)0x59, hero->GetPos(), 0, static_cast<CThingAICreature*>(ParentThing), (EOpinionPostDeedType)1, false, false, 0, false, 0);
                    return;
                }
                if (DivorceWarningPending) {
                    DivorceWarningPending = false;
                    FrameLastGaveDivorceWarning = GetWorld().GetFrame();
                    deed_log->PostDeed((EOpinionDeedType)0x58, hero->GetPos(), 0, static_cast<CThingAICreature*>(ParentThing), (EOpinionPostDeedType)1, false, false, 0, false, 0);
                }
                const CTCVolumeContainmentTracker* tracker;
                hero->PeekTC(&tracker);
                if (OpinionAttitude == (EOpinionAttitudeType)13 && tracker->GetPThingInside() &&
                    tracker->GetPThingInside() == static_cast<CThingAICreature*>(ParentThing)->GetHomeBuilding()) {
                    if (FrameEnteredLoveWithHusbandPresentAtHome == 0x7fffffff) {
                        FrameEnteredLoveWithHusbandPresentAtHome = GetWorld().GetFrame();
                        FrameLastGaveSexOffer = (long)(FrameEnteredLoveWithHusbandPresentAtHome - CMainGameComponent::GetConstantFPS() * PHeroMarriageDef->SecsInLoveAndHusbandPresentBetweenSexOffers);
                    } else if ((float)(GetWorld().GetFrame() - FrameEnteredLoveWithHusbandPresentAtHome) > CMainGameComponent::GetConstantFPS() * PHeroMarriageDef->SecsInLoveAndHusbandPresentBeforeSexOffered &&
                               (float)(GetWorld().GetFrame() - FrameLastGaveSexOffer) > CMainGameComponent::GetConstantFPS() * PHeroMarriageDef->SecsInLoveAndHusbandPresentBetweenSexOffers) {
                        FrameLastGaveSexOffer = GetWorld().GetFrame();
                        deed_log->PostDeed((EOpinionDeedType)0x5a, hero->GetPos(), 0, static_cast<CThingAICreature*>(ParentThing), (EOpinionPostDeedType)1, false, false, 0, false, 0);
                    }
                } else {
                    FrameEnteredLoveWithHusbandPresentAtHome = 0x7fffffff;
                }
                if ((float)(frame - FrameLastAwareOfHusband) > CMainGameComponent::GetConstantFPS() * PHeroMarriageDef->SecondsTimeSinceSeeingYouMinThreshold) {
                    deed_log->PostDeed((EOpinionDeedType)0x51, hero->GetPos(), 0, static_cast<CThingAICreature*>(ParentThing), (EOpinionPostDeedType)1, false, false, 0, false, 0);
                    FrameLastAwareOfHusband = frame;
                }
                CThingAICreature* wife = static_cast<CThingAICreature*>(ParentThing);
                const CTCBuyableHouse* house;
                if (wife->GetHomeBuilding() && wife->GetHomeBuilding()->PeekTC(&house) && hero->PeekTC(&tracker) &&
                    wife->GetHomeBuilding() == tracker->GetPThingInside() &&
                    house->GetCurrentDressLevel() > HouseDressingLevelLastCommentedOn) {
                    deed_log->PostDeed((EOpinionDeedType)0x56, hero->GetPos(), 0, static_cast<CThingAICreature*>(ParentThing), (EOpinionPostDeedType)1, false, false, 0, false, 0);
                    HouseDressingLevelLastCommentedOn = house->GetCurrentDressLevel();
                }
                if (perceive->CanSeeOrSmellPerceivedThing() && frame > FrameToCheckAppearanceChanges) {
                    CheckAppearanceChanges(*hero);
                    FrameToCheckAppearanceChanges = frame + PHeroMarriageDef->SecondsBetweenCheckingPlayersAppearance * CMainGameComponent::GetConstantFPS();
                }
            } else {
                FrameEnteredLoveWithHusbandPresentAtHome = 0x7fffffff;
            }
            C3DVector offset = hero->GetPos() - ParentThing->GetPos();
            float distance = (float)sqrt(offset.GetSquaredMagnitude2D());
            if (distance < perceive->GetExtendedSightRadius())
                FrameLastAwareOfHusband = frame;
        }
        CTCCreatureOpinionOfHero* opinion = 0;
        ParentThing->GetTC(&opinion);
        long gift_period = PHeroMarriageDef->SecondsBetweenEvaluatingGiftOpinion * CMainGameComponent::GetConstantFPS();
        if (frame - FrameLastEvaluatedGiftOpinion > gift_period) {
            float a = 1.0f - GFLimitReturn(opinion->RelativeOpinion.OpinionValues[3], -1.0f, 1.0f);
            float b = 1.0f - GFLimitReturn(opinion->RelativeOpinion.OpinionValues[4], -1.0f, 1.0f);
            float distance = (float)sqrt(a * a + b * b);
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
            if (stats->PeekMarriageInfo(ParentThing->GetUniqueID(), info) && info.State != (EMarriageState)4) {
                if ((float)(CWorld::GetFrame() - FrameGotMarriedToThePlayer) < CMainGameComponent::GetConstantFPS() * PHeroMarriageDef->SecondsJustMarriedDuration) {
                    JustMarried = true;
                    stats->SetMarriageState(static_cast<CThingAICreature*>(ParentThing), (EMarriageState)1);
                } else {
                    JustMarried = false;
                    switch (OpinionAttitude) {
                    case 13: case 14:
                        stats->SetMarriageState(static_cast<CThingAICreature*>(ParentThing), (EMarriageState)3);
                        break;
                    case 15: case 16: case 17:
                        stats->SetMarriageState(static_cast<CThingAICreature*>(ParentThing), (EMarriageState)2);
                        break;
                    }
                }
            }
        }
    } else {
        CThingAICreature* wife = static_cast<CThingAICreature*>(ParentThing);
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
                    ParentThing->GetTC(&opinion);
                    opinion->ForcedAttitude = (EOpinionAttitudeType)12;
                    if (!HasBeenInLoveWithPlayer) {
                        HasBeenInLoveWithPlayer = true;
                        CThingPlayerCreature* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
                        CTCHeroStats* stats;
                        if (IS_THING_ALIVE(hero) && hero->GetTC(&stats)) {
                            long lovers = ++stats->NumLoversAcquired;
                            CTCHeroOnlineScoreboard* scoreboard;
                            hero->GetTC(&scoreboard);
                            scoreboard->UpdateScoreForActivity((CTCHeroOnlineScoreboard::EOnlineScoreboardActivity)0xb, (float)lovers, false);
                        }
                        opinion->SetPersonalityOverride(PHeroMarriageDef->FianceeOpinionPersonalityDef, false);
                        CTCSoundPlayer* sound_player;
                        if (ParentThing->GetTC(&sound_player)) {
                            unsigned long sample = sound_player->PlayAttachedCriteria(PHeroMarriageDef->FallenInLoveSoundCriteria, true, false, false, -1);
                            if (sample != INVALID_SAMPLE_ID)
                                sound_player->SetSpecificSampleVolume(sample, PHeroMarriageDef->FallenInLoveSoundGain);
                        }
                    }
                } else {
                    CTCCreatureOpinionOfHero* opinion = 0;
                    ParentThing->GetTC(&opinion);
                    if (opinion->ForcedAttitude == (EOpinionAttitudeType)12)
                        opinion->ForcedAttitude = (EOpinionAttitudeType)18;
                }
            }
            const CTCCreatureOpinionOfHero* opinion = 0;
            ParentThing->PeekTC(&opinion);
            if (ParentThing->HasTC(TCI_EMOTEICON)) {
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
        while (it != GiftsReceived.end()) {
            std::map<long,long>::iterator next = it;
            ++next;
            if (CWorld::GetFrame() - it->second > PHeroMarriageDef->SecondsThatGiftsReceivedWillBeRemembered * CMainGameComponent::GetConstantFPS())
                GiftsReceived.erase(it);
            it = next;
        }
    }
}
