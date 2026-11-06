// NScript::CV_GuildMasterScript::GetGuildMasterSpeech() -- the guildmaster's line for the current gameflow position (PostSavePosition switch; quest-card / SeenAbbeyMotherAtGuild / hero-souls sub-choices), swapped one time in four for an unsaid info line when it repeats the last line said; remembers and returns it
// Leaf: ?GetGuildMasterSpeech@CV_GuildMasterScript@NScript@@QAE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E91F20, 2522 bytes, thiscall `ret 4` (hidden CCharString result); code ends at 0x00E924E9, then five
//   dword jump tables + byte index tables (0x00E924EC..0x00E928F9) for the sparse EGameflowPosition switch.
// Identity (two sources): Ego_r.pdb / FableWin.pdb name set ?GetGuildMasterSpeech@CV_GuildMasterScript@NScript@@QAE?AVCCharString@@XZ;
//   TEXT_QST_081_* literals (quest 081 = the guildmaster), Ego_r CV_GuildMasterScript {GuildMasterDialogue bool[5] +0x5c, LastDialogueSaid
//   +0x64} = retail +0x48 / +0x50 (CScriptBase 0x14 smaller in retail).
// Callees: CCharString(const char*, long) 0x0099EBF0, CCharString() 0x0099E4B0, copy 0x0099EC30, ~CCharString 0x0099EAE0, operator=(const
//   char*) 0x0099EFE0, operator=(const CCharString&) 0x0099EFB0, CBasicString<char>::Compare 0x00411570 (inline operator== chain),
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940, rand 0x00BFEB16; CGameScriptInterfaceBase slots 205 GetHeroTitle, 284
//   IsQuestActive, 286 IsQuestCompleted (ghidra_out/cgsi_slots_aligned.tsv).
// Data: CQ_SunnyvaleMasterData PostSavePosition +0x04 (EGameflowPosition), SeenAbbeyMotherAtGuild +0x60, CurrentHeroSoulsPosition +0x70
//   (EPlaceInHeroSouls; an if-chain in retail, compared 0..7 in order); every case value is an EGP_ enumerator of the shared header.
// Shape: the first local is the terminate-path return value, a CCharString("") temporary copied by value (the ctor's eax is pushed to
//   the copy ctor, then the temporary dies): written as the inline pass-through CopyString (descriptive; no retail symbol, it is
//   expanded) because every direct spelling (copy-init, cast, nested temporary) is elided by VC7.1. Each `speech = <literal>; break;`
//   arm merges into one operator= call; the unknown positions and the past-the-end hero-souls state return a fresh "".
// Jump tables: all 29 dword entries were paired with their case arms against retail and the five byte index tables are identical.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CLEVELSCRIPTBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#include <stdlib.h>
#include "fable_string.h"
#include "engine/CLevelScriptBase.h"
#include "engine/CGameScriptInterfaceBase.h"
#include "engine/CQ_SunnyvaleMasterData.h"
static inline CCharString CopyString(const CCharString& text)   // by value: the copy VC7.1 keeps (no elision through a reference)
{
    return text;
}
namespace NScript {
class CV_GuildMasterScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    bool GuildMasterDialogue[5];                            // +0x48 (PDB +0x5c)
    CCharString LastDialogueSaid;                           // +0x50 (PDB +0x64)
    CCharString GetGuildMasterSpeech();
};
}
CCharString NScript::CV_GuildMasterScript::GetGuildMasterSpeech()
{
    CCharString nothing = CopyString(CCharString("", -1));
    CCharString speech;
    switch (MasterScriptData->PostSavePosition) {
    case EGP_TRAINING: speech = "TEXT_QST_081_TRAINING"; break;
    case EGP_WAITING_FOR_WASP_BOSS: speech = "TEXT_QST_081_WAITING_FOR_WASP_BOSS"; break;
    case EGP_WASP_BOSS: speech = "TEXT_QST_081_WASP_BOSS"; break;
    case EGP_MAZE_MEETING: speech = "TEXT_QST_081_MAZE_MEETING"; break;
    case EGP_WAITING_FOR_ORCHARD_FARM: speech = "TEXT_QST_081_WAITING_FOR_ORCHARD_FARM"; break;
    case EGP_ORCHARD_FARM: speech = "TEXT_QST_081_ORCHARD_FARM"; break;
    case EGP_WAITING_FOR_TRADER_ESCORT: speech = "TEXT_QST_081_WAITING_FOR_TRADER_ESCORT"; break;
    case EGP_TRADER_ESCORT: speech = "TEXT_QST_081_TRADER_ESCORT"; break;
    case EGP_SECOND_MAZE_MEETING:
        if (Game->IsQuestCompleted("QS_GuardianSisterInfo2_SisterInBanditCamp")) {
            if (IsActiveThreadTerminating()) return nothing;
            speech = "TEXT_QST_081_BANDIT_CAMP";
        } else {
            if (IsActiveThreadTerminating()) return nothing;
            speech = "TEXT_QST_081_SECOND_MAZE_MEETING";
        }
        break;
    case EGP_BANDIT_CAMP: speech = "TEXT_QST_081_BANDIT_CAMP"; break;
    case EGP_WAITING_FOR_BANDIT_CAMP_TWINBLADE: speech = "TEXT_QST_081_WAITING_FOR_BANDIT_CAMP_TWINBLADE"; break;
    case EGP_BANDIT_CAMP_TWINBLADE: speech = "TEXT_QST_081_BANDIT_CAMP_TWINBLADE"; break;
    case EGP_MAZE_TELEPORT_TO_WW:
        if (Game->IsQuestActive("V_TrophyDealer")) {
            if (IsActiveThreadTerminating()) return nothing;
            speech = "TEXT_QST_081_TROPHY_DEALER";
        } else {
            if (IsActiveThreadTerminating()) return nothing;
            speech = "TEXT_QST_081_MAZE_TELEPORT_TO_WW";
        }
        break;
    case EGP_TROPHY_DEALER: speech = "TEXT_QST_081_TROPHY_DEALER"; break;
    case EGP_WITCHWOOD_POST_TROPHY_DEALER: speech = "TEXT_QST_081_WITCHWOOD_POST_TROPHY_DEALER"; break;
    case EGP_WHITE_BALVERINE_KHG: speech = "TEXT_QST_081_WHITE_BALVERINE_KHG"; break;
    case EGP_WHITE_BALVERINE_WITCHWOOD: speech = "TEXT_QST_081_WHITE_BALVERINE_WITCHWOOD"; break;
    case EGP_WITCHWOOD_WAITING_FOR_ARENA: speech = "TEXT_QST_081_WITCHWOOD_WAITING_FOR_ARENA"; break;
    case EGP_ARENA: speech = "TEXT_QST_081_ARENA"; break;
    case EGP_MEET_SISTER: speech = "TEXT_QST_081_MEET_SISTER"; break;
    case EGP_WAITING_FOR_MCC: speech = "TEXT_QST_081_WAITING_FOR_MCC"; break;
    case EGP_MINION_CLIFFTOP_CHASE: speech = "TEXT_QST_081_MINION_CLIFFTOP_CHASE"; break;
    case EGP_GRAVEYARD: speech = "TEXT_QST_081_GRAVEYARD"; break;
    case EGP_WAITING_FOR_FINALGRAVEYARD: speech = "TEXT_QST_081_WAITING_FOR_FINALGRAVEYARD"; break;
    case EGP_FINALGRAVEYARD: speech = "TEXT_QST_081_FINALGRAVEYARD"; break;
    case EGP_PRISON: speech = "TEXT_QST_081_PRISON"; break;
    case EGP_HOOK_COAST:
        if (MasterScriptData->SeenAbbeyMotherAtGuild) {
            if (IsActiveThreadTerminating()) return nothing;
            speech = "TEXT_QST_081_HOOK_COAST";
        } else {
            if (IsActiveThreadTerminating()) return nothing;
            speech = "TEXT_QST_081_BEFORE_HOOK_COAST";
        }
        break;
    case EGP_WAITING_FOR_WIZARD_BATTLE: speech = "TEXT_QST_081_WAITING_FOR_WIZARD_BATTLE"; break;
    case EGP_WIZARD_BATTLE: speech = "TEXT_QST_081_WIZARD_BATTLE"; break;
    case EGP_WAITING_FOR_FOCAL_SITES: speech = "TEXT_QST_081_WAITING_FOR_FOCAL_SITES"; break;
    case EGP_FOCAL_SITES: speech = "TEXT_QST_081_FOCAL_SITES"; break;
    case EGP_JACK_BOSS_FIGHT: speech = "TEXT_QST_081_JACK_BOSS_FIGHT"; break;
    case EGP_THE_SHIP: speech = "TEXT_QST_081_SHIP_SUMMONING"; break;
    case EGP_THE_ORACLE: speech = "TEXT_QST_081_THE_ORACLE"; break;
    case EGP_SCYTHE_MEETING: speech = "TEXT_QST_081_THE_ORACLE"; break;
    case EGP_HERO_SOULS:
        if (MasterScriptData->CurrentHeroSoulsPosition == EPIHS_1ST_SOUL_TALK_TO_BRIAR) {
            if (IsActiveThreadTerminating()) return nothing;
            speech = "TEXT_QST_081_HERO_SOULS";
        } else if (MasterScriptData->CurrentHeroSoulsPosition == EPIHS_1ST_SOUL_TALK_TO_THUNDER) {
            if (IsActiveThreadTerminating()) return nothing;
            speech = "TEXT_QST_081_HERO_SOULS_1ST_SOUL_TALK_TO_THUNDER";
        } else if (MasterScriptData->CurrentHeroSoulsPosition == EPIHS_1ST_SOUL_VISIT_THE_ARENA) {
            if (IsActiveThreadTerminating()) return nothing;
            speech = "TEXT_QST_081_HERO_SOULS_1ST_SOUL_VISIT_THE_ARENA";
        } else if (MasterScriptData->CurrentHeroSoulsPosition == EPIHS_2ND_SOUL_TALK_TO_BRIAR_KILLED_THUNDER) {
            if (IsActiveThreadTerminating()) return nothing;
            speech = "TEXT_QST_081_HERO_SOULS_2ND_SOUL_TALK_TO_BRIAR_KILLED_THUNDER";
        } else if (MasterScriptData->CurrentHeroSoulsPosition == EPIHS_2ND_SOUL_TALK_TO_BRIAR_GOT_ARENA_SOUL) {
            if (IsActiveThreadTerminating()) return nothing;
            speech = "TEXT_QST_081_HERO_SOULS_2ND_SOUL_TALK_TO_BRIAR_GOT_ARENA_SOUL";
        } else if (MasterScriptData->CurrentHeroSoulsPosition == EPIHS_2ND_SOUL_VISIT_YOUR_MOTHER) {
            if (IsActiveThreadTerminating()) return nothing;
            speech = "TEXT_QST_081_HERO_SOULS_2ND_SOUL_VISIT_YOUR_MOTHER";
        } else if (MasterScriptData->CurrentHeroSoulsPosition == EPIHS_3RD_SOUL_TALK_TO_BRIAR) {
            if (IsActiveThreadTerminating()) return nothing;
            speech = "TEXT_QST_081_HERO_SOULS_3RD_SOUL_TALK_TO_BRIAR";
        } else if (MasterScriptData->CurrentHeroSoulsPosition == EPIHS_3RD_SOUL_TALK_TO_SCYTHE) {
            if (IsActiveThreadTerminating()) return nothing;
            speech = "TEXT_QST_081_HERO_SOULS_3RD_SOUL_TALK_TO_SCYTHE";
        } else {
            if (IsActiveThreadTerminating()) return nothing;
            return "";
        }
        break;
    case EGP_DRAGON_FIGHT: speech = "TEXT_QST_081_DRAGON_FIGHT_10"; break;
    default:
        return "";
    }
    if (LastDialogueSaid == speech) {
        if (rand() % 4 == 0) {
            if (!GuildMasterDialogue[2]) {
                GuildMasterDialogue[2] = true;
                speech = "TEXT_QST_081_INFO_TELEPORTERS";
            } else if (!GuildMasterDialogue[3]) {
                GuildMasterDialogue[3] = true;
                speech = "TEXT_QST_081_INFO_WEAPONS";
            } else if (!GuildMasterDialogue[4]) {
                GuildMasterDialogue[4] = true;
                speech = "TEXT_QST_081_INFO_LOG_BOOK";
            } else if (Game->GetHeroTitle() == (EHeroTitle)19 /* retail compares with 0x13 */ && !GuildMasterDialogue[1]) {
                GuildMasterDialogue[1] = true;
                speech = "TEXT_QST_081_INFO_HERO_TITLE";
            }
        }
    }
    LastDialogueSaid = speech;
    return speech;
}