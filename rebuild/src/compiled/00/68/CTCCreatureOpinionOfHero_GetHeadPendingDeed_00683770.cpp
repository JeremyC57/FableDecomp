// 00683770 CTCCreatureOpinionOfHero::GetHeadPendingDeed
// Leaf: ?GetHeadPendingDeed@CTCCreatureOpinionOfHero@@QAEPBVCOpinionDeed@@XZ
// Flags: none
#define FABLE_RETAIL_HEAD_DEED_PASS29
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_HEADPENDINGDEED
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_WITNESSEDCRIME
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_STLP_LIST_REVERSE
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_DEEDS
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_COMPLETE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_LIST_POPBACK
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_DEEDS
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/EOpinionDeedType.h"
#include "engine/CTCCreatureOpinionOfHero.h"

#include "stlport/fable_map.h"
#include "engine/COpinionDeed.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/COpinionDeedEffectsDef.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CTCVillageMember.h"
#include "engine/CTCVillage.h"
#include "engine/NCreatureMode.h"
const COpinionDeed* CTCCreatureOpinionOfHero::GetHeadPendingDeed()
{
    const COpinionDeed* preturn_deed = 0;
    CThingPlayerCreature* player = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
    if (player) {
        const CTCHeroOpinionDeedLog* ptc_deedlog;
        player->PeekTC(&ptc_deedlog);
        long frames_since_reacted = GetWorld().GetFrame() - LastOpinionReactionFrame;
        while (!preturn_deed && !PendingDeeds.empty()) {
            const CTCCreatureModeManager* ptc_mode_manager;
            const CTCVillageMember* tc_village_member;
            preturn_deed = ptc_deedlog->GetDeedFromHandle(*PendingDeeds.begin());
            if (!preturn_deed) {
                PendingDeeds.pop_front();
            }

            else if ((float)CMainGameComponent::GetConstantFPS() * ptc_deedlog->PeekPDeedEffectsDef(preturn_deed->DeedType)->PreReactionDelayIgnoreOnDelay > (float)frames_since_reacted) {
                KnownDeeds[preturn_deed->DeedType].push_front(preturn_deed->DeedHandle);
                PendingDeeds.pop_front();
                preturn_deed = 0;
            }
            else if (preturn_deed->WasSearchingForWitnesses && !preturn_deed->IsStillHappening) {
                KnownDeeds[preturn_deed->DeedType].push_front(preturn_deed->DeedHandle);
                PendingDeeds.pop_front();
                preturn_deed = 0;
            }
            else if (preturn_deed->PTarget.Get() != &GetParentThing() && !preturn_deed->IsCrime
                    && !ptc_deedlog->PeekPDeedEffectsDef(preturn_deed->DeedType)->AllowIndirectReactWhileCarrying
                    && GetParentThing().PeekTC(&ptc_mode_manager)
                    && (ptc_mode_manager->GetCurrentMode().GetType() == NCreatureMode::MODE_CARRYING_GOODS_STAND || ptc_mode_manager->GetCurrentMode().GetType() == NCreatureMode::MODE_HOLD_JUG
                        || ptc_mode_manager->GetCurrentMode().GetType() == NCreatureMode::MODE_ARMS_OFF_TABLE || ptc_mode_manager->GetCurrentMode().GetType() == NCreatureMode::MODE_SIT_TAVERN_STOOL
                        || ptc_mode_manager->GetCurrentMode().GetType() == NCreatureMode::MODE_FILL_JUG || ptc_mode_manager->GetCurrentMode().GetType() == NCreatureMode::MODE_SERVE_CUSTOMER)) {
                KnownDeeds[preturn_deed->DeedType].push_front(preturn_deed->DeedHandle);
                PendingDeeds.pop_front();
                preturn_deed = 0;
            }
            else if (preturn_deed->IsCrime && (preturn_deed->DeedType == OPINION_DEED_TYPE_CRIME_ASSAULT || preturn_deed->DeedType == OPINION_DEED_TYPE_CRIME_GBH) && GetParentThing().PeekTC(&tc_village_member)
                && tc_village_member->GetPVillage() && IsGuardOnLevel() && !GetParentThing().HasTC(TCI_SHOP_KEEPER)
                && !tc_village_member->GetPVillage()->HasCrimeBeenReported(preturn_deed->DeedHandle)) {
                KnownDeeds[preturn_deed->DeedType].push_front(preturn_deed->DeedHandle);
                PendingDeeds.pop_front();
                preturn_deed = 0;
            }
        }
    }
    return preturn_deed;
}