// CTCTrophy::OnShowTrophyPerformAction(CThing& thing) -- while the trophy minigame runs and the hero shows the trophy: witnesses already gained who perceive and notice it get bored of it (deed, personality override, opinion reaction); available witnesses who notice it move to the gained list (emote cleared, time bonus, gained-witness sound, a good or evil deed by their creature group, personality override unless married to / divorced from the hero, opinion reaction); when none are left, the all-witnesses sound and dialog
// Leaf: ?OnShowTrophyPerformAction@CTCTrophy@@QAEXAAVCThing@@@Z
// Retail: Fable.exe 0x006DE8C0, 1460 bytes (`ret 4` at 0x006DEE71). Ego_r.pdb / FableWin.pdb ?OnShowTrophyPerformAction@CTCTrophy@@QAEXAAVCThing@@@Z;
//   the only caller is CCreatureActionShowTrophy::PerformAction 0x008455C0 (call at 0x00845823). The CThing& argument is not read.
// Callees: CTCCarryable::GetPCarriedByThing 0x00705180; CBaseIntelligentPointer::GetPItem 0x00A01B50 / ctor 0x00A01B10 / SetPItem
//   0x00A01B90 / dtor 0x00A01C10; CTCPerceiveThing::IsPlayerIgnoreFOVLineOfSight 0x007448B0; NCreatureAI::IsAwareOfThingIndirectly
//   0x00864700; CThing::GetPos 0x004C73D0; CTCHeroOpinionDeedLog::PostDeed 0x006C6990; CTCCreatureOpinionOfHero::SetPersonalityOverride
//   0x006820E0; CThingAICreature::SetStateGroupEnabled 0x008302D0; list<CIntelligentPointer<CThingAICreature> >::push_back 0x006604EB;
//   free 0x00BFEA14 (the inline erase); CTCEmoteIcon::SetEmoteIcon 0x0071AAB0; CDefClassBase::GetSubDef<CCreatureDef> 0x00485815;
//   CTCWife::IsMarriedToThePlayer 0x006CEC80; CTCSoundPlayer::PlayAttachedCriteria 0x0066BD00; NGameText::CDataBank::GetTextBySymbol
//   0x009C95E0; NGameText::ConstructString(CWideString&, long) 0x009C7B70; StartDialogBox 0x006DD7E0; CCharString / CWideString.
// Shape: both witness loops cache end() and carry a `next` iterator initialised from `it` and advanced first in the body
//   (`for (it = begin(), end = end(), next = it; it != end; it = next) { ++next; ... }`): retail keeps end in ebx and next in memory
//   and enters the aligned loop head through a jump (0x006DE95D..0x006DE970); erase(it) / push_back(CIntelligentPointer(creature)) as
//   STLport does (push_back called out of line, 0x006604EB); GetSubDef<CCreatureDef>'s body visible (noinline) so the def handle's
//   release is `dec [ecx+4]` (0x006DED69); CTCTrophy::PlaySoundW's body in the TU so VC7.1 expands it at the all-witnesses site; the
//   all-witnesses tail is a block (its text shares a frame word with the loops' temporaries; frame 0x30).
// Interfaces: TCI_CARRYABLE 71, TCI_HERO_OPINION_DEED_LOG 81, TCI_PERCEIVE_THING 52, TCI_CREATURE_OPINION_OF_HERO 79, TCI_EMOTEICON 69,
//   TCI_WIFE 215, TCI_SOUND_PLAYER 16. Deeds: OPINION_DEED_TYPE_SHOW_TROPHY_EVIL / _GOOD / _BORED (55 / 56 / 57).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_STATEGROUPENABLED
#define FABLE_RETAIL_CTCCARRYABLE_NONCONST_CARRIER
#define FABLE_RETAIL_CTCHEROOPINIONDEEDLOG
#define FABLE_RETAIL_CTCPERCEIVETHING_SMELL
#define FABLE_RETAIL_CTCCREATUREOPINIONOFHERO
#define FABLE_RETAIL_CTCWIFE
#define FABLE_RETAIL_CTCWIFE_MARRY
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_CCREATUREDEF_DIALOGUEVOICES
#define FABLE_RETAIL_CCREATUREDEF_SCREENINVENTORIES
#define FABLE_RETAIL_CCREATUREDEF_RESPAWNWAIT
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_NGAMETEXT_CONSTRUCTSTRING_LONGS
#define FABLE_RETAIL_CTROPHYDEF
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/ETCInterfaceType.h"
#include "engine/EOpinionDeedType.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefinitionManager.h"
#include "engine/CThing.h"
#include "engine/CThingBaseDef.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CCreatureDef.h"
#include "engine/CTCCarryable.h"
#include "engine/CTCHeroOpinionDeedLog.h"
#include "engine/CTCPerceiveThing.h"
#include "engine/CTCCreatureOpinionOfHero.h"
#include "engine/CTCWife.h"
#include "engine/CTCEmoteIcon.h"
#include "engine/CTCSoundPlayer.h"
#include "fable_game.h"
#include "engine/NGameText_CDataBank.h"
#include "engine/CTrophyDef.h"
#include "engine/CTCTrophy.h"

namespace NCreatureAI {
bool __fastcall IsAwareOfThingIndirectly(const CThingAICreature* creature, const CThing* thing, bool check_line_of_sight);   // 0x00864700
}

// list<CIntelligentPointer<CThingAICreature> >::push_back is called out of line in retail (0x006604EB, `call` at 0x006DEBA6).
template <>
void std::list<CIntelligentPointer<CThingAICreature> >::push_back(const CIntelligentPointer<CThingAICreature>& x);

