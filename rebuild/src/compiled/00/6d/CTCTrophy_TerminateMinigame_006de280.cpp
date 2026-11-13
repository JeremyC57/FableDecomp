// CTCTrophy::TerminateMinigame() -- ends the trophy minigame: a creature carrying the trophy sheathes it (otherwise it returns to the inventory and the hero is the holder), every gained or available witness gets its opinion reaction back (gained ones lose the trophy personality override, available ones their emote), then the holder's score (gained minus still available, at least 0) is shown and kept as the best, and beating the previous best rewards renown, a deed and the new-high-score sound and text
// Leaf: ?TerminateMinigame@CTCTrophy@@AAEXXZ
// Flags: /d2inlT77
// Inline-budget: /d2inlT77 (owner decision 2026-11-08, preference 2) -- this TU has CVectorMap::LowerBound's body inline
//   (FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND) and retail expands it at exactly one GetTC site, the holder's hero stats after the
//   witness loops (0x006DE4FE..0x006DE53B, `cmp [esi+eax*8],4`), while the opinion / emote / deed-log / sound-player sites call
//   0x0040F020, and it expands CTCTrophy::PlaySoundW at both score sites (0x006DE6BC / 0x006DE79B: presence word tested twice, then
//   PlayAttachedCriteria). The c2 inline trace (hook c2.dll 0x1078D1AA, tools/inl.sh) with two plain `for` loops gives the emote and
//   hero-stats sites the same score (68 at f 3.125), so no flag or budget can split them; with the available-witness loop behind its
//   own `if (it != end)` guard (no code: c2 merges it with the loop guard) the sites score emote 77 (f 1.5625), hero stats 74 (f 1.875),
//   both PlaySoundW 76 (f 1.875), deed log / sound player 100, gained-witness opinion 95; the default T74 expands none of them, T77
//   expands exactly retail's three (hero stats + both PlaySoundW) and leaves every other decision of the function unchanged.
//   Control-flow alternatives traced first (no grade): for / while / do-while / for(;;)+break / post-increment / `!(it == end)` /
//   `if (!empty())` (adds a test) / a guard round both loops -- none puts emote below and hero stats above the threshold.
// Retail: Fable.exe 0x006DE280, 1581 bytes (`ret` at 0x006DE8AC; the next function, OnShowTrophyInitialise, starts at 0x006DE8B0).
//   Name: Ego_r.pdb / FableWin.pdb ?TerminateMinigame@CTCTrophy@@AAEXXZ (private), donor order StartMinigame 0x0070BA30 /
//   TerminateMinigame 0x0070BC50 / OnShowTrophyInitialise 0x0070C2C0 = retail 0x006DE080 / 0x006DE280 / 0x006DE8B0; GlobalUpdate
//   0x006DEEE0 calls it on a click-past while WaitingForTerminate.
// Callees: CTCCarryable::GetPCarriedByThing 0x00705180; CThingCreatureBase::FinishCurrentAction 0x00662650 / SetCurrentAction
//   0x006644F0; CCreatureAction_SheatheItemToInventory ctor 0x0069FFD0 / dtor 0x0048C950; CTCBase::GetPlayerManager 0x00686D40 /
//   GetWorld 0x00686A70; CPlayerManager::GetMainPlayer 0x00449970; CPlayer::GetPControlledCreature 0x00487DC0;
//   CTCInventoryItem::ReturnToInventory 0x005D8E50; CBaseIntelligentPointer::GetPItem 0x00A01B50; CThingAICreature::SetStateGroupEnabled
//   0x008302D0; CTCCreatureOpinionOfHero::ClearPersonalityOverride 0x00682310; CTCEmoteIcon::SetEmoteIcon 0x0071AAB0;
//   CVectorMap::LowerBound 0x0040F020; NGameText::CDataBank::GetTextBySymbol 0x009C95E0; NGameText::ConstructString (J) 0x009C7B70 /
//   (JJ) 0x009C7B80 / (JJJ) 0x009C7BA0; StartDialogBox 0x006DD7E0; CTCHeroStats::AddRenown 0x0057D8BE; CThing::GetPos 0x004C73D0;
//   CTCHeroOpinionDeedLog::PostDeed 0x006C6990; CTCSoundPlayer::PlayAttachedCriteria 0x0066BD00 (inside the expanded PlaySoundW);
//   NPlayerGui::CDrawQuestInfo::DisplayMiniGameInfo 0x00654593; CCharString / CWideString ctors / dtors.
// Inline in retail (FableWin.pdb emits their COMDATs right after TerminateMinigame, 0x0249D470 / 0x0249D490):
//   CWorld::SetRegionLoadConfirmationAsGiven (the final `mov byte ptr [eax+130h],0`) and CTCHeroStats::SetBestTrophyWitnesses (+0xDC).
// Shape: the holder is assigned inside the creature test (its frame home store sits after the PCurrentAction load, 0x006DE2E5) and
//   again by the fallback; `sheathed` makes the failed null / type tests jump straight to the fallback; the gained-witness loop caches
//   end() in ebp with `this` kept in edi, the available-witness loop keeps end() in the frame word the uninitialised
//   GetTC out-pointers share (GETTC_EXPANDED: the emote pointer is read from that word when the presence bit is clear, 0x006DE422).
// Interfaces: TCI_CARRYABLE 71, TCI_INVENTORY_ITEM 26, TCI_CREATURE_OPINION_OF_HERO 79, TCI_EMOTEICON 69, TCI_HERO_STATS 4,
//   TCI_HERO_OPINION_DEED_LOG 81, TCI_SOUND_PLAYER 16. Deeds 43 / 45 (OPINION_DEED_TYPE_SCRIPT_ACTION_ANNOYING_LARGE / _NICE_LARGE),
//   EOpinionPostDeedType 2 (value only), EEmotePriority 2 (value only), MINIGAME_TROPHY 6. Thing type 2 = creature (as GlobalUpdate).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CVECTORMAP_INLINE_LOWERBOUND
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_FINISHACTION
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMATION
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_STATEGROUPENABLED
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_INTERRUPTION
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTCCARRYABLE_NONCONST_CARRIER
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO_CLEARPERSONALITY
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CTCHEROSTATS
#define FABLE_RETAIL_CTCHEROSTATS_ADDRENOWN
#define FABLE_RETAIL_CTCHEROSTATS_TROPHYWITNESSES
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_QUEST_DRAW
#define FABLE_RETAIL_CDRAWQUESTINFO_ORACLESYMBOLS
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_REGIONLOADCONFIRMATION
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_NGAMETEXT_CONSTRUCTSTRING_LONGS
#define FABLE_RETAIL_NGAMETEXT_CONSTRUCTSTRING_LONGS3
#define FABLE_RETAIL_CTROPHYDEF
#define FABLE_RETAIL_STLP_LIST
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/ETCInterfaceType.h"
#include "engine/EOpinionDeedType.h"
#include "engine/CDefinitionManager.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CThingAICreature.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_SheatheItemToInventory.h"
#include "engine/CTCCarryable.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/CTCCreatureOpinionOfHero.h"
#include "engine/CTCEmoteIcon.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/CTCHeroStats.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "engine/NPlayerGui_CDrawQuestInfo.h"
#include "engine/CPlayerGui.h"
#include "engine/CWorld.h"
#include "fable_game.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/CTrophyDef.h"
#include "engine/CTCTrophy.h"

extern unsigned long g_InvalidSoundHandle;                               // 0x0129D25C (the established descriptive spelling)

// CTCTrophy::PlaySoundW 0x006DDCD0 (landed): the body of this TU, defined ahead of the row as in retail (the chunk 2007 rows' form),
// where VC7.1 expands it at both score sites.
unsigned long CTCTrophy::PlaySoundW(const CCharString& name)
{
    CTCSoundPlayer* sound;
    if (ParentThing->HasTC(TCI_SOUND_PLAYER) && ParentThing->GetTC(TCI_SOUND_PLAYER, &sound))
        return sound->PlayAttachedCriteria(name, true, false, false, -1);
    return g_InvalidSoundHandle;
}

void CTCTrophy::TerminateMinigame()
{
    MinigameInProgress = false;
    CTCCarryable* carryable;
    ParentThing->GetTC(TCI_CARRYABLE, &carryable);
    CThing* holder;
    CThing* carrier = carryable->GetPCarriedByThing();
    bool sheathed = false;
    if (carrier && carrier->GetType() == 2) {
        holder = carrier;
        CThingCreatureBase* creature = (CThingCreatureBase*)carrier;
        if (creature->PeekPCurrentAction() && creature->PeekPCurrentAction()->GetInterruptionGroupID() <= 0)
            creature->FinishCurrentAction();
        if (creature->SetCurrentAction(CCreatureAction_SheatheItemToInventory(*creature, 0, 100)))
            sheathed = true;
    }
    if (!sheathed) {
        holder = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        CTCInventoryItem* item;
        if (ParentThing->GetTC(TCI_INVENTORY_ITEM, &item))
            item->ReturnToInventory();
    }
    for (std::list<CIntelligentPointer<CThingAICreature> >::iterator gained = WitnessesAchieved.begin(), gained_end = WitnessesAchieved.end();
         gained != gained_end; ++gained) {
        CIntelligentPointer<CThingAICreature>& witness = *gained;
        if (witness.Get() && !witness.Get()->IsDead()) {
            witness.Get()->SetStateGroupEnabled("SG_OPINION_REACTION_ROOT", true);
            CTCCreatureOpinionOfHero* opinion;
            if (witness.Get()->GetTC(TCI_CREATURE_OPINION_OF_HERO, &opinion))
                opinion->ClearPersonalityOverride();
        }
    }
    std::list<CIntelligentPointer<CThingAICreature> >::iterator it = AvailableWitnesses.begin(), end = AvailableWitnesses.end();
    if (it != end) {
        for (; it != end; ++it) {
            CIntelligentPointer<CThingAICreature>& witness = *it;
            if (witness.Get() && !witness.Get()->IsDead()) {
                witness.Get()->SetStateGroupEnabled("SG_OPINION_REACTION_ROOT", true);
                CTCEmoteIcon* emote;
                witness.Get()->GetTC(TCI_EMOTEICON, &emote);
                if (PDef->EmoteIconForAvailableWitness)
                    emote->SetEmoteIcon((CTCEmoteIcon::EEmotePriority)2, 0, 1.0f, 1.0f, 255);
            }
        }
    }
    if (holder && !holder->IsDead()) {
        CTCHeroStats* stats;
        holder->GetTC(TCI_HERO_STATS, &stats);
        long witnesses = WitnessesAchieved.size() - AvailableWitnesses.size();
        if (witnesses < 0)
            witnesses = 0;
        if (witnesses > stats->GetBestTrophyWitnesses())
            stats->SetBestTrophyWitnesses(witnesses);
        CWideString text = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_TROPHY_GAME_SCORED");
        NGameText::ConstructString(text, WitnessesAchieved.size(), AvailableWitnesses.size(), witnesses);
        StartDialogBox(text, 10, L"", L"", L"");
        WaitingForScore = true;
        if (witnesses > BestWitnessesAheadToDate) {
            long renown = (witnesses - BestWitnessesAheadToDate) * PDef->RenownGainedPerWitnessAhead;
            stats->AddRenown(renown);
            CTCHeroOpinionDeedLog* deed_log;
            if (!holder->IsDead() && holder->GetTC(TCI_HERO_OPINION_DEED_LOG, &deed_log))
                deed_log->PostDeed(PDef->EvilTrophy ? OPINION_DEED_TYPE_SCRIPT_ACTION_ANNOYING_LARGE : OPINION_DEED_TYPE_SCRIPT_ACTION_NICE_LARGE,
                                   holder->GetPos(), 0, 0, (EOpinionPostDeedType)2, false, false, 0, false, 0);
            PlaySoundW(PDef->SoundEndNewHighScore);
            CWideString beaten = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_TROPHY_GAME_BEATEN");
            NGameText::ConstructString(beaten, BestWitnessesAheadToDate, renown);
            StartDialogBox(beaten, 10, L"", L"", L"");
            BestWitnessesAheadToDate = witnesses;
        } else {
            PlaySoundW(PDef->SoundEndNoNewScore);
            CWideString failed = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_TROPHY_GAME_FAILED");
            NGameText::ConstructString(failed, BestWitnessesAheadToDate);
            StartDialogBox(failed, 10, L"", L"", L"");
        }
    }
    CPlayerGui::Get().GetQuestInfo().DisplayMiniGameInfo(false, MINIGAME_TROPHY, 0);
    GetWorld().SetRegionLoadConfirmationAsGiven();
}