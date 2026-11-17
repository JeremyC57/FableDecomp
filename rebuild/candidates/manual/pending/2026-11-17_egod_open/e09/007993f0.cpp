// CTCShopKeeper::WitnessedCrime(const COpinionDeed& deed, long deed_handle) -- a shopkeeper who saw a crime in (or, for a stall, near) his work building raises the alarm (a shop) or shouts for the guards (a stall) when a guard exists, reports the crime to his village (and the village's opinion of the hero), and has the village start an urgent security sweep for a severity 2 or 3 deed
// Grade: DIFFER(698v698) -- x87 fsub order 0x007994A3..0x007994AA only (dx before dy in retail; today's shared GFGetWorld2DDistance body computes dy first). RELOCATION_MATCH with the FableWin-sized dx-first body (probe/hdr2), which breaks landed 0x009711D0: coordinator decision.
// FITTED: GFGetWorld2DDistance(building pos, keeper pos) for the stall distance (picked by grading over (a - b).GetMagnitude2D() and a written-out sqrt); the implicit const char* -> CCharString speech argument (retail re-reads the temporary at 0x00799589); the early-return shape inside Ego_d's two if-blocks.
// Leaf: ?WitnessedCrime@CTCShopKeeper@@QAEXABVCOpinionDeed@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007993F0, 698 bytes (`ret 8`).
// Ego_d.pdb (tc_shopkeeper.obj): parameters deed / deed_handle; ptc_village_member at function scope; { ptc_village { ptc_deed_log,
//   tc_shop, creature, guard, player, pdeed_effects_def; { MAX_DISTANCE_FOR_ALARM } { tc_talk } { ptc_village_opinion } } }.
// Callees: LowerBound 0x0040F020 (TC lookups 0x23 / 0x0F / 0x4F / 0x51), CTCVillageMember::GetPVillage 0x007EE540, CTCShopKeeper::GetShopTC
//   0x00798F50, CBaseIntelligentPointer::GetPItem 0x00A01B50 (WorkBuilding), CThing::GetPos 0x004C73D0, CThingBuilding::IsPointInside
//   0x0082D000, CTCBase::PeekThingSearchTools 0x00686D90, CThingSearchTools::GetPFirstThing<CThingFilter_HasTC,CThingAICreature> 0x00799C40
//   (same TU), CTCShop::SetAlarm 0x00773C90, CTCTalk::StopCurrentSpeech 0x006E2E60 / PlayAISpeech 0x006E3B30, CCharString 0x0099EBF0 /
//   ~ 0x0099EAE0, CTCVillage::HasCrimeBeenReported 0x0065914B / ReportCrime 0x0065CB6B / StartSecuritySweep 0x00659C32,
//   CTCCreatureOpinionOfHero::KnowsOfDeed 0x00681AE0 / InformOfDeed 0x00683590, CTCBase::GetPlayerManager 0x00686D40,
//   CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetPControlledCreature 0x00487DC0,
//   CTCHeroOpinionDeedLog::PeekPDeedEffectsDef 0x006C5630.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_GFGETWORLD2DDISTANCE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_MANAGERACCESS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING
#define FABLE_RETAIL_CTHINGAICREATURE_SAVE
#define FABLE_RETAIL_CTHINGAICREATURE_WORKBUILDING
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_WITNESSEDCRIME
#define FABLE_RETAIL_CTCSHOP
#define FABLE_RETAIL_CTCSHOP_STALL_STOCK_ACCESSORS
#define FABLE_RETAIL_CTCSHOPKEEPER_SHOP
#define FABLE_RETAIL_CTCSHOPKEEPER_WITNESSEDCRIME
#define FABLE_RETAIL_CTCTALK
#define FABLE_RETAIL_CTCTALK_AISPEECHTEXT
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG_DEEDS
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_INFORM
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_INFORM_HANDLE
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTTHING
#define FABLE_RETAIL_IS_THING_ALIVE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/ETCInterfaceType.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CThingBuilding.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CTCVillage.h"
#include "engine/CTCVillageMember.h"
#include "engine/CTCShop.h"
#include "engine/CTCShopKeeper.h"
#include "engine/CTCTalk.h"
#include "engine/COpinionDeed.h"
#include "engine/COpinionDeedEffectsDef.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/CTCCreatureOpinionOfHero.h"
#include "engine/CThingFilter_HasTC.h"
#include "engine/CThingSearchTools.h"
#include "engine/ThingAlive.h"
#include "engine/GFWorldDistance.h"
#include <math.h>
namespace NSpeechGainManager { enum EDialogueLayer { DIALOGUE_LAYER_FOREGROUND = 0, DIALOGUE_LAYER_MIDGROUND = 1, DIALOGUE_LAYER_BACKGROUND = 2, DIALOGUE_LAYER_LAST = 3 }; }

inline CThingBuilding* CThingAICreature::GetWorkBuilding() const { return WorkBuilding.Get(); }

void CTCShopKeeper::WitnessedCrime(const COpinionDeed& deed, long deed_handle)
{
    CTCVillageMember* ptc_village_member;
    if (GetParentThing().GetTC(TCI_VILLAGE_MEMBER, &ptc_village_member))
    {
        CTCVillage* ptc_village = ptc_village_member->GetPVillage();
        if (ptc_village)
        {
            CTCShop* tc_shop = GetShopTC();
            if (tc_shop == 0)
                return;
            if (GetParentThing().GetType() != 1)
                return;
            CThingAICreature* creature = static_cast<CThingAICreature*>(&GetParentThing());
            if (!IS_THING_ALIVE(creature->GetWorkBuilding()))
                return;
            if (tc_shop->IsStallLike())
            {
                const float MAX_DISTANCE_FOR_ALARM = 10.0f;
                if (GFGetWorld2DDistance(creature->GetWorkBuilding()->GetPos(), creature->GetPos()) > MAX_DISTANCE_FOR_ALARM)
                    return;
                if (tc_shop->GetStockState() == CTCShop::STOCKED_NO)
                    return;
            }
            else if (!creature->GetWorkBuilding()->IsPointInside(creature->GetPos()))
                return;
            CThingAICreature* guard;
            if (!PeekThingSearchTools().GetPFirstThing(CThingFilter_HasTC(TCI_GUARD), &guard))
                return;
            if (&GetParentThing() == &tc_shop->GetParentThing())
                return;
            if (!tc_shop->IsStallLike())
                tc_shop->SetAlarm(true);
            else
            {
                CTCTalk* tc_talk;
                if (GetParentThing().GetTC(TCI_TALK, &tc_talk))
                {
                    tc_talk->StopCurrentSpeech();
                    tc_talk->PlayAISpeech("TEXT_AI_CRIME_RUN_TO_GUARDS", false, NSpeechGainManager::DIALOGUE_LAYER_MIDGROUND, false, true);
                }
            }
            if (!ptc_village->HasCrimeBeenReported(deed_handle))
            {
                ptc_village->ReportCrime(deed_handle);
                CTCCreatureOpinionOfHero* ptc_village_opinion;
                if (ptc_village->GetParentThing().GetTC(TCI_CREATURE_OPINION_OF_HERO, &ptc_village_opinion))
                {
                    if (!ptc_village_opinion->KnowsOfDeed(deed_handle, deed.DeedType, true))
                        ptc_village_opinion->InformOfDeed(deed_handle, false);
                }
            }
            CThingPlayerCreature* player = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
            CTCHeroOpinionDeedLog* ptc_deed_log;
            player->GetTC(TCI_HERO_OPINION_DEED_LOG, &ptc_deed_log);
            CDefPointer<const COpinionDeedEffectsDef> pdeed_effects_def = ptc_deed_log->PeekPDeedEffectsDef(deed.DeedType);
            if (pdeed_effects_def->CrimeSeverity == 2 || pdeed_effects_def->CrimeSeverity == 3)
                ptc_village->StartSecuritySweep(true);
        }
    }
}