extern unsigned long g_InvalidSoundHandle;                               // 0x0129D25C (the established descriptive spelling)

// CTCTrophy::PlaySoundW 0x006DDCD0 (landed): the body of this TU, defined ahead of the row as in retail, where VC7.1 expands it at
// the all-witnesses site below (0x006DED90..0x006DEDDE: the presence word tested twice, then PlayAttachedCriteria).
unsigned long CTCTrophy::PlaySoundW(const CCharString& name)
{
    CTCSoundPlayer* sound;
    if (ParentThing->HasTC(TCI_SOUND_PLAYER) && ParentThing->GetTC(TCI_SOUND_PLAYER, &sound))
        return sound->PlayAttachedCriteria(name, true, false, false, -1);
    return g_InvalidSoundHandle;
}

void CTCTrophy::OnShowTrophyPerformAction(CThing& thing)
{
    if (!MinigameInProgress)
        return;
    CTCCarryable* carryable;
    ParentThing->GetTC(TCI_CARRYABLE, &carryable);
    CThing* holder = carryable->GetPCarriedByThing();
    CTCHeroOpinionDeedLog* deed_log;
    holder->GetTC(TCI_HERO_OPINION_DEED_LOG, &deed_log);
    for (std::list<CIntelligentPointer<CThingAICreature> >::iterator it = WitnessesAchieved.begin(), end = WitnessesAchieved.end(), next = it;
         it != end; it = next) {
        ++next;
        CIntelligentPointer<CThingAICreature>& witness = *it;
        if (!witness.Get() || witness.Get()->IsDead())
            continue;
        CThingAICreature* creature = witness.Get();
        const CTCPerceiveThing* perceive;
        if (!creature->PeekTC(TCI_PERCEIVE_THING, &perceive) || !perceive->IsPlayerIgnoreFOVLineOfSight()
            || !NCreatureAI::IsAwareOfThingIndirectly(witness.Get(), holder, true))
            continue;
        deed_log->PostDeed(OPINION_DEED_TYPE_SHOW_TROPHY_BORED, holder->GetPos(), creature, creature, (EOpinionPostDeedType)1, false, false,
                           0, false, 0);
        CTCCreatureOpinionOfHero* opinion;
        if (creature->GetTC(TCI_CREATURE_OPINION_OF_HERO, &opinion))
            opinion->SetPersonalityOverride("OPINION_PERSONALITY_TROPHY_WITNESS_BORED", true);
        creature->SetStateGroupEnabled("SG_OPINION_REACTION_ROOT", true);
    }
    for (std::list<CIntelligentPointer<CThingAICreature> >::iterator it = AvailableWitnesses.begin(), end = AvailableWitnesses.end(), next = it;
         it != end; it = next) {
        ++next;
        CIntelligentPointer<CThingAICreature>& witness = *it;
        if (!witness.Get() || witness.Get()->IsDead())
            continue;
        CThingAICreature* creature = witness.Get();
        const CTCPerceiveThing* perceive;
        if (!creature->PeekTC(TCI_PERCEIVE_THING, &perceive) || !perceive->IsPlayerIgnoreFOVLineOfSight()
            || !NCreatureAI::IsAwareOfThingIndirectly(witness.Get(), holder, true))
            continue;
        AvailableWitnesses.erase(it);
        WitnessesAchieved.push_back(CIntelligentPointer<CThingAICreature>(creature));
        CTCEmoteIcon* emote;
        creature->GetTC(TCI_EMOTEICON, &emote);
        if (PDef->EmoteIconForAvailableWitness)
            emote->SetEmoteIcon((CTCEmoteIcon::EEmotePriority)2, 0, 0.0f, 1.0f, 255);
        MinigameCurrentFramesLeft += CMainGameComponent::GetConstantFPS() * PDef->TimeBonusPerWitness;
        PendingGainedWitnessSounds++;
        CDefPointer<const CCreatureDef> creature_def;
        bool bandit = creature->PeekBaseDef().GetSubDef(&creature_def) && !creature_def.IsNull() && creature_def->CreatureGroup == 4;
        deed_log->PostDeed(PDef->EvilTrophy == bandit ? OPINION_DEED_TYPE_SHOW_TROPHY_GOOD : OPINION_DEED_TYPE_SHOW_TROPHY_EVIL,
                           holder->GetPos(), creature, creature, (EOpinionPostDeedType)1, false, false, 0, false, 0);
        CTCCreatureOpinionOfHero* opinion;
        if (creature->GetTC(TCI_CREATURE_OPINION_OF_HERO, &opinion)) {
            const CTCWife* wife;
            if (!creature->PeekTC(TCI_WIFE, &wife) || (!wife->IsMarriedToThePlayer() && !wife->DivorcedHero))
                opinion->SetPersonalityOverride(PDef->EvilTrophy ? "OPINION_PERSONALITY_TROPHY_WITNESS_EVIL"
                                                                 : "OPINION_PERSONALITY_TROPHY_WITNESS_GOOD", true);
        }
        creature->SetStateGroupEnabled("SG_OPINION_REACTION_ROOT", true);
        NumberOfWitnessesLeft--;
    }
    if (NumberOfWitnessesLeft <= 0) {
        PlaySoundW(PDef->SoundGotAllWitnesses);
        CWideString text = CMainGameComponent::Get()->GetTextBank().GetTextBySymbol("TEXT_GUI_TROPHY_ALL_WITNESSES");
        NGameText::ConstructString(text, MinigameStartingWitnessCount);
        StartDialogBox(text, 10, L"", L"", L"");
        WaitingForTerminate = true;
    }
}