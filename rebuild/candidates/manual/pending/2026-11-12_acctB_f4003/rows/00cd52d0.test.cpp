// Behaviour test of CScriptInfoManager::RegisterScripts (0x00CD52D0).
// The body of the row runs against a recording model of CScriptInfoManager / CScriptInfo / CCharString.  The expected
// sequence of 479 calls (texts, script names, allocator addresses, master data allocator addresses, id, master flag, code
// sections, statuses) was generated from the RETAIL disassembly in address order (tools/map.py -> map/events.txt), not from
// the source; every Alloc function of the model carries the retail address of the function it stands for.
// Checks: (1) the recorded call sequence equals retail's, entry by entry; (2) the table AddScript builds (the landed
// 0x00CB5C90 logic: a non-empty section goes to the code-section list, every script to the map) has 161 scripts with
// unique names, 118 non-empty code sections with unique names (38 scripts have none), 5 master scripts, and every other script carries its master's data allocator
// or none; (3) each allocator stored in the table is the one retail stores (called through the stored pointer, it returns
// its own tag); (4) every CCharString temporary is destroyed again (only the strings the table holds stay alive);
// (5) the CScriptInfo constructor copies the name before it stores the other four members (retail 0x00CD531E..0x00CD5348).
#include <cstdio>
#include <cstring>
#include <stddef.h>

#define FABLE_FASTCALL

static int g_live = 0;            // CCharString objects alive
static int g_built = 0;           // CCharString objects ever constructed
static int g_fail = 0;
static void Fail(const char* what, int index) { if (g_fail < 10) std::printf("  mismatch: %s (entry %d)\n", what, index); ++g_fail; }

class CCharString {
public:
    char Text[64];
    CCharString() { Text[0] = 0; ++g_live; ++g_built; }
    CCharString(const char* text, long length = -1)
    {
        size_t n = length < 0 ? std::strlen(text) : (size_t)length;
        if (n > sizeof(Text) - 1) { n = sizeof(Text) - 1; Fail("text longer than the model string", -1); }
        std::memcpy(Text, text, n); Text[n] = 0; ++g_live; ++g_built;
    }
    CCharString(const CCharString& other) { std::strcpy(Text, other.Text); ++g_live; ++g_built; }
    ~CCharString() { --g_live; }
    CCharString& operator=(const CCharString& other) { std::strcpy(Text, other.Text); return *this; }
    long GetLength() const { return (long)std::strlen(Text); }
};

class CScriptBase;
class CScriptDataBase;
class CGameScriptInterfaceBase;

static int g_name_copied_at = 0;  // order stamps taken inside the CScriptInfo constructor
static int g_stamp = 0;

class CScriptInfo {
public:
    typedef CScriptBase* (FABLE_FASTCALL *tAllocFunc)(CScriptDataBase*, CGameScriptInterfaceBase&);
    typedef CScriptDataBase* (FABLE_FASTCALL *tMasterScriptDataAllocFunc)();
    CScriptInfo() : AllocFunc(0), MasterScriptDataAlloc(0), ID(0), MasterScript(false) { }
    CScriptInfo(const CCharString& name, long id, bool master_script, tAllocFunc alloc_func, tMasterScriptDataAllocFunc master_script_data_alloc)
        : Name(name)
    {
        g_name_copied_at = ++g_stamp;
        ID = id;
        MasterScript = master_script;
        AllocFunc = alloc_func;
        MasterScriptDataAlloc = master_script_data_alloc;
        ++g_stamp;
    }
    tAllocFunc AllocFunc;
    tMasterScriptDataAllocFunc MasterScriptDataAlloc;
    CCharString Name;
    long ID;
    bool MasterScript;
};

enum EEvent { EV_PROGRESS, EV_CHECK, EV_ADD, EV_STATUS };
struct SExpected {
    EEvent Kind;
    const char* Text;             // progress text / section (check) / script name (add, status)
    const char* Section;          // add: the code section
    unsigned int Alloc;           // add: retail address of the script allocator
    unsigned int DataAlloc;       // add: retail address of the master data allocator (0 = none)
    long Value;                   // add: id; status: the status value
    int Master;                   // add: master script flag
};
struct SRecorded {
    EEvent Kind;
    char Text[64];
    char Section[64];
    CScriptInfo::tAllocFunc Alloc;
    CScriptInfo::tMasterScriptDataAllocFunc DataAlloc;
    long Value;
    int Master;
    float Progress;
    int Flags;
};
enum { MAX_EVENTS = 1024, MAX_SCRIPTS = 256 };
static SRecorded g_rec[MAX_EVENTS];
static int g_count = 0;
static SRecorded* Next(EEvent kind)
{
    static SRecorded overflow;
    SRecorded* r = g_count < MAX_EVENTS ? &g_rec[g_count] : &overflow;
    ++g_count;
    std::memset(r, 0, sizeof(*r));
    r->Kind = kind;
    return r;
}

namespace NProgressDisplay {
void FABLE_FASTCALL DisplayProgress(const CCharString& text, float progress, bool flag1, bool flag2)
{
    SRecorded* r = Next(EV_PROGRESS);
    std::strcpy(r->Text, text.Text); r->Progress = progress; r->Flags = (flag1 ? 1 : 0) | (flag2 ? 2 : 0);
}
}

enum EScriptActiveStatus { };
void FABLE_FASTCALL SetScriptActiveStatus(const CCharString& name, EScriptActiveStatus status)
{
    SRecorded* r = Next(EV_STATUS);
    std::strcpy(r->Text, name.Text); r->Value = (long)status;
}

class CScriptInfoManager {
public:
    CScriptInfoManager() : CurrentScriptID(0), ScriptCount(0), SectionCount(0) { }
    void RegisterScripts();
    void AddScript(const CScriptInfo& info, CCharString section)
    {
        SRecorded* r = Next(EV_ADD);
        std::strcpy(r->Text, info.Name.Text); std::strcpy(r->Section, section.Text);
        r->Alloc = info.AllocFunc; r->DataAlloc = info.MasterScriptDataAlloc; r->Value = info.ID; r->Master = info.MasterScript ? 1 : 0;
        if (section.GetLength() > 0 && SectionCount < MAX_SCRIPTS)        // the landed AddScript 0x00CB5C90
            CodeSectionNames[SectionCount++] = section;
        if (ScriptCount < MAX_SCRIPTS) {
            ScriptNames[ScriptCount] = info.Name;
            Scripts[ScriptCount].AllocFunc = info.AllocFunc;
            Scripts[ScriptCount].MasterScriptDataAlloc = info.MasterScriptDataAlloc;
            Scripts[ScriptCount].Name = info.Name;
            Scripts[ScriptCount].ID = info.ID;
            Scripts[ScriptCount].MasterScript = info.MasterScript;
            ++ScriptCount;
        }
    }
    void CheckSectionIsNotAlreadyRegistered(CCharString section)
    {
        SRecorded* r = Next(EV_CHECK);
        std::strcpy(r->Text, section.Text);
    }
    long CurrentScriptID;
    int ScriptCount;
    int SectionCount;
    CCharString ScriptNames[MAX_SCRIPTS];
    CScriptInfo Scripts[MAX_SCRIPTS];
    CCharString CodeSectionNames[MAX_SCRIPTS];
};

// The model's script classes: every Alloc returns the address of its own tag, so two allocators are never the same function.
namespace NScript {
#define FABLE_SCRIPT_CLASS(Class) \
    class Class { public: static char Tag; static CScriptBase* FABLE_FASTCALL Alloc(CScriptDataBase*, CGameScriptInterfaceBase&) { return (CScriptBase*)&Tag; } }; \
    char Class::Tag
#define FABLE_MASTER_SCRIPT_CLASS(Class, Data) \
    class Data { public: static char Tag; static CScriptDataBase* FABLE_FASTCALL Alloc() { return (CScriptDataBase*)&Tag; } }; \
    char Data::Tag; \
    class Class { public: static char Tag; static CScriptBase* FABLE_FASTCALL Alloc(CScriptDataBase*, CGameScriptInterfaceBase&) { return (CScriptBase*)&Tag; } \
                  static CScriptInfo::tMasterScriptDataAllocFunc FABLE_FASTCALL StatGetMasterScriptDataAlloc() { return Data::Alloc; } }; \
    char Class::Tag

FABLE_MASTER_SCRIPT_CLASS(CQ_SunnyvaleMasterScript, CQ_SunnyvaleMasterData);
FABLE_MASTER_SCRIPT_CLASS(CalextestScript, CalextestData);
FABLE_MASTER_SCRIPT_CLASS(CEmptyScript, CEmptyData);
FABLE_MASTER_SCRIPT_CLASS(Cjoss_test_scriptsScript, Cjoss_test_scriptsData);
FABLE_MASTER_SCRIPT_CLASS(CDanyalTestScriptsScript, CDanyalTestScriptsData);
FABLE_SCRIPT_CLASS(CHeroBoastsScript);
FABLE_SCRIPT_CLASS(CPersonalScriptMainScript);
FABLE_SCRIPT_CLASS(CPersonalScript_GlobalThingsScript);
FABLE_SCRIPT_CLASS(CGameflowScript);
FABLE_SCRIPT_CLASS(CGameflowAssistanceScript);
FABLE_SCRIPT_CLASS(CQ_ArenaHoldingScriptScript);
FABLE_SCRIPT_CLASS(CLadyGreyWifeManagerScript);
FABLE_SCRIPT_CLASS(CQ_AmbushTradersScript);
FABLE_SCRIPT_CLASS(CQ_ArenaScript);
FABLE_SCRIPT_CLASS(CQ_AwakeningTheOracleScript);
FABLE_SCRIPT_CLASS(CQ_BanditCampScript);
FABLE_SCRIPT_CLASS(CQ_BanditCampBossBattleScript);
FABLE_SCRIPT_CLASS(CQ_BanditCampHoldingScriptScript);
FABLE_SCRIPT_CLASS(CQ_BountyHuntScript);
FABLE_SCRIPT_CLASS(CQ_BreakSiegeScript);
FABLE_SCRIPT_CLASS(CQ_BowerstoneTownLifeIntroScript);
FABLE_SCRIPT_CLASS(CQ_DragonBossFightScript);
FABLE_SCRIPT_CLASS(CQ_EndGameScript);
FABLE_SCRIPT_CLASS(CQ_EndGameFocalSitesScript);
FABLE_SCRIPT_CLASS(CQ_EndGameBossBattleScript);
FABLE_SCRIPT_CLASS(CQ_FireHeartScript);
FABLE_SCRIPT_CLASS(CQ_GuildTrainingScript);
FABLE_SCRIPT_CLASS(CQ_GuildTrainingDepartureScript);
FABLE_SCRIPT_CLASS(CQ_GuildTrainingPreMeleeScript);
FABLE_SCRIPT_CLASS(CQ_GuildTrainingMeleeScript);
FABLE_SCRIPT_CLASS(CQ_GuildTrainingWillScript);
FABLE_SCRIPT_CLASS(CQ_GuildTrainingSkillScript);
FABLE_SCRIPT_CLASS(CQ_GuildTrainingWoodsDepartureScript);
FABLE_SCRIPT_CLASS(CQ_GuildTrainingWoodsMeleeScript);
FABLE_SCRIPT_CLASS(CQ_GuildTrainingWoodsWillScript);
FABLE_SCRIPT_CLASS(CQ_HangingTreeGoodScript);
FABLE_SCRIPT_CLASS(CQ_HangingTreeEvilScript);
FABLE_SCRIPT_CLASS(CQ_HeroSoulsScript);
FABLE_SCRIPT_CLASS(CQ_HeroSoulsThunderScript);
FABLE_SCRIPT_CLASS(CQ_HeroSoulsArenaScript);
FABLE_SCRIPT_CLASS(CQ_HeroSoulsBriarScript);
FABLE_SCRIPT_CLASS(CQ_HeroSoulsMotherScript);
FABLE_SCRIPT_CLASS(CQ_HeroSoulsGuildmasterScript);
FABLE_SCRIPT_CLASS(CQ_HeroSoulsNostroScript);
FABLE_SCRIPT_CLASS(CQ_HerosOldHouseScript);
FABLE_SCRIPT_CLASS(CQ_HobbeCaveScript);
FABLE_SCRIPT_CLASS(CQ_HobbeToothContestScript);
FABLE_SCRIPT_CLASS(CQ_MinionCampScript);
FABLE_SCRIPT_CLASS(CQ_MinionClifftopChaseScript);
FABLE_SCRIPT_CLASS(CQ_NewOakValeIntroScript);
FABLE_SCRIPT_CLASS(CQ_OakValeBanditRaidScript);
FABLE_SCRIPT_CLASS(CQ_OrchardFarmRaidScript);
FABLE_SCRIPT_CLASS(CQ_OrchardFarmRaidEvilScript);
FABLE_SCRIPT_CLASS(CQ_OrchardFarmRaidGoodScript);
FABLE_SCRIPT_CLASS(CQ_OpeningGraveyardSecretPassageScript);
FABLE_SCRIPT_CLASS(CQ_PrisonEscapeRescueMotherScript);
FABLE_SCRIPT_CLASS(CQ_PrisonRaceScript);
FABLE_SCRIPT_CLASS(CQ_PrisonWardenGameScript);
FABLE_SCRIPT_CLASS(CQ_RansomVictimScript);
FABLE_SCRIPT_CLASS(CQ_RansomVictimChiefsHouseScript);
FABLE_SCRIPT_CLASS(CQ_SecretPassageScript);
FABLE_SCRIPT_CLASS(CQ_SummoningTheShipScript);
FABLE_SCRIPT_CLASS(CQ_TentacleKrakenBossFightScript);
FABLE_SCRIPT_CLASS(CQ_TraderConflictEvilScript);
FABLE_SCRIPT_CLASS(CQ_TraderConflictGoodScript);
FABLE_SCRIPT_CLASS(CQ_TraderEscortScript);
FABLE_SCRIPT_CLASS(CQ_WaspBossScript);
FABLE_SCRIPT_CLASS(CQ_WhiteBalverineKnotholeGladeScript);
FABLE_SCRIPT_CLASS(CQ_UndeadRisingScript);
FABLE_SCRIPT_CLASS(CQ_WhiteBalverineWWScript);
FABLE_SCRIPT_CLASS(CQ_WizardBattleScript);
FABLE_SCRIPT_CLASS(CQR_EscortTraderScript);
FABLE_SCRIPT_CLASS(CQR_EscortTrader_ManagerScript);
FABLE_SCRIPT_CLASS(CQS_GuardianSisterInfoScript);
FABLE_SCRIPT_CLASS(CQS_GuardianSisterInfo2_SisterInBanditCampScript);
FABLE_SCRIPT_CLASS(CQS_GuardianTrophyDealerInfoScript);
FABLE_SCRIPT_CLASS(CQS_MeetSisterScript);
FABLE_SCRIPT_CLASS(CQS_ScytheInfoScript);
FABLE_SCRIPT_CLASS(CV_AmbushScamScript);
FABLE_SCRIPT_CLASS(CV_ArcheryCompetitionScript);
FABLE_SCRIPT_CLASS(CV_AssassinAttacksScript);
FABLE_SCRIPT_CLASS(CV_BanditTollScript);
FABLE_SCRIPT_CLASS(CV_BanditCampPathScript);
FABLE_SCRIPT_CLASS(CV_BeardyBaldyScript);
FABLE_SCRIPT_CLASS(CV_BeggarAndChildScript);
FABLE_SCRIPT_CLASS(CV_BodyGuardScript);
FABLE_SCRIPT_CLASS(CV_BookCollectingScript);
FABLE_SCRIPT_CLASS(CV_BordelloScript);
FABLE_SCRIPT_CLASS(CV_ChapelOfEvilScript);
FABLE_SCRIPT_CLASS(CV_ExposeMayorScript);
FABLE_SCRIPT_CLASS(CV_ChickenKickingScript);
FABLE_SCRIPT_CLASS(CV_DemonDoorsScript);
FABLE_SCRIPT_CLASS(CV_FishermanScript);
FABLE_SCRIPT_CLASS(CV_FishingCompetitionScript);
FABLE_SCRIPT_CLASS(CV_FisticuffsClubScript);
FABLE_SCRIPT_CLASS(CV_GhostGrannyNecklaceScript);
FABLE_SCRIPT_CLASS(CV_GuildMasterScript);
FABLE_SCRIPT_CLASS(CV_HauntedHouseScript);
FABLE_SCRIPT_CLASS(CV_HeroDollsScript);
FABLE_SCRIPT_CLASS(CV_HeroDuelScript);
FABLE_SCRIPT_CLASS(CV_HiddenBootyScript);
FABLE_SCRIPT_CLASS(CV_IntroductionToTrophiesScript);
FABLE_SCRIPT_CLASS(CV_KnotholeGladeGatesScript);
FABLE_SCRIPT_CLASS(CV_LostTraderScript);
FABLE_SCRIPT_CLASS(CV_MazeResearchScript);
FABLE_SCRIPT_CLASS(CV_MurderTwistScript);
FABLE_SCRIPT_CLASS(CV_MurderTwistFinalScript);
FABLE_SCRIPT_CLASS(CV_MayorsInvitationScript);
FABLE_SCRIPT_CLASS(CV_OracleScript);
FABLE_SCRIPT_CLASS(CV_ParanoidWhispersScript);
FABLE_SCRIPT_CLASS(CV_PicnicAreaAfterWaspBossScript);
FABLE_SCRIPT_CLASS(CV_RandomPopulationSimScript);
FABLE_SCRIPT_CLASS(CV_RockTrollFirstEncounterScript);
FABLE_SCRIPT_CLASS(CV_SickChildScript);
FABLE_SCRIPT_CLASS(CV_SingingStonesScript);
FABLE_SCRIPT_CLASS(CV_StatueMasterScript);
FABLE_SCRIPT_CLASS(CV_SwordInTheStoneScript);
FABLE_SCRIPT_CLASS(CV_TalentlessBardScript);
FABLE_SCRIPT_CLASS(CV_TourGuideScript);
FABLE_SCRIPT_CLASS(CV_TempleOfLightScript);
FABLE_SCRIPT_CLASS(CV_TravellingHeroesScript);
FABLE_SCRIPT_CLASS(CV_TrophyDealerScript);
FABLE_SCRIPT_CLASS(CGlobal_WatchForHeroDeathScript);
FABLE_SCRIPT_CLASS(CGlobal_DebugCycleThroughSpeechScript);
FABLE_SCRIPT_CLASS(CExpression_FishScript);
FABLE_SCRIPT_CLASS(CExpression_FollowScript);
FABLE_SCRIPT_CLASS(CExpression_WaitScript);
FABLE_SCRIPT_CLASS(CExpression_DigScript);
FABLE_SCRIPT_CLASS(CExpression_PickpocketScript);
FABLE_SCRIPT_CLASS(CExpression_PicklockScript);
FABLE_SCRIPT_CLASS(CExpression_StealScript);
FABLE_SCRIPT_CLASS(CGlobal_TeleportToHeroGuildScript);
FABLE_SCRIPT_CLASS(CGlobal_ToggleTimeDisplayScript);
FABLE_SCRIPT_CLASS(CGlobal_GiveHeroItemsFromRewardChestScript);
FABLE_SCRIPT_CLASS(CGlobal_OpenChestScript);
FABLE_SCRIPT_CLASS(CCS_OakValeRevisitedScript);
FABLE_SCRIPT_CLASS(CPersonalScript_alextestScript);
FABLE_SCRIPT_CLASS(CProximityPodiumTestScript);
FABLE_SCRIPT_CLASS(CHitSwitchTestScript);
FABLE_SCRIPT_CLASS(CGuildSealTriggerTestScript);
FABLE_SCRIPT_CLASS(CPersonalScript_AardvarkScript);
FABLE_SCRIPT_CLASS(CHeroTurningDebuggerScript);
FABLE_SCRIPT_CLASS(Cguy_scriptScript);
FABLE_SCRIPT_CLASS(Cbandit_level_cutsceneScript);
FABLE_SCRIPT_CLASS(CTestTheInventoryScript);
FABLE_SCRIPT_CLASS(CPersonalScript_EmptyScript);
FABLE_SCRIPT_CLASS(CTestNewNav1Script);
FABLE_SCRIPT_CLASS(CTestNewNav2Script);
FABLE_SCRIPT_CLASS(CTestNewNav3Script);
FABLE_SCRIPT_CLASS(CTestNewNav4Script);
FABLE_SCRIPT_CLASS(CTestNewNav5Script);
FABLE_SCRIPT_CLASS(CTestQuestCard1Script);
FABLE_SCRIPT_CLASS(CTestQuestCard2Script);
FABLE_SCRIPT_CLASS(CTestQuestCard3Script);
FABLE_SCRIPT_CLASS(CMarkTestScriptsScript);
FABLE_SCRIPT_CLASS(CBenTestScriptsScript);
FABLE_SCRIPT_CLASS(CTonyTestScriptsScript);
FABLE_SCRIPT_CLASS(CShowTargetedThingHealthScript);
FABLE_SCRIPT_CLASS(COracleMinigameTestScript);
FABLE_SCRIPT_CLASS(CQ_CinemaTestScript);
FABLE_SCRIPT_CLASS(CCS_PlayCutsceneScript);
}

// retail address -> the model function standing for it
struct SAllocAddress { unsigned int Retail; CScriptInfo::tAllocFunc Func; char* Tag; };
static const SAllocAddress g_allocs[] = {
    { 0x00CDD550u, NScript::CQ_SunnyvaleMasterScript::Alloc, &NScript::CQ_SunnyvaleMasterScript::Tag },
    { 0x00EED220u, NScript::CalextestScript::Alloc, &NScript::CalextestScript::Tag },
    { 0x00EEE940u, NScript::CEmptyScript::Alloc, &NScript::CEmptyScript::Tag },
    { 0x00EF9EE0u, NScript::Cjoss_test_scriptsScript::Alloc, &NScript::Cjoss_test_scriptsScript::Tag },
    { 0x00EFB0B0u, NScript::CDanyalTestScriptsScript::Alloc, &NScript::CDanyalTestScriptsScript::Tag },
    { 0x00CE6C40u, NScript::CHeroBoastsScript::Alloc, &NScript::CHeroBoastsScript::Tag },
    { 0x00CDE2F0u, NScript::CPersonalScriptMainScript::Alloc, &NScript::CPersonalScriptMainScript::Tag },
    { 0x00CE19A0u, NScript::CPersonalScript_GlobalThingsScript::Alloc, &NScript::CPersonalScript_GlobalThingsScript::Tag },
    { 0x00CEF950u, NScript::CGameflowScript::Alloc, &NScript::CGameflowScript::Tag },
    { 0x00CF0640u, NScript::CGameflowAssistanceScript::Alloc, &NScript::CGameflowAssistanceScript::Tag },
    { 0x00CF8860u, NScript::CQ_ArenaHoldingScriptScript::Alloc, &NScript::CQ_ArenaHoldingScriptScript::Tag },
    { 0x00CF91C0u, NScript::CLadyGreyWifeManagerScript::Alloc, &NScript::CLadyGreyWifeManagerScript::Tag },
    { 0x00CFA2F0u, NScript::CQ_AmbushTradersScript::Alloc, &NScript::CQ_AmbushTradersScript::Tag },
    { 0x00CFABF0u, NScript::CQ_ArenaScript::Alloc, &NScript::CQ_ArenaScript::Tag },
    { 0x00D00590u, NScript::CQ_AwakeningTheOracleScript::Alloc, &NScript::CQ_AwakeningTheOracleScript::Tag },
    { 0x00D03580u, NScript::CQ_BanditCampScript::Alloc, &NScript::CQ_BanditCampScript::Tag },
    { 0x00D12920u, NScript::CQ_BanditCampBossBattleScript::Alloc, &NScript::CQ_BanditCampBossBattleScript::Tag },
    { 0x00D12C50u, NScript::CQ_BanditCampHoldingScriptScript::Alloc, &NScript::CQ_BanditCampHoldingScriptScript::Tag },
    { 0x00D1CBA0u, NScript::CQ_BountyHuntScript::Alloc, &NScript::CQ_BountyHuntScript::Tag },
    { 0x00D23C80u, NScript::CQ_BreakSiegeScript::Alloc, &NScript::CQ_BreakSiegeScript::Tag },
    { 0x00D253B0u, NScript::CQ_BowerstoneTownLifeIntroScript::Alloc, &NScript::CQ_BowerstoneTownLifeIntroScript::Tag },
    { 0x00D27240u, NScript::CQ_DragonBossFightScript::Alloc, &NScript::CQ_DragonBossFightScript::Tag },
    { 0x00D2A7C0u, NScript::CQ_EndGameScript::Alloc, &NScript::CQ_EndGameScript::Tag },
    { 0x00D2F5F0u, NScript::CQ_EndGameFocalSitesScript::Alloc, &NScript::CQ_EndGameFocalSitesScript::Tag },
    { 0x00D34410u, NScript::CQ_EndGameBossBattleScript::Alloc, &NScript::CQ_EndGameBossBattleScript::Tag },
    { 0x00D3AFC0u, NScript::CQ_FireHeartScript::Alloc, &NScript::CQ_FireHeartScript::Tag },
    { 0x00D50600u, NScript::CQ_GuildTrainingScript::Alloc, &NScript::CQ_GuildTrainingScript::Tag },
    { 0x00D513C0u, NScript::CQ_GuildTrainingDepartureScript::Alloc, &NScript::CQ_GuildTrainingDepartureScript::Tag },
    { 0x00D55D10u, NScript::CQ_GuildTrainingPreMeleeScript::Alloc, &NScript::CQ_GuildTrainingPreMeleeScript::Tag },
    { 0x00D5A9F0u, NScript::CQ_GuildTrainingMeleeScript::Alloc, &NScript::CQ_GuildTrainingMeleeScript::Tag },
    { 0x00D61BB0u, NScript::CQ_GuildTrainingWillScript::Alloc, &NScript::CQ_GuildTrainingWillScript::Tag },
    { 0x00D5DBF0u, NScript::CQ_GuildTrainingSkillScript::Alloc, &NScript::CQ_GuildTrainingSkillScript::Tag },
    { 0x00D664D0u, NScript::CQ_GuildTrainingWoodsDepartureScript::Alloc, &NScript::CQ_GuildTrainingWoodsDepartureScript::Tag },
    { 0x00D67740u, NScript::CQ_GuildTrainingWoodsMeleeScript::Alloc, &NScript::CQ_GuildTrainingWoodsMeleeScript::Tag },
    { 0x00D68EA0u, NScript::CQ_GuildTrainingWoodsWillScript::Alloc, &NScript::CQ_GuildTrainingWoodsWillScript::Tag },
    { 0x00D78290u, NScript::CQ_HangingTreeGoodScript::Alloc, &NScript::CQ_HangingTreeGoodScript::Tag },
    { 0x00D6ED10u, NScript::CQ_HangingTreeEvilScript::Alloc, &NScript::CQ_HangingTreeEvilScript::Tag },
    { 0x00D7AC50u, NScript::CQ_HeroSoulsScript::Alloc, &NScript::CQ_HeroSoulsScript::Tag },
    { 0x00D7C6A0u, NScript::CQ_HeroSoulsThunderScript::Alloc, &NScript::CQ_HeroSoulsThunderScript::Tag },
    { 0x00D80BA0u, NScript::CQ_HeroSoulsArenaScript::Alloc, &NScript::CQ_HeroSoulsArenaScript::Tag },
    { 0x00D83300u, NScript::CQ_HeroSoulsBriarScript::Alloc, &NScript::CQ_HeroSoulsBriarScript::Tag },
    { 0x00D84AF0u, NScript::CQ_HeroSoulsMotherScript::Alloc, &NScript::CQ_HeroSoulsMotherScript::Tag },
    { 0x00D87A80u, NScript::CQ_HeroSoulsGuildmasterScript::Alloc, &NScript::CQ_HeroSoulsGuildmasterScript::Tag },
    { 0x00D89CF0u, NScript::CQ_HeroSoulsNostroScript::Alloc, &NScript::CQ_HeroSoulsNostroScript::Tag },
    { 0x00D8D5A0u, NScript::CQ_HerosOldHouseScript::Alloc, &NScript::CQ_HerosOldHouseScript::Tag },
    { 0x00D99DC0u, NScript::CQ_HobbeCaveScript::Alloc, &NScript::CQ_HobbeCaveScript::Tag },
    { 0x00D9E190u, NScript::CQ_HobbeToothContestScript::Alloc, &NScript::CQ_HobbeToothContestScript::Tag },
    { 0x00DA2F40u, NScript::CQ_MinionCampScript::Alloc, &NScript::CQ_MinionCampScript::Tag },
    { 0x00DAAB70u, NScript::CQ_MinionClifftopChaseScript::Alloc, &NScript::CQ_MinionClifftopChaseScript::Tag },
    { 0x00DBEF70u, NScript::CQ_NewOakValeIntroScript::Alloc, &NScript::CQ_NewOakValeIntroScript::Tag },
    { 0x00DC24E0u, NScript::CQ_OakValeBanditRaidScript::Alloc, &NScript::CQ_OakValeBanditRaidScript::Tag },
    { 0x00DD1EF0u, NScript::CQ_OrchardFarmRaidScript::Alloc, &NScript::CQ_OrchardFarmRaidScript::Tag },
    { 0x00DD2300u, NScript::CQ_OrchardFarmRaidEvilScript::Alloc, &NScript::CQ_OrchardFarmRaidEvilScript::Tag },
    { 0x00DD2680u, NScript::CQ_OrchardFarmRaidGoodScript::Alloc, &NScript::CQ_OrchardFarmRaidGoodScript::Tag },
    { 0x00DCBFD0u, NScript::CQ_OpeningGraveyardSecretPassageScript::Alloc, &NScript::CQ_OpeningGraveyardSecretPassageScript::Tag },
    { 0x00DDACB0u, NScript::CQ_PrisonEscapeRescueMotherScript::Alloc, &NScript::CQ_PrisonEscapeRescueMotherScript::Tag },
    { 0x00DDDC30u, NScript::CQ_PrisonRaceScript::Alloc, &NScript::CQ_PrisonRaceScript::Tag },
    { 0x00DE0290u, NScript::CQ_PrisonWardenGameScript::Alloc, &NScript::CQ_PrisonWardenGameScript::Tag },
    { 0x00DEBC60u, NScript::CQ_RansomVictimScript::Alloc, &NScript::CQ_RansomVictimScript::Tag },
    { 0x00DEC790u, NScript::CQ_RansomVictimChiefsHouseScript::Alloc, &NScript::CQ_RansomVictimChiefsHouseScript::Tag },
    { 0x00DF1080u, NScript::CQ_SecretPassageScript::Alloc, &NScript::CQ_SecretPassageScript::Tag },
    { 0x00DF4070u, NScript::CQ_SummoningTheShipScript::Alloc, &NScript::CQ_SummoningTheShipScript::Tag },
    { 0x00DF5C00u, NScript::CQ_TentacleKrakenBossFightScript::Alloc, &NScript::CQ_TentacleKrakenBossFightScript::Tag },
    { 0x00DFA020u, NScript::CQ_TraderConflictEvilScript::Alloc, &NScript::CQ_TraderConflictEvilScript::Tag },
    { 0x00E00610u, NScript::CQ_TraderConflictGoodScript::Alloc, &NScript::CQ_TraderConflictGoodScript::Tag },
    { 0x00E0AF60u, NScript::CQ_TraderEscortScript::Alloc, &NScript::CQ_TraderEscortScript::Tag },
    { 0x00E0E9F0u, NScript::CQ_WaspBossScript::Alloc, &NScript::CQ_WaspBossScript::Tag },
    { 0x00E183B0u, NScript::CQ_WhiteBalverineKnotholeGladeScript::Alloc, &NScript::CQ_WhiteBalverineKnotholeGladeScript::Tag },
    { 0x00E0E760u, NScript::CQ_UndeadRisingScript::Alloc, &NScript::CQ_UndeadRisingScript::Tag },
    { 0x00E1A770u, NScript::CQ_WhiteBalverineWWScript::Alloc, &NScript::CQ_WhiteBalverineWWScript::Tag },
    { 0x00E1D9F0u, NScript::CQ_WizardBattleScript::Alloc, &NScript::CQ_WizardBattleScript::Tag },
    { 0x00E24C30u, NScript::CQR_EscortTraderScript::Alloc, &NScript::CQR_EscortTraderScript::Tag },
    { 0x00E25970u, NScript::CQR_EscortTrader_ManagerScript::Alloc, &NScript::CQR_EscortTrader_ManagerScript::Tag },
    { 0x00E26780u, NScript::CQS_GuardianSisterInfoScript::Alloc, &NScript::CQS_GuardianSisterInfoScript::Tag },
    { 0x00E277E0u, NScript::CQS_GuardianSisterInfo2_SisterInBanditCampScript::Alloc, &NScript::CQS_GuardianSisterInfo2_SisterInBanditCampScript::Tag },
    { 0x00E28CA0u, NScript::CQS_GuardianTrophyDealerInfoScript::Alloc, &NScript::CQS_GuardianTrophyDealerInfoScript::Tag },
    { 0x00E29990u, NScript::CQS_MeetSisterScript::Alloc, &NScript::CQS_MeetSisterScript::Tag },
    { 0x00E2AB00u, NScript::CQS_ScytheInfoScript::Alloc, &NScript::CQS_ScytheInfoScript::Tag },
    { 0x00E322C0u, NScript::CV_AmbushScamScript::Alloc, &NScript::CV_AmbushScamScript::Tag },
    { 0x00E37630u, NScript::CV_ArcheryCompetitionScript::Alloc, &NScript::CV_ArcheryCompetitionScript::Tag },
    { 0x00E39850u, NScript::CV_AssassinAttacksScript::Alloc, &NScript::CV_AssassinAttacksScript::Tag },
    { 0x00E4F550u, NScript::CV_BanditTollScript::Alloc, &NScript::CV_BanditTollScript::Tag },
    { 0x00E477A0u, NScript::CV_BanditCampPathScript::Alloc, &NScript::CV_BanditCampPathScript::Tag },
    { 0x00E54260u, NScript::CV_BeardyBaldyScript::Alloc, &NScript::CV_BeardyBaldyScript::Tag },
    { 0x00E5D0B0u, NScript::CV_BeggarAndChildScript::Alloc, &NScript::CV_BeggarAndChildScript::Tag },
    { 0x00E62730u, NScript::CV_BodyGuardScript::Alloc, &NScript::CV_BodyGuardScript::Tag },
    { 0x00E57A50u, NScript::CV_BookCollectingScript::Alloc, &NScript::CV_BookCollectingScript::Tag },
    { 0x00E46A00u, NScript::CV_BordelloScript::Alloc, &NScript::CV_BordelloScript::Tag },
    { 0x00E6E230u, NScript::CV_ChapelOfEvilScript::Alloc, &NScript::CV_ChapelOfEvilScript::Tag },
    { 0x00E7B290u, NScript::CV_ExposeMayorScript::Alloc, &NScript::CV_ExposeMayorScript::Tag },
    { 0x00E68E60u, NScript::CV_ChickenKickingScript::Alloc, &NScript::CV_ChickenKickingScript::Tag },
    { 0x00E75590u, NScript::CV_DemonDoorsScript::Alloc, &NScript::CV_DemonDoorsScript::Tag },
    { 0x00E7E0B0u, NScript::CV_FishermanScript::Alloc, &NScript::CV_FishermanScript::Tag },
    { 0x00E81EB0u, NScript::CV_FishingCompetitionScript::Alloc, &NScript::CV_FishingCompetitionScript::Tag },
    { 0x00E8B700u, NScript::CV_FisticuffsClubScript::Alloc, &NScript::CV_FisticuffsClubScript::Tag },
    { 0x00E906F0u, NScript::CV_GhostGrannyNecklaceScript::Alloc, &NScript::CV_GhostGrannyNecklaceScript::Tag },
    { 0x00E92900u, NScript::CV_GuildMasterScript::Alloc, &NScript::CV_GuildMasterScript::Tag },
    { 0x00E96DD0u, NScript::CV_HauntedHouseScript::Alloc, &NScript::CV_HauntedHouseScript::Tag },
    { 0x00E98640u, NScript::CV_HeroDollsScript::Alloc, &NScript::CV_HeroDollsScript::Tag },
    { 0x00E9C300u, NScript::CV_HeroDuelScript::Alloc, &NScript::CV_HeroDuelScript::Tag },
    { 0x00E93CF0u, NScript::CV_HiddenBootyScript::Alloc, &NScript::CV_HiddenBootyScript::Tag },
    { 0x00E9FB40u, NScript::CV_IntroductionToTrophiesScript::Alloc, &NScript::CV_IntroductionToTrophiesScript::Tag },
    { 0x00EA2C60u, NScript::CV_KnotholeGladeGatesScript::Alloc, &NScript::CV_KnotholeGladeGatesScript::Tag },
    { 0x00EA7680u, NScript::CV_LostTraderScript::Alloc, &NScript::CV_LostTraderScript::Tag },
    { 0x00EA8690u, NScript::CV_MazeResearchScript::Alloc, &NScript::CV_MazeResearchScript::Tag },
    { 0x00EAE890u, NScript::CV_MurderTwistScript::Alloc, &NScript::CV_MurderTwistScript::Tag },
    { 0x00EB6A60u, NScript::CV_MurderTwistFinalScript::Alloc, &NScript::CV_MurderTwistFinalScript::Tag },
    { 0x00EBCC10u, NScript::CV_MayorsInvitationScript::Alloc, &NScript::CV_MayorsInvitationScript::Tag },
    { 0x00EBEE30u, NScript::CV_OracleScript::Alloc, &NScript::CV_OracleScript::Tag },
    { 0x00EC11B0u, NScript::CV_ParanoidWhispersScript::Alloc, &NScript::CV_ParanoidWhispersScript::Tag },
    { 0x00EC1780u, NScript::CV_PicnicAreaAfterWaspBossScript::Alloc, &NScript::CV_PicnicAreaAfterWaspBossScript::Tag },
    { 0x00EC3BC0u, NScript::CV_RandomPopulationSimScript::Alloc, &NScript::CV_RandomPopulationSimScript::Tag },
    { 0x00EC5360u, NScript::CV_RockTrollFirstEncounterScript::Alloc, &NScript::CV_RockTrollFirstEncounterScript::Tag },
    { 0x00ED0E00u, NScript::CV_SickChildScript::Alloc, &NScript::CV_SickChildScript::Tag },
    { 0x00ED39A0u, NScript::CV_SingingStonesScript::Alloc, &NScript::CV_SingingStonesScript::Tag },
    { 0x00ED4A40u, NScript::CV_StatueMasterScript::Alloc, &NScript::CV_StatueMasterScript::Tag },
    { 0x00ED75F0u, NScript::CV_SwordInTheStoneScript::Alloc, &NScript::CV_SwordInTheStoneScript::Tag },
    { 0x00EDACC0u, NScript::CV_TalentlessBardScript::Alloc, &NScript::CV_TalentlessBardScript::Tag },
    { 0x00EE6C00u, NScript::CV_TourGuideScript::Alloc, &NScript::CV_TourGuideScript::Tag },
    { 0x00EDEB40u, NScript::CV_TempleOfLightScript::Alloc, &NScript::CV_TempleOfLightScript::Tag },
    { 0x00EE40A0u, NScript::CV_TravellingHeroesScript::Alloc, &NScript::CV_TravellingHeroesScript::Tag },
    { 0x00EE80A0u, NScript::CV_TrophyDealerScript::Alloc, &NScript::CV_TrophyDealerScript::Tag },
    { 0x00EE90A0u, NScript::CGlobal_WatchForHeroDeathScript::Alloc, &NScript::CGlobal_WatchForHeroDeathScript::Tag },
    { 0x00EE9450u, NScript::CGlobal_DebugCycleThroughSpeechScript::Alloc, &NScript::CGlobal_DebugCycleThroughSpeechScript::Tag },
    { 0x00EE9F00u, NScript::CExpression_FishScript::Alloc, &NScript::CExpression_FishScript::Tag },
    { 0x00EEA3E0u, NScript::CExpression_FollowScript::Alloc, &NScript::CExpression_FollowScript::Tag },
    { 0x00EEA740u, NScript::CExpression_WaitScript::Alloc, &NScript::CExpression_WaitScript::Tag },
    { 0x00EEAE70u, NScript::CExpression_DigScript::Alloc, &NScript::CExpression_DigScript::Tag },
    { 0x00EEB730u, NScript::CExpression_PickpocketScript::Alloc, &NScript::CExpression_PickpocketScript::Tag },
    { 0x00EEBD70u, NScript::CExpression_PicklockScript::Alloc, &NScript::CExpression_PicklockScript::Tag },
    { 0x00EEC390u, NScript::CExpression_StealScript::Alloc, &NScript::CExpression_StealScript::Tag },
    { 0x00CDDB60u, NScript::CGlobal_TeleportToHeroGuildScript::Alloc, &NScript::CGlobal_TeleportToHeroGuildScript::Tag },
    { 0x00EED060u, NScript::CGlobal_ToggleTimeDisplayScript::Alloc, &NScript::CGlobal_ToggleTimeDisplayScript::Tag },
    { 0x00EEC750u, NScript::CGlobal_GiveHeroItemsFromRewardChestScript::Alloc, &NScript::CGlobal_GiveHeroItemsFromRewardChestScript::Tag },
    { 0x00EECEA0u, NScript::CGlobal_OpenChestScript::Alloc, &NScript::CGlobal_OpenChestScript::Tag },
    { 0x00EE8BE0u, NScript::CCS_OakValeRevisitedScript::Alloc, &NScript::CCS_OakValeRevisitedScript::Tag },
    { 0x00EEE6D0u, NScript::CPersonalScript_alextestScript::Alloc, &NScript::CPersonalScript_alextestScript::Tag },
    { 0x00EFB610u, NScript::CProximityPodiumTestScript::Alloc, &NScript::CProximityPodiumTestScript::Tag },
    { 0x00EFBB70u, NScript::CHitSwitchTestScript::Alloc, &NScript::CHitSwitchTestScript::Tag },
    { 0x00EFC130u, NScript::CGuildSealTriggerTestScript::Alloc, &NScript::CGuildSealTriggerTestScript::Tag },
    { 0x00EFC8D0u, NScript::CPersonalScript_AardvarkScript::Alloc, &NScript::CPersonalScript_AardvarkScript::Tag },
    { 0x00EFC460u, NScript::CHeroTurningDebuggerScript::Alloc, &NScript::CHeroTurningDebuggerScript::Tag },
    { 0x00EFA3C0u, NScript::Cguy_scriptScript::Alloc, &NScript::Cguy_scriptScript::Tag },
    { 0x00EFAF30u, NScript::Cbandit_level_cutsceneScript::Alloc, &NScript::Cbandit_level_cutsceneScript::Tag },
    { 0x00EEEE40u, NScript::CTestTheInventoryScript::Alloc, &NScript::CTestTheInventoryScript::Tag },
    { 0x00EF9130u, NScript::CPersonalScript_EmptyScript::Alloc, &NScript::CPersonalScript_EmptyScript::Tag },
    { 0x00EEF9C0u, NScript::CTestNewNav1Script::Alloc, &NScript::CTestNewNav1Script::Tag },
    { 0x00EEFB20u, NScript::CTestNewNav2Script::Alloc, &NScript::CTestNewNav2Script::Tag },
    { 0x00EEFC80u, NScript::CTestNewNav3Script::Alloc, &NScript::CTestNewNav3Script::Tag },
    { 0x00EEFDE0u, NScript::CTestNewNav4Script::Alloc, &NScript::CTestNewNav4Script::Tag },
    { 0x00EEFF40u, NScript::CTestNewNav5Script::Alloc, &NScript::CTestNewNav5Script::Tag },
    { 0x00EEF540u, NScript::CTestQuestCard1Script::Alloc, &NScript::CTestQuestCard1Script::Tag },
    { 0x00EEF6D0u, NScript::CTestQuestCard2Script::Alloc, &NScript::CTestQuestCard2Script::Tag },
    { 0x00EEF860u, NScript::CTestQuestCard3Script::Alloc, &NScript::CTestQuestCard3Script::Tag },
    { 0x00EFE380u, NScript::CMarkTestScriptsScript::Alloc, &NScript::CMarkTestScriptsScript::Tag },
    { 0x00EFF000u, NScript::CBenTestScriptsScript::Alloc, &NScript::CBenTestScriptsScript::Tag },
    { 0x00EFF770u, NScript::CTonyTestScriptsScript::Alloc, &NScript::CTonyTestScriptsScript::Tag },
    { 0x00EFFCD0u, NScript::CShowTargetedThingHealthScript::Alloc, &NScript::CShowTargetedThingHealthScript::Tag },
    { 0x00F01330u, NScript::COracleMinigameTestScript::Alloc, &NScript::COracleMinigameTestScript::Tag },
    { 0x00F02460u, NScript::CQ_CinemaTestScript::Alloc, &NScript::CQ_CinemaTestScript::Tag },
    { 0x00F01760u, NScript::CCS_PlayCutsceneScript::Alloc, &NScript::CCS_PlayCutsceneScript::Tag },
};
struct SDataAllocAddress { unsigned int Retail; CScriptInfo::tMasterScriptDataAllocFunc Func; char* Tag; };
static const SDataAllocAddress g_data_allocs[] = {
    { 0x00CDBD20u, NScript::CQ_SunnyvaleMasterData::Alloc, &NScript::CQ_SunnyvaleMasterData::Tag },
    { 0x00CD4A60u, NScript::CalextestData::Alloc, &NScript::CalextestData::Tag },
    { 0x00CD4AC0u, NScript::CEmptyData::Alloc, &NScript::CEmptyData::Tag },
    { 0x00CD4B20u, NScript::Cjoss_test_scriptsData::Alloc, &NScript::Cjoss_test_scriptsData::Tag },
    { 0x00CD4B80u, NScript::CDanyalTestScriptsData::Alloc, &NScript::CDanyalTestScriptsData::Tag },
};

// retail call order (generated from the disassembly of 0x00CD52D0..0x00CDB35C)
static const SExpected g_expected[] = {
    { EV_PROGRESS, "Registering Master Script", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_SunnyvaleMaster", "", 0x00CDD550u, 0x00CDBD20u, 1, 1 },
    { EV_PROGRESS, "Registering Important Scripts", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_HB", "", 0, 0, 0, 0 },
    { EV_ADD, "HeroBoasts", "S_HB", 0x00CE6C40u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "HeroBoasts", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_PSM", "", 0, 0, 0, 0 },
    { EV_ADD, "PersonalScriptMain", "S_PSM", 0x00CDE2F0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "PersonalScriptMain", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_PSGT", "", 0, 0, 0, 0 },
    { EV_ADD, "PersonalScript_GlobalThings", "S_PSGT", 0x00CE19A0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "PersonalScript_GlobalThings", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_GF", "", 0, 0, 0, 0 },
    { EV_ADD, "Gameflow", "S_GF", 0x00CEF950u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Gameflow", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_GFA", "", 0, 0, 0, 0 },
    { EV_ADD, "GameflowAssistance", "S_GFA", 0x00CF0640u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "GameflowAssistance", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_AHS", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_ArenaHoldingScript", "S_AHS", 0x00CF8860u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_ArenaHoldingScript", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_LGWM", "", 0, 0, 0, 0 },
    { EV_ADD, "LadyGreyWifeManager", "S_LGWM", 0x00CF91C0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "LadyGreyWifeManager", "", 0, 0, 0, 0 },
    { EV_PROGRESS, "Registering Quest Scripts", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QAT", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_AmbushTraders", "S_QAT", 0x00CFA2F0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_AmbushTraders", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QA", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_Arena", "S_QA", 0x00CFABF0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_Arena", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QATO", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_AwakeningTheOracle", "S_QATO", 0x00D00590u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_AwakeningTheOracle", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QBC", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_BanditCamp", "S_QBC", 0x00D03580u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_BanditCamp", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QBCBB", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_BanditCampBossBattle", "S_QBCBB", 0x00D12920u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_BanditCampBossBattle", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QBCHS", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_BanditCampHoldingScript", "S_QBCHS", 0x00D12C50u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_BanditCampHoldingScript", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QBH", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_BountyHunt", "S_QBH", 0x00D1CBA0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_BountyHunt", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QBS", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_BreakSiege", "S_QBS", 0x00D23C80u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_BreakSiege", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QBTLI", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_BowerstoneTownLifeIntro", "S_QBTLI", 0x00D253B0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_BowerstoneTownLifeIntro", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QDBF", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_DragonBossFight", "S_QDBF", 0x00D27240u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_DragonBossFight", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QEG", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_EndGame", "S_QEG", 0x00D2A7C0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_EndGame", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QEGFS", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_EndGameFocalSites", "S_QEGFS", 0x00D2F5F0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_EndGameFocalSites", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QEGBB", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_EndGameBossBattle", "S_QEGBB", 0x00D34410u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_EndGameBossBattle", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QFHT", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_FireHeart", "S_QFHT", 0x00D3AFC0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_FireHeart", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QGT", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_GuildTraining", "S_QGT", 0x00D50600u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_GuildTraining", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QGTD", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_GuildTrainingDeparture", "S_QGTD", 0x00D513C0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_GuildTrainingDeparture", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QGTPM", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_GuildTrainingPreMelee", "S_QGTPM", 0x00D55D10u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_GuildTrainingPreMelee", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QGTM", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_GuildTrainingMelee", "S_QGTM", 0x00D5A9F0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_GuildTrainingMelee", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QGTW", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_GuildTrainingWill", "S_QGTW", 0x00D61BB0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_GuildTrainingWill", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QGTS", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_GuildTrainingSkill", "S_QGTS", 0x00D5DBF0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_GuildTrainingSkill", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QGTWD", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_GuildTrainingWoodsDeparture", "S_QGTWD", 0x00D664D0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_GuildTrainingWoodsDeparture", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QGTWM", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_GuildTrainingWoodsMelee", "S_QGTWM", 0x00D67740u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_GuildTrainingWoodsMelee", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QGTWW", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_GuildTrainingWoodsWill", "S_QGTWW", 0x00D68EA0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_GuildTrainingWoodsWill", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QHTG", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_HangingTreeGood", "S_QHTG", 0x00D78290u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_HangingTreeGood", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QHTE", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_HangingTreeEvil", "S_QHTE", 0x00D6ED10u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_HangingTreeEvil", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QHSS", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_HeroSouls", "S_QHSS", 0x00D7AC50u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_HeroSouls", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QHST", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_HeroSoulsThunder", "S_QHST", 0x00D7C6A0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_HeroSoulsThunder", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QHSA", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_HeroSoulsArena", "S_QHSA", 0x00D80BA0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_HeroSoulsArena", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QHSB", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_HeroSoulsBriar", "S_QHSB", 0x00D83300u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_HeroSoulsBriar", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QHSM", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_HeroSoulsMother", "S_QHSM", 0x00D84AF0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_HeroSoulsMother", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QHSG", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_HeroSoulsGuildmaster", "S_QHSG", 0x00D87A80u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_HeroSoulsGuildmaster", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QHSN", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_HeroSoulsNostro", "S_QHSN", 0x00D89CF0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_HeroSoulsNostro", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QHOH", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_HerosOldHouse", "S_QHOH", 0x00D8D5A0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_HerosOldHouse", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QHC", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_HobbeCave", "S_QHC", 0x00D99DC0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_HobbeCave", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QHTC", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_HobbeToothContest", "S_QHTC", 0x00D9E190u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_HobbeToothContest", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QMC", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_MinionCamp", "S_QMC", 0x00DA2F40u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_MinionCamp", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QMCC", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_MinionClifftopChase", "S_QMCC", 0x00DAAB70u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_MinionClifftopChase", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QNOVI", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_NewOakValeIntro", "S_QNOVI", 0x00DBEF70u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_NewOakValeIntro", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QOVBR", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_OakValeBanditRaid", "S_QOVBR", 0x00DC24E0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_OakValeBanditRaid", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QOFR", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_OrchardFarmRaid", "S_QOFR", 0x00DD1EF0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_OrchardFarmRaid", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QOFRE", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_OrchardFarmRaidEvil", "S_QOFRE", 0x00DD2300u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_OrchardFarmRaidEvil", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QOFRG", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_OrchardFarmRaidGood", "S_QOFRG", 0x00DD2680u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_OrchardFarmRaidGood", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QOGSP", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_OpeningGraveyardSecretPassage", "S_QOGSP", 0x00DCBFD0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_OpeningGraveyardSecretPassage", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QPERM", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_PrisonEscapeRescueMother", "S_QPERM", 0x00DDACB0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_PrisonEscapeRescueMother", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QPR", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_PrisonRace", "S_QPR", 0x00DDDC30u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_PrisonRace", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QPWG", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_PrisonWardenGame", "S_QPWG", 0x00DE0290u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_PrisonWardenGame", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QRV", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_RansomVictim", "S_QRV", 0x00DEBC60u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_RansomVictim", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QRVCH", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_RansomVictimChiefsHouse", "S_QRVCH", 0x00DEC790u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_RansomVictimChiefsHouse", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QSP", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_SecretPassage", "S_QSP", 0x00DF1080u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_SecretPassage", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QSTS", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_SummoningTheShip", "S_QSTS", 0x00DF4070u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_SummoningTheShip", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QTKBF", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_TentacleKrakenBossFight", "S_QTKBF", 0x00DF5C00u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_TentacleKrakenBossFight", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QTCE", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_TraderConflictEvil", "S_QTCE", 0x00DFA020u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_TraderConflictEvil", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QTCG", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_TraderConflictGood", "S_QTCG", 0x00E00610u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_TraderConflictGood", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QTE", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_TraderEscort", "S_QTE", 0x00E0AF60u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_TraderEscort", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QWB", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_WaspBoss", "S_QWB", 0x00E0E9F0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_WaspBoss", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QWBKG", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_WhiteBalverineKnotholeGlade", "S_QWBKG", 0x00E183B0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_WhiteBalverineKnotholeGlade", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QUR", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_UndeadRising", "S_QUR", 0x00E0E760u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_UndeadRising", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QWBW", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_WhiteBalverineWW", "S_QWBW", 0x00E1A770u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_WhiteBalverineWW", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QWZB", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_WizardBattle", "S_QWZB", 0x00E1D9F0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_WizardBattle", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QRET", "", 0, 0, 0, 0 },
    { EV_ADD, "QR_EscortTrader", "S_QRET", 0x00E24C30u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "QR_EscortTrader", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QRETM", "", 0, 0, 0, 0 },
    { EV_ADD, "QR_EscortTrader_Manager", "S_QRETM", 0x00E25970u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "QR_EscortTrader_Manager", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QSGSI", "", 0, 0, 0, 0 },
    { EV_ADD, "QS_GuardianSisterInfo", "S_QSGSI", 0x00E26780u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "QS_GuardianSisterInfo", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QSGSI2", "", 0, 0, 0, 0 },
    { EV_ADD, "QS_GuardianSisterInfo2_SisterInBanditCamp", "S_QSGSI2", 0x00E277E0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "QS_GuardianSisterInfo2_SisterInBanditCamp", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QSGTDI", "", 0, 0, 0, 0 },
    { EV_ADD, "QS_GuardianTrophyDealerInfo", "S_QSGTDI", 0x00E28CA0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "QS_GuardianTrophyDealerInfo", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QSMS", "", 0, 0, 0, 0 },
    { EV_ADD, "QS_MeetSister", "S_QSMS", 0x00E29990u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "QS_MeetSister", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_QSSI", "", 0, 0, 0, 0 },
    { EV_ADD, "QS_ScytheInfo", "S_QSSI", 0x00E2AB00u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "QS_ScytheInfo", "", 0, 0, 0, 0 },
    { EV_PROGRESS, "Registering Vignette Scripts", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_VAS", "", 0, 0, 0, 0 },
    { EV_ADD, "V_AmbushScam", "S_VAS", 0x00E322C0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "V_AmbushScam", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_VAC", "", 0, 0, 0, 0 },
    { EV_ADD, "V_ArcheryCompetition", "S_VAC", 0x00E37630u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "V_ArcheryCompetition", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_VAA", "", 0, 0, 0, 0 },
    { EV_ADD, "V_AssassinAttacks", "S_VAA", 0x00E39850u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "V_AssassinAttacks", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_VBT", "", 0, 0, 0, 0 },
    { EV_ADD, "V_BanditToll", "S_VBT", 0x00E4F550u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "V_BanditToll", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_VBCP", "", 0, 0, 0, 0 },
    { EV_ADD, "V_BanditCampPath", "S_VBCP", 0x00E477A0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "V_BanditCampPath", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_VBB", "", 0, 0, 0, 0 },
    { EV_ADD, "V_BeardyBaldy", "S_VBB", 0x00E54260u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "V_BeardyBaldy", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_VBAC", "", 0, 0, 0, 0 },
    { EV_ADD, "V_BeggarAndChild", "S_VBAC", 0x00E5D0B0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "V_BeggarAndChild", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_VBGu", "", 0, 0, 0, 0 },
    { EV_ADD, "V_BodyGuard", "S_VBGu", 0x00E62730u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "V_BodyGuard", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_VBC", "", 0, 0, 0, 0 },
    { EV_ADD, "V_BookCollecting", "S_VBC", 0x00E57A50u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "V_BookCollecting", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_VB", "", 0, 0, 0, 0 },
    { EV_ADD, "V_Bordello", "S_VB", 0x00E46A00u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "V_Bordello", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_VCOE", "", 0, 0, 0, 0 },
    { EV_ADD, "V_ChapelOfEvil", "S_VCOE", 0x00E6E230u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "V_ChapelOfEvil", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_VEM", "", 0, 0, 0, 0 },
    { EV_ADD, "V_ExposeMayor", "S_VEM", 0x00E7B290u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "V_ExposeMayor", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_VCK", "", 0, 0, 0, 0 },
    { EV_ADD, "V_ChickenKicking", "S_VCK", 0x00E68E60u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "V_ChickenKicking", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_DDRS", "", 0, 0, 0, 0 },
    { EV_ADD, "V_DemonDoors", "S_DDRS", 0x00E75590u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "V_DemonDoors", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_VFM", "", 0, 0, 0, 0 },
    { EV_ADD, "V_Fisherman", "S_VFM", 0x00E7E0B0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "V_Fisherman", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_VFC", "", 0, 0, 0, 0 },
    { EV_ADD, "V_FishingCompetition", "S_VFC", 0x00E81EB0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "V_FishingCompetition", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_VFCC", "", 0, 0, 0, 0 },
    { EV_ADD, "V_FisticuffsClub", "S_VFCC", 0x00E8B700u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "V_FisticuffsClub", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_VGGN", "", 0, 0, 0, 0 },
    { EV_ADD, "V_GhostGrannyNecklace", "S_VGGN", 0x00E906F0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "V_GhostGrannyNecklace", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_VGM", "", 0, 0, 0, 0 },
    { EV_ADD, "V_GuildMaster", "S_VGM", 0x00E92900u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "V_GuildMaster", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_VHH", "", 0, 0, 0, 0 },
    { EV_ADD, "V_HauntedHouse", "S_VHH", 0x00E96DD0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "V_HauntedHouse", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_VHDS", "", 0, 0, 0, 0 },
    { EV_ADD, "V_HeroDolls", "S_VHDS", 0x00E98640u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "V_HeroDolls", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_VHD", "", 0, 0, 0, 0 },
    { EV_ADD, "V_HeroDuel", "S_VHD", 0x00E9C300u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "V_HeroDuel", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_VHB", "", 0, 0, 0, 0 },
    { EV_ADD, "V_HiddenBooty", "S_VHB", 0x00E93CF0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "V_HiddenBooty", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_VITT", "", 0, 0, 0, 0 },
    { EV_ADD, "V_IntroductionToTrophies", "S_VITT", 0x00E9FB40u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "V_IntroductionToTrophies", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_VKGG", "", 0, 0, 0, 0 },
    { EV_ADD, "V_KnotholeGladeGates", "S_VKGG", 0x00EA2C60u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "V_KnotholeGladeGates", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_VLT", "", 0, 0, 0, 0 },
    { EV_ADD, "V_LostTrader", "S_VLT", 0x00EA7680u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "V_LostTrader", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_VMR", "", 0, 0, 0, 0 },
    { EV_ADD, "V_MazeResearch", "S_VMR", 0x00EA8690u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "V_MazeResearch", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_VMT", "", 0, 0, 0, 0 },
    { EV_ADD, "V_MurderTwist", "S_VMT", 0x00EAE890u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "V_MurderTwist", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_VMTF", "", 0, 0, 0, 0 },
    { EV_ADD, "V_MurderTwistFinal", "S_VMTF", 0x00EB6A60u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "V_MurderTwistFinal", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_VMI", "", 0, 0, 0, 0 },
    { EV_ADD, "V_MayorsInvitation", "S_VMI", 0x00EBCC10u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "V_MayorsInvitation", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_VO", "", 0, 0, 0, 0 },
    { EV_ADD, "V_Oracle", "S_VO", 0x00EBEE30u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "V_Oracle", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_VPW", "", 0, 0, 0, 0 },
    { EV_ADD, "V_ParanoidWhispers", "S_VPW", 0x00EC11B0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "V_ParanoidWhispers", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_VPAAWB", "", 0, 0, 0, 0 },
    { EV_ADD, "V_PicnicAreaAfterWaspBoss", "S_VPAAWB", 0x00EC1780u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "V_PicnicAreaAfterWaspBoss", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_VRPS", "", 0, 0, 0, 0 },
    { EV_ADD, "V_RandomPopulationSim", "S_VRPS", 0x00EC3BC0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "V_RandomPopulationSim", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_VRTFE", "", 0, 0, 0, 0 },
    { EV_ADD, "V_RockTrollFirstEncounter", "S_VRTFE", 0x00EC5360u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "V_RockTrollFirstEncounter", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_VSC", "", 0, 0, 0, 0 },
    { EV_ADD, "V_SickChild", "S_VSC", 0x00ED0E00u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "V_SickChild", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_VSS", "", 0, 0, 0, 0 },
    { EV_ADD, "V_SingingStones", "S_VSS", 0x00ED39A0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "V_SingingStones", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_VSM", "", 0, 0, 0, 0 },
    { EV_ADD, "V_StatueMaster", "S_VSM", 0x00ED4A40u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "V_StatueMaster", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_VSITS", "", 0, 0, 0, 0 },
    { EV_ADD, "V_SwordInTheStone", "S_VSITS", 0x00ED75F0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "V_SwordInTheStone", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_VTB", "", 0, 0, 0, 0 },
    { EV_ADD, "V_TalentlessBard", "S_VTB", 0x00EDACC0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "V_TalentlessBard", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_VTG", "", 0, 0, 0, 0 },
    { EV_ADD, "V_TourGuide", "S_VTG", 0x00EE6C00u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "V_TourGuide", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_VTOL", "", 0, 0, 0, 0 },
    { EV_ADD, "V_TempleOfLight", "S_VTOL", 0x00EDEB40u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "V_TempleOfLight", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_VTH", "", 0, 0, 0, 0 },
    { EV_ADD, "V_TravellingHeroes", "S_VTH", 0x00EE40A0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "V_TravellingHeroes", "", 0, 0, 0, 0 },
    { EV_CHECK, "S_VTD", "", 0, 0, 0, 0 },
    { EV_ADD, "V_TrophyDealer", "S_VTD", 0x00EE80A0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "V_TrophyDealer", "", 0, 0, 0, 0 },
    { EV_PROGRESS, "Registering Global Quest Scripts", "", 0, 0, 0, 0 },
    { EV_CHECK, "", "", 0, 0, 0, 0 },
    { EV_ADD, "Global_WatchForHeroDeath", "", 0x00EE90A0u, 0x00000000u, 1, 0 },
    { EV_STATUS, "Global_WatchForHeroDeath", "", 0, 0, 0, 0 },
    { EV_CHECK, "", "", 0, 0, 0, 0 },
    { EV_ADD, "Global_DebugCycleThroughSpeech", "", 0x00EE9450u, 0x00000000u, 1, 0 },
    { EV_STATUS, "Global_DebugCycleThroughSpeech", "", 0, 0, 0, 0 },
    { EV_CHECK, "", "", 0, 0, 0, 0 },
    { EV_ADD, "Expression_Fish", "", 0x00EE9F00u, 0x00000000u, 1, 0 },
    { EV_STATUS, "Expression_Fish", "", 0, 0, 0, 0 },
    { EV_CHECK, "", "", 0, 0, 0, 0 },
    { EV_ADD, "Expression_Follow", "", 0x00EEA3E0u, 0x00000000u, 1, 0 },
    { EV_STATUS, "Expression_Follow", "", 0, 0, 0, 0 },
    { EV_CHECK, "", "", 0, 0, 0, 0 },
    { EV_ADD, "Expression_Wait", "", 0x00EEA740u, 0x00000000u, 1, 0 },
    { EV_STATUS, "Expression_Wait", "", 0, 0, 0, 0 },
    { EV_CHECK, "", "", 0, 0, 0, 0 },
    { EV_ADD, "Expression_Dig", "", 0x00EEAE70u, 0x00000000u, 1, 0 },
    { EV_STATUS, "Expression_Dig", "", 0, 0, 0, 0 },
    { EV_CHECK, "", "", 0, 0, 0, 0 },
    { EV_ADD, "Expression_Pickpocket", "", 0x00EEB730u, 0x00000000u, 1, 0 },
    { EV_STATUS, "Expression_Pickpocket", "", 0, 0, 0, 0 },
    { EV_CHECK, "", "", 0, 0, 0, 0 },
    { EV_ADD, "Expression_Picklock", "", 0x00EEBD70u, 0x00000000u, 1, 0 },
    { EV_STATUS, "Expression_Picklock", "", 0, 0, 0, 0 },
    { EV_CHECK, "", "", 0, 0, 0, 0 },
    { EV_ADD, "Expression_Steal", "", 0x00EEC390u, 0x00000000u, 1, 0 },
    { EV_STATUS, "Expression_Steal", "", 0, 0, 0, 0 },
    { EV_CHECK, "", "", 0, 0, 0, 0 },
    { EV_ADD, "Global_TeleportToHeroGuild", "", 0x00CDDB60u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Global_TeleportToHeroGuild", "", 0, 0, 0, 0 },
    { EV_CHECK, "", "", 0, 0, 0, 0 },
    { EV_ADD, "Global_ToggleTimeDisplay", "", 0x00EED060u, 0x00000000u, 1, 0 },
    { EV_STATUS, "Global_ToggleTimeDisplay", "", 0, 0, 0, 0 },
    { EV_CHECK, "", "", 0, 0, 0, 0 },
    { EV_ADD, "Global_GiveHeroItemsFromRewardChest", "", 0x00EEC750u, 0x00000000u, 1, 0 },
    { EV_STATUS, "Global_GiveHeroItemsFromRewardChest", "", 0, 0, 0, 0 },
    { EV_CHECK, "", "", 0, 0, 0, 0 },
    { EV_ADD, "Global_OpenChest", "", 0x00EECEA0u, 0x00000000u, 1, 0 },
    { EV_STATUS, "Global_OpenChest", "", 0, 0, 0, 0 },
    { EV_CHECK, "CS_OVR", "", 0, 0, 0, 0 },
    { EV_ADD, "CS_OakValeRevisited", "CS_OVR", 0x00EE8BE0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "CS_OakValeRevisited", "", 0, 0, 0, 0 },
    { EV_PROGRESS, "Registering Test Scripts", "", 0, 0, 0, 0 },
    { EV_ADD, "alextest", "", 0x00EED220u, 0x00CD4A60u, 1, 1 },
    { EV_CHECK, "", "", 0, 0, 0, 0 },
    { EV_ADD, "PersonalScript_alextest", "", 0x00EEE6D0u, 0x00CD4A60u, 1, 0 },
    { EV_STATUS, "PersonalScript_alextest", "", 0, 0, 0, 0 },
    { EV_ADD, "DanyalTestScripts", "", 0x00EFB0B0u, 0x00CD4B80u, 1, 1 },
    { EV_CHECK, "", "", 0, 0, 0, 0 },
    { EV_ADD, "ProximityPodiumTest", "", 0x00EFB610u, 0x00CD4B80u, 1, 0 },
    { EV_STATUS, "ProximityPodiumTest", "", 0, 0, 0, 0 },
    { EV_CHECK, "", "", 0, 0, 0, 0 },
    { EV_ADD, "HitSwitchTest", "", 0x00EFBB70u, 0x00CD4B80u, 1, 0 },
    { EV_STATUS, "HitSwitchTest", "", 0, 0, 0, 0 },
    { EV_CHECK, "", "", 0, 0, 0, 0 },
    { EV_ADD, "GuildSealTriggerTest", "", 0x00EFC130u, 0x00CD4B80u, 1, 0 },
    { EV_STATUS, "GuildSealTriggerTest", "", 0, 0, 0, 0 },
    { EV_CHECK, "", "", 0, 0, 0, 0 },
    { EV_ADD, "PersonalScript_Aardvark", "", 0x00EFC8D0u, 0x00CD4B80u, 1, 0 },
    { EV_STATUS, "PersonalScript_Aardvark", "", 0, 0, 0, 0 },
    { EV_CHECK, "", "", 0, 0, 0, 0 },
    { EV_ADD, "HeroTurningDebugger", "", 0x00EFC460u, 0x00CD4B80u, 1, 0 },
    { EV_STATUS, "HeroTurningDebugger", "", 0, 0, 0, 0 },
    { EV_ADD, "joss_test_scripts", "", 0x00EF9EE0u, 0x00CD4B20u, 1, 1 },
    { EV_CHECK, "", "", 0, 0, 0, 0 },
    { EV_ADD, "guy_script", "", 0x00EFA3C0u, 0x00CD4B20u, 1, 0 },
    { EV_STATUS, "guy_script", "", 0, 0, 0, 0 },
    { EV_CHECK, "", "", 0, 0, 0, 0 },
    { EV_ADD, "bandit_level_cutscene", "", 0x00EFAF30u, 0x00CD4B20u, 1, 0 },
    { EV_STATUS, "bandit_level_cutscene", "", 0, 0, 0, 0 },
    { EV_ADD, "Empty", "", 0x00EEE940u, 0x00CD4AC0u, 1, 1 },
    { EV_CHECK, "", "", 0, 0, 0, 0 },
    { EV_ADD, "TestTheInventory", "", 0x00EEEE40u, 0x00CD4AC0u, 1, 0 },
    { EV_STATUS, "TestTheInventory", "", 0, 0, 0, 0 },
    { EV_CHECK, "", "", 0, 0, 0, 0 },
    { EV_ADD, "PersonalScript_Empty", "", 0x00EF9130u, 0x00CD4AC0u, 1, 0 },
    { EV_STATUS, "PersonalScript_Empty", "", 0, 0, 0, 0 },
    { EV_CHECK, "", "", 0, 0, 0, 0 },
    { EV_ADD, "TestNewNav1", "", 0x00EEF9C0u, 0x00CD4AC0u, 1, 0 },
    { EV_STATUS, "TestNewNav1", "", 0, 0, 0, 0 },
    { EV_CHECK, "", "", 0, 0, 0, 0 },
    { EV_ADD, "TestNewNav2", "", 0x00EEFB20u, 0x00CD4AC0u, 1, 0 },
    { EV_STATUS, "TestNewNav2", "", 0, 0, 0, 0 },
    { EV_CHECK, "", "", 0, 0, 0, 0 },
    { EV_ADD, "TestNewNav3", "", 0x00EEFC80u, 0x00CD4AC0u, 1, 0 },
    { EV_STATUS, "TestNewNav3", "", 0, 0, 0, 0 },
    { EV_CHECK, "", "", 0, 0, 0, 0 },
    { EV_ADD, "TestNewNav4", "", 0x00EEFDE0u, 0x00CD4AC0u, 1, 0 },
    { EV_STATUS, "TestNewNav4", "", 0, 0, 0, 0 },
    { EV_CHECK, "", "", 0, 0, 0, 0 },
    { EV_ADD, "TestNewNav5", "", 0x00EEFF40u, 0x00CD4AC0u, 1, 0 },
    { EV_STATUS, "TestNewNav5", "", 0, 0, 0, 0 },
    { EV_CHECK, "", "", 0, 0, 0, 0 },
    { EV_ADD, "TestQuestCard1", "", 0x00EEF540u, 0x00CD4AC0u, 1, 0 },
    { EV_STATUS, "TestQuestCard1", "", 0, 0, 0, 0 },
    { EV_CHECK, "", "", 0, 0, 0, 0 },
    { EV_ADD, "TestQuestCard2", "", 0x00EEF6D0u, 0x00CD4AC0u, 1, 0 },
    { EV_STATUS, "TestQuestCard2", "", 0, 0, 0, 0 },
    { EV_CHECK, "", "", 0, 0, 0, 0 },
    { EV_ADD, "TestQuestCard3", "", 0x00EEF860u, 0x00CD4AC0u, 1, 0 },
    { EV_STATUS, "TestQuestCard3", "", 0, 0, 0, 0 },
    { EV_CHECK, "", "", 0, 0, 0, 0 },
    { EV_ADD, "MarkTestScripts", "", 0x00EFE380u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "MarkTestScripts", "", 0, 0, 0, 0 },
    { EV_CHECK, "", "", 0, 0, 0, 0 },
    { EV_ADD, "BenTestScripts", "", 0x00EFF000u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "BenTestScripts", "", 0, 0, 0, 0 },
    { EV_CHECK, "", "", 0, 0, 0, 0 },
    { EV_ADD, "TonyTestScripts", "", 0x00EFF770u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "TonyTestScripts", "", 0, 0, 0, 0 },
    { EV_CHECK, "", "", 0, 0, 0, 0 },
    { EV_ADD, "ShowTargetedThingHealth", "", 0x00EFFCD0u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "ShowTargetedThingHealth", "", 0, 0, 0, 0 },
    { EV_CHECK, "", "", 0, 0, 0, 0 },
    { EV_ADD, "OracleMinigameTest", "", 0x00F01330u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "OracleMinigameTest", "", 0, 0, 0, 0 },
    { EV_CHECK, "", "", 0, 0, 0, 0 },
    { EV_ADD, "Q_CinemaTest", "", 0x00F02460u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "Q_CinemaTest", "", 0, 0, 0, 0 },
    { EV_CHECK, "", "", 0, 0, 0, 0 },
    { EV_ADD, "CS_PlayCutscene", "", 0x00F01760u, 0x00CDBD20u, 1, 0 },
    { EV_STATUS, "CS_PlayCutscene", "", 0, 0, 0, 0 },
};

void CScriptInfoManager::RegisterScripts()
{
    NProgressDisplay::DisplayProgress("Registering Master Script", -1.0f, false, false);
    AddScript(CScriptInfo("Q_SunnyvaleMaster", 1, true, NScript::CQ_SunnyvaleMasterScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "");
    NProgressDisplay::DisplayProgress("Registering Important Scripts", -1.0f, false, false);
    CheckSectionIsNotAlreadyRegistered("S_HB");
    AddScript(CScriptInfo("HeroBoasts", 1, false, NScript::CHeroBoastsScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_HB");
    SetScriptActiveStatus("HeroBoasts", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_PSM");
    AddScript(CScriptInfo("PersonalScriptMain", 1, false, NScript::CPersonalScriptMainScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_PSM");
    SetScriptActiveStatus("PersonalScriptMain", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_PSGT");
    AddScript(CScriptInfo("PersonalScript_GlobalThings", 1, false, NScript::CPersonalScript_GlobalThingsScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_PSGT");
    SetScriptActiveStatus("PersonalScript_GlobalThings", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_GF");
    AddScript(CScriptInfo("Gameflow", 1, false, NScript::CGameflowScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_GF");
    SetScriptActiveStatus("Gameflow", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_GFA");
    AddScript(CScriptInfo("GameflowAssistance", 1, false, NScript::CGameflowAssistanceScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_GFA");
    SetScriptActiveStatus("GameflowAssistance", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_AHS");
    AddScript(CScriptInfo("Q_ArenaHoldingScript", 1, false, NScript::CQ_ArenaHoldingScriptScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_AHS");
    SetScriptActiveStatus("Q_ArenaHoldingScript", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_LGWM");
    AddScript(CScriptInfo("LadyGreyWifeManager", 1, false, NScript::CLadyGreyWifeManagerScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_LGWM");
    SetScriptActiveStatus("LadyGreyWifeManager", (EScriptActiveStatus)0);
    NProgressDisplay::DisplayProgress("Registering Quest Scripts", -1.0f, false, false);
    CheckSectionIsNotAlreadyRegistered("S_QAT");
    AddScript(CScriptInfo("Q_AmbushTraders", 1, false, NScript::CQ_AmbushTradersScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QAT");
    SetScriptActiveStatus("Q_AmbushTraders", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QA");
    AddScript(CScriptInfo("Q_Arena", 1, false, NScript::CQ_ArenaScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QA");
    SetScriptActiveStatus("Q_Arena", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QATO");
    AddScript(CScriptInfo("Q_AwakeningTheOracle", 1, false, NScript::CQ_AwakeningTheOracleScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QATO");
    SetScriptActiveStatus("Q_AwakeningTheOracle", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QBC");
    AddScript(CScriptInfo("Q_BanditCamp", 1, false, NScript::CQ_BanditCampScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QBC");
    SetScriptActiveStatus("Q_BanditCamp", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QBCBB");
    AddScript(CScriptInfo("Q_BanditCampBossBattle", 1, false, NScript::CQ_BanditCampBossBattleScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QBCBB");
    SetScriptActiveStatus("Q_BanditCampBossBattle", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QBCHS");
    AddScript(CScriptInfo("Q_BanditCampHoldingScript", 1, false, NScript::CQ_BanditCampHoldingScriptScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QBCHS");
    SetScriptActiveStatus("Q_BanditCampHoldingScript", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QBH");
    AddScript(CScriptInfo("Q_BountyHunt", 1, false, NScript::CQ_BountyHuntScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QBH");
    SetScriptActiveStatus("Q_BountyHunt", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QBS");
    AddScript(CScriptInfo("Q_BreakSiege", 1, false, NScript::CQ_BreakSiegeScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QBS");
    SetScriptActiveStatus("Q_BreakSiege", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QBTLI");
    AddScript(CScriptInfo("Q_BowerstoneTownLifeIntro", 1, false, NScript::CQ_BowerstoneTownLifeIntroScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QBTLI");
    SetScriptActiveStatus("Q_BowerstoneTownLifeIntro", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QDBF");
    AddScript(CScriptInfo("Q_DragonBossFight", 1, false, NScript::CQ_DragonBossFightScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QDBF");
    SetScriptActiveStatus("Q_DragonBossFight", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QEG");
    AddScript(CScriptInfo("Q_EndGame", 1, false, NScript::CQ_EndGameScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QEG");
    SetScriptActiveStatus("Q_EndGame", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QEGFS");
    AddScript(CScriptInfo("Q_EndGameFocalSites", 1, false, NScript::CQ_EndGameFocalSitesScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QEGFS");
    SetScriptActiveStatus("Q_EndGameFocalSites", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QEGBB");
    AddScript(CScriptInfo("Q_EndGameBossBattle", 1, false, NScript::CQ_EndGameBossBattleScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QEGBB");
    SetScriptActiveStatus("Q_EndGameBossBattle", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QFHT");
    AddScript(CScriptInfo("Q_FireHeart", 1, false, NScript::CQ_FireHeartScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QFHT");
    SetScriptActiveStatus("Q_FireHeart", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QGT");
    AddScript(CScriptInfo("Q_GuildTraining", 1, false, NScript::CQ_GuildTrainingScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QGT");
    SetScriptActiveStatus("Q_GuildTraining", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QGTD");
    AddScript(CScriptInfo("Q_GuildTrainingDeparture", 1, false, NScript::CQ_GuildTrainingDepartureScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QGTD");
    SetScriptActiveStatus("Q_GuildTrainingDeparture", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QGTPM");
    AddScript(CScriptInfo("Q_GuildTrainingPreMelee", 1, false, NScript::CQ_GuildTrainingPreMeleeScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QGTPM");
    SetScriptActiveStatus("Q_GuildTrainingPreMelee", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QGTM");
    AddScript(CScriptInfo("Q_GuildTrainingMelee", 1, false, NScript::CQ_GuildTrainingMeleeScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QGTM");
    SetScriptActiveStatus("Q_GuildTrainingMelee", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QGTW");
    AddScript(CScriptInfo("Q_GuildTrainingWill", 1, false, NScript::CQ_GuildTrainingWillScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QGTW");
    SetScriptActiveStatus("Q_GuildTrainingWill", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QGTS");
    AddScript(CScriptInfo("Q_GuildTrainingSkill", 1, false, NScript::CQ_GuildTrainingSkillScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QGTS");
    SetScriptActiveStatus("Q_GuildTrainingSkill", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QGTWD");
    AddScript(CScriptInfo("Q_GuildTrainingWoodsDeparture", 1, false, NScript::CQ_GuildTrainingWoodsDepartureScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QGTWD");
    SetScriptActiveStatus("Q_GuildTrainingWoodsDeparture", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QGTWM");
    AddScript(CScriptInfo("Q_GuildTrainingWoodsMelee", 1, false, NScript::CQ_GuildTrainingWoodsMeleeScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QGTWM");
    SetScriptActiveStatus("Q_GuildTrainingWoodsMelee", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QGTWW");
    AddScript(CScriptInfo("Q_GuildTrainingWoodsWill", 1, false, NScript::CQ_GuildTrainingWoodsWillScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QGTWW");
    SetScriptActiveStatus("Q_GuildTrainingWoodsWill", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QHTG");
    AddScript(CScriptInfo("Q_HangingTreeGood", 1, false, NScript::CQ_HangingTreeGoodScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QHTG");
    SetScriptActiveStatus("Q_HangingTreeGood", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QHTE");
    AddScript(CScriptInfo("Q_HangingTreeEvil", 1, false, NScript::CQ_HangingTreeEvilScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QHTE");
    SetScriptActiveStatus("Q_HangingTreeEvil", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QHSS");
    AddScript(CScriptInfo("Q_HeroSouls", 1, false, NScript::CQ_HeroSoulsScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QHSS");
    SetScriptActiveStatus("Q_HeroSouls", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QHST");
    AddScript(CScriptInfo("Q_HeroSoulsThunder", 1, false, NScript::CQ_HeroSoulsThunderScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QHST");
    SetScriptActiveStatus("Q_HeroSoulsThunder", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QHSA");
    AddScript(CScriptInfo("Q_HeroSoulsArena", 1, false, NScript::CQ_HeroSoulsArenaScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QHSA");
    SetScriptActiveStatus("Q_HeroSoulsArena", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QHSB");
    AddScript(CScriptInfo("Q_HeroSoulsBriar", 1, false, NScript::CQ_HeroSoulsBriarScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QHSB");
    SetScriptActiveStatus("Q_HeroSoulsBriar", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QHSM");
    AddScript(CScriptInfo("Q_HeroSoulsMother", 1, false, NScript::CQ_HeroSoulsMotherScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QHSM");
    SetScriptActiveStatus("Q_HeroSoulsMother", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QHSG");
    AddScript(CScriptInfo("Q_HeroSoulsGuildmaster", 1, false, NScript::CQ_HeroSoulsGuildmasterScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QHSG");
    SetScriptActiveStatus("Q_HeroSoulsGuildmaster", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QHSN");
    AddScript(CScriptInfo("Q_HeroSoulsNostro", 1, false, NScript::CQ_HeroSoulsNostroScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QHSN");
    SetScriptActiveStatus("Q_HeroSoulsNostro", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QHOH");
    AddScript(CScriptInfo("Q_HerosOldHouse", 1, false, NScript::CQ_HerosOldHouseScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QHOH");
    SetScriptActiveStatus("Q_HerosOldHouse", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QHC");
    AddScript(CScriptInfo("Q_HobbeCave", 1, false, NScript::CQ_HobbeCaveScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QHC");
    SetScriptActiveStatus("Q_HobbeCave", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QHTC");
    AddScript(CScriptInfo("Q_HobbeToothContest", 1, false, NScript::CQ_HobbeToothContestScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QHTC");
    SetScriptActiveStatus("Q_HobbeToothContest", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QMC");
    AddScript(CScriptInfo("Q_MinionCamp", 1, false, NScript::CQ_MinionCampScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QMC");
    SetScriptActiveStatus("Q_MinionCamp", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QMCC");
    AddScript(CScriptInfo("Q_MinionClifftopChase", 1, false, NScript::CQ_MinionClifftopChaseScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QMCC");
    SetScriptActiveStatus("Q_MinionClifftopChase", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QNOVI");
    AddScript(CScriptInfo("Q_NewOakValeIntro", 1, false, NScript::CQ_NewOakValeIntroScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QNOVI");
    SetScriptActiveStatus("Q_NewOakValeIntro", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QOVBR");
    AddScript(CScriptInfo("Q_OakValeBanditRaid", 1, false, NScript::CQ_OakValeBanditRaidScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QOVBR");
    SetScriptActiveStatus("Q_OakValeBanditRaid", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QOFR");
    AddScript(CScriptInfo("Q_OrchardFarmRaid", 1, false, NScript::CQ_OrchardFarmRaidScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QOFR");
    SetScriptActiveStatus("Q_OrchardFarmRaid", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QOFRE");
    AddScript(CScriptInfo("Q_OrchardFarmRaidEvil", 1, false, NScript::CQ_OrchardFarmRaidEvilScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QOFRE");
    SetScriptActiveStatus("Q_OrchardFarmRaidEvil", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QOFRG");
    AddScript(CScriptInfo("Q_OrchardFarmRaidGood", 1, false, NScript::CQ_OrchardFarmRaidGoodScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QOFRG");
    SetScriptActiveStatus("Q_OrchardFarmRaidGood", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QOGSP");
    AddScript(CScriptInfo("Q_OpeningGraveyardSecretPassage", 1, false, NScript::CQ_OpeningGraveyardSecretPassageScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QOGSP");
    SetScriptActiveStatus("Q_OpeningGraveyardSecretPassage", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QPERM");
    AddScript(CScriptInfo("Q_PrisonEscapeRescueMother", 1, false, NScript::CQ_PrisonEscapeRescueMotherScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QPERM");
    SetScriptActiveStatus("Q_PrisonEscapeRescueMother", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QPR");
    AddScript(CScriptInfo("Q_PrisonRace", 1, false, NScript::CQ_PrisonRaceScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QPR");
    SetScriptActiveStatus("Q_PrisonRace", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QPWG");
    AddScript(CScriptInfo("Q_PrisonWardenGame", 1, false, NScript::CQ_PrisonWardenGameScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QPWG");
    SetScriptActiveStatus("Q_PrisonWardenGame", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QRV");
    AddScript(CScriptInfo("Q_RansomVictim", 1, false, NScript::CQ_RansomVictimScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QRV");
    SetScriptActiveStatus("Q_RansomVictim", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QRVCH");
    AddScript(CScriptInfo("Q_RansomVictimChiefsHouse", 1, false, NScript::CQ_RansomVictimChiefsHouseScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QRVCH");
    SetScriptActiveStatus("Q_RansomVictimChiefsHouse", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QSP");
    AddScript(CScriptInfo("Q_SecretPassage", 1, false, NScript::CQ_SecretPassageScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QSP");
    SetScriptActiveStatus("Q_SecretPassage", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QSTS");
    AddScript(CScriptInfo("Q_SummoningTheShip", 1, false, NScript::CQ_SummoningTheShipScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QSTS");
    SetScriptActiveStatus("Q_SummoningTheShip", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QTKBF");
    AddScript(CScriptInfo("Q_TentacleKrakenBossFight", 1, false, NScript::CQ_TentacleKrakenBossFightScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QTKBF");
    SetScriptActiveStatus("Q_TentacleKrakenBossFight", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QTCE");
    AddScript(CScriptInfo("Q_TraderConflictEvil", 1, false, NScript::CQ_TraderConflictEvilScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QTCE");
    SetScriptActiveStatus("Q_TraderConflictEvil", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QTCG");
    AddScript(CScriptInfo("Q_TraderConflictGood", 1, false, NScript::CQ_TraderConflictGoodScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QTCG");
    SetScriptActiveStatus("Q_TraderConflictGood", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QTE");
    AddScript(CScriptInfo("Q_TraderEscort", 1, false, NScript::CQ_TraderEscortScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QTE");
    SetScriptActiveStatus("Q_TraderEscort", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QWB");
    AddScript(CScriptInfo("Q_WaspBoss", 1, false, NScript::CQ_WaspBossScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QWB");
    SetScriptActiveStatus("Q_WaspBoss", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QWBKG");
    AddScript(CScriptInfo("Q_WhiteBalverineKnotholeGlade", 1, false, NScript::CQ_WhiteBalverineKnotholeGladeScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QWBKG");
    SetScriptActiveStatus("Q_WhiteBalverineKnotholeGlade", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QUR");
    AddScript(CScriptInfo("Q_UndeadRising", 1, false, NScript::CQ_UndeadRisingScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QUR");
    SetScriptActiveStatus("Q_UndeadRising", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QWBW");
    AddScript(CScriptInfo("Q_WhiteBalverineWW", 1, false, NScript::CQ_WhiteBalverineWWScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QWBW");
    SetScriptActiveStatus("Q_WhiteBalverineWW", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QWZB");
    AddScript(CScriptInfo("Q_WizardBattle", 1, false, NScript::CQ_WizardBattleScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QWZB");
    SetScriptActiveStatus("Q_WizardBattle", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QRET");
    AddScript(CScriptInfo("QR_EscortTrader", 1, false, NScript::CQR_EscortTraderScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QRET");
    SetScriptActiveStatus("QR_EscortTrader", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QRETM");
    AddScript(CScriptInfo("QR_EscortTrader_Manager", 1, false, NScript::CQR_EscortTrader_ManagerScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QRETM");
    SetScriptActiveStatus("QR_EscortTrader_Manager", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QSGSI");
    AddScript(CScriptInfo("QS_GuardianSisterInfo", 1, false, NScript::CQS_GuardianSisterInfoScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QSGSI");
    SetScriptActiveStatus("QS_GuardianSisterInfo", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QSGSI2");
    AddScript(CScriptInfo("QS_GuardianSisterInfo2_SisterInBanditCamp", 1, false, NScript::CQS_GuardianSisterInfo2_SisterInBanditCampScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QSGSI2");
    SetScriptActiveStatus("QS_GuardianSisterInfo2_SisterInBanditCamp", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QSGTDI");
    AddScript(CScriptInfo("QS_GuardianTrophyDealerInfo", 1, false, NScript::CQS_GuardianTrophyDealerInfoScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QSGTDI");
    SetScriptActiveStatus("QS_GuardianTrophyDealerInfo", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QSMS");
    AddScript(CScriptInfo("QS_MeetSister", 1, false, NScript::CQS_MeetSisterScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QSMS");
    SetScriptActiveStatus("QS_MeetSister", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_QSSI");
    AddScript(CScriptInfo("QS_ScytheInfo", 1, false, NScript::CQS_ScytheInfoScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_QSSI");
    SetScriptActiveStatus("QS_ScytheInfo", (EScriptActiveStatus)0);
    NProgressDisplay::DisplayProgress("Registering Vignette Scripts", -1.0f, false, false);
    CheckSectionIsNotAlreadyRegistered("S_VAS");
    AddScript(CScriptInfo("V_AmbushScam", 1, false, NScript::CV_AmbushScamScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VAS");
    SetScriptActiveStatus("V_AmbushScam", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VAC");
    AddScript(CScriptInfo("V_ArcheryCompetition", 1, false, NScript::CV_ArcheryCompetitionScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VAC");
    SetScriptActiveStatus("V_ArcheryCompetition", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VAA");
    AddScript(CScriptInfo("V_AssassinAttacks", 1, false, NScript::CV_AssassinAttacksScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VAA");
    SetScriptActiveStatus("V_AssassinAttacks", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VBT");
    AddScript(CScriptInfo("V_BanditToll", 1, false, NScript::CV_BanditTollScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VBT");
    SetScriptActiveStatus("V_BanditToll", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VBCP");
    AddScript(CScriptInfo("V_BanditCampPath", 1, false, NScript::CV_BanditCampPathScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VBCP");
    SetScriptActiveStatus("V_BanditCampPath", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VBB");
    AddScript(CScriptInfo("V_BeardyBaldy", 1, false, NScript::CV_BeardyBaldyScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VBB");
    SetScriptActiveStatus("V_BeardyBaldy", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VBAC");
    AddScript(CScriptInfo("V_BeggarAndChild", 1, false, NScript::CV_BeggarAndChildScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VBAC");
    SetScriptActiveStatus("V_BeggarAndChild", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VBGu");
    AddScript(CScriptInfo("V_BodyGuard", 1, false, NScript::CV_BodyGuardScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VBGu");
    SetScriptActiveStatus("V_BodyGuard", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VBC");
    AddScript(CScriptInfo("V_BookCollecting", 1, false, NScript::CV_BookCollectingScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VBC");
    SetScriptActiveStatus("V_BookCollecting", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VB");
    AddScript(CScriptInfo("V_Bordello", 1, false, NScript::CV_BordelloScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VB");
    SetScriptActiveStatus("V_Bordello", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VCOE");
    AddScript(CScriptInfo("V_ChapelOfEvil", 1, false, NScript::CV_ChapelOfEvilScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VCOE");
    SetScriptActiveStatus("V_ChapelOfEvil", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VEM");
    AddScript(CScriptInfo("V_ExposeMayor", 1, false, NScript::CV_ExposeMayorScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VEM");
    SetScriptActiveStatus("V_ExposeMayor", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VCK");
    AddScript(CScriptInfo("V_ChickenKicking", 1, false, NScript::CV_ChickenKickingScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VCK");
    SetScriptActiveStatus("V_ChickenKicking", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_DDRS");
    AddScript(CScriptInfo("V_DemonDoors", 1, false, NScript::CV_DemonDoorsScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_DDRS");
    SetScriptActiveStatus("V_DemonDoors", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VFM");
    AddScript(CScriptInfo("V_Fisherman", 1, false, NScript::CV_FishermanScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VFM");
    SetScriptActiveStatus("V_Fisherman", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VFC");
    AddScript(CScriptInfo("V_FishingCompetition", 1, false, NScript::CV_FishingCompetitionScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VFC");
    SetScriptActiveStatus("V_FishingCompetition", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VFCC");
    AddScript(CScriptInfo("V_FisticuffsClub", 1, false, NScript::CV_FisticuffsClubScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VFCC");
    SetScriptActiveStatus("V_FisticuffsClub", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VGGN");
    AddScript(CScriptInfo("V_GhostGrannyNecklace", 1, false, NScript::CV_GhostGrannyNecklaceScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VGGN");
    SetScriptActiveStatus("V_GhostGrannyNecklace", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VGM");
    AddScript(CScriptInfo("V_GuildMaster", 1, false, NScript::CV_GuildMasterScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VGM");
    SetScriptActiveStatus("V_GuildMaster", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VHH");
    AddScript(CScriptInfo("V_HauntedHouse", 1, false, NScript::CV_HauntedHouseScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VHH");
    SetScriptActiveStatus("V_HauntedHouse", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VHDS");
    AddScript(CScriptInfo("V_HeroDolls", 1, false, NScript::CV_HeroDollsScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VHDS");
    SetScriptActiveStatus("V_HeroDolls", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VHD");
    AddScript(CScriptInfo("V_HeroDuel", 1, false, NScript::CV_HeroDuelScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VHD");
    SetScriptActiveStatus("V_HeroDuel", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VHB");
    AddScript(CScriptInfo("V_HiddenBooty", 1, false, NScript::CV_HiddenBootyScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VHB");
    SetScriptActiveStatus("V_HiddenBooty", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VITT");
    AddScript(CScriptInfo("V_IntroductionToTrophies", 1, false, NScript::CV_IntroductionToTrophiesScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VITT");
    SetScriptActiveStatus("V_IntroductionToTrophies", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VKGG");
    AddScript(CScriptInfo("V_KnotholeGladeGates", 1, false, NScript::CV_KnotholeGladeGatesScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VKGG");
    SetScriptActiveStatus("V_KnotholeGladeGates", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VLT");
    AddScript(CScriptInfo("V_LostTrader", 1, false, NScript::CV_LostTraderScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VLT");
    SetScriptActiveStatus("V_LostTrader", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VMR");
    AddScript(CScriptInfo("V_MazeResearch", 1, false, NScript::CV_MazeResearchScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VMR");
    SetScriptActiveStatus("V_MazeResearch", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VMT");
    AddScript(CScriptInfo("V_MurderTwist", 1, false, NScript::CV_MurderTwistScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VMT");
    SetScriptActiveStatus("V_MurderTwist", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VMTF");
    AddScript(CScriptInfo("V_MurderTwistFinal", 1, false, NScript::CV_MurderTwistFinalScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VMTF");
    SetScriptActiveStatus("V_MurderTwistFinal", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VMI");
    AddScript(CScriptInfo("V_MayorsInvitation", 1, false, NScript::CV_MayorsInvitationScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VMI");
    SetScriptActiveStatus("V_MayorsInvitation", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VO");
    AddScript(CScriptInfo("V_Oracle", 1, false, NScript::CV_OracleScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VO");
    SetScriptActiveStatus("V_Oracle", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VPW");
    AddScript(CScriptInfo("V_ParanoidWhispers", 1, false, NScript::CV_ParanoidWhispersScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VPW");
    SetScriptActiveStatus("V_ParanoidWhispers", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VPAAWB");
    AddScript(CScriptInfo("V_PicnicAreaAfterWaspBoss", 1, false, NScript::CV_PicnicAreaAfterWaspBossScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VPAAWB");
    SetScriptActiveStatus("V_PicnicAreaAfterWaspBoss", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VRPS");
    AddScript(CScriptInfo("V_RandomPopulationSim", 1, false, NScript::CV_RandomPopulationSimScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VRPS");
    SetScriptActiveStatus("V_RandomPopulationSim", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VRTFE");
    AddScript(CScriptInfo("V_RockTrollFirstEncounter", 1, false, NScript::CV_RockTrollFirstEncounterScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VRTFE");
    SetScriptActiveStatus("V_RockTrollFirstEncounter", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VSC");
    AddScript(CScriptInfo("V_SickChild", 1, false, NScript::CV_SickChildScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VSC");
    SetScriptActiveStatus("V_SickChild", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VSS");
    AddScript(CScriptInfo("V_SingingStones", 1, false, NScript::CV_SingingStonesScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VSS");
    SetScriptActiveStatus("V_SingingStones", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VSM");
    AddScript(CScriptInfo("V_StatueMaster", 1, false, NScript::CV_StatueMasterScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VSM");
    SetScriptActiveStatus("V_StatueMaster", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VSITS");
    AddScript(CScriptInfo("V_SwordInTheStone", 1, false, NScript::CV_SwordInTheStoneScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VSITS");
    SetScriptActiveStatus("V_SwordInTheStone", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VTB");
    AddScript(CScriptInfo("V_TalentlessBard", 1, false, NScript::CV_TalentlessBardScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VTB");
    SetScriptActiveStatus("V_TalentlessBard", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VTG");
    AddScript(CScriptInfo("V_TourGuide", 1, false, NScript::CV_TourGuideScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VTG");
    SetScriptActiveStatus("V_TourGuide", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VTOL");
    AddScript(CScriptInfo("V_TempleOfLight", 1, false, NScript::CV_TempleOfLightScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VTOL");
    SetScriptActiveStatus("V_TempleOfLight", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VTH");
    AddScript(CScriptInfo("V_TravellingHeroes", 1, false, NScript::CV_TravellingHeroesScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VTH");
    SetScriptActiveStatus("V_TravellingHeroes", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("S_VTD");
    AddScript(CScriptInfo("V_TrophyDealer", 1, false, NScript::CV_TrophyDealerScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "S_VTD");
    SetScriptActiveStatus("V_TrophyDealer", (EScriptActiveStatus)0);
    NProgressDisplay::DisplayProgress("Registering Global Quest Scripts", -1.0f, false, false);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("Global_WatchForHeroDeath", 1, false, NScript::CGlobal_WatchForHeroDeathScript::Alloc, NULL), "");
    SetScriptActiveStatus("Global_WatchForHeroDeath", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("Global_DebugCycleThroughSpeech", 1, false, NScript::CGlobal_DebugCycleThroughSpeechScript::Alloc, NULL), "");
    SetScriptActiveStatus("Global_DebugCycleThroughSpeech", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("Expression_Fish", 1, false, NScript::CExpression_FishScript::Alloc, NULL), "");
    SetScriptActiveStatus("Expression_Fish", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("Expression_Follow", 1, false, NScript::CExpression_FollowScript::Alloc, NULL), "");
    SetScriptActiveStatus("Expression_Follow", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("Expression_Wait", 1, false, NScript::CExpression_WaitScript::Alloc, NULL), "");
    SetScriptActiveStatus("Expression_Wait", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("Expression_Dig", 1, false, NScript::CExpression_DigScript::Alloc, NULL), "");
    SetScriptActiveStatus("Expression_Dig", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("Expression_Pickpocket", 1, false, NScript::CExpression_PickpocketScript::Alloc, NULL), "");
    SetScriptActiveStatus("Expression_Pickpocket", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("Expression_Picklock", 1, false, NScript::CExpression_PicklockScript::Alloc, NULL), "");
    SetScriptActiveStatus("Expression_Picklock", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("Expression_Steal", 1, false, NScript::CExpression_StealScript::Alloc, NULL), "");
    SetScriptActiveStatus("Expression_Steal", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("Global_TeleportToHeroGuild", 1, false, NScript::CGlobal_TeleportToHeroGuildScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("Global_TeleportToHeroGuild", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("Global_ToggleTimeDisplay", 1, false, NScript::CGlobal_ToggleTimeDisplayScript::Alloc, NULL), "");
    SetScriptActiveStatus("Global_ToggleTimeDisplay", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("Global_GiveHeroItemsFromRewardChest", 1, false, NScript::CGlobal_GiveHeroItemsFromRewardChestScript::Alloc, NULL), "");
    SetScriptActiveStatus("Global_GiveHeroItemsFromRewardChest", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("Global_OpenChest", 1, false, NScript::CGlobal_OpenChestScript::Alloc, NULL), "");
    SetScriptActiveStatus("Global_OpenChest", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("CS_OVR");
    AddScript(CScriptInfo("CS_OakValeRevisited", 1, false, NScript::CCS_OakValeRevisitedScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "CS_OVR");
    SetScriptActiveStatus("CS_OakValeRevisited", (EScriptActiveStatus)0);
    NProgressDisplay::DisplayProgress("Registering Test Scripts", -1.0f, false, false);
    AddScript(CScriptInfo("alextest", 1, true, NScript::CalextestScript::Alloc, NScript::CalextestScript::StatGetMasterScriptDataAlloc()), "");
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("PersonalScript_alextest", 1, false, NScript::CPersonalScript_alextestScript::Alloc, NScript::CalextestScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("PersonalScript_alextest", (EScriptActiveStatus)0);
    AddScript(CScriptInfo("DanyalTestScripts", 1, true, NScript::CDanyalTestScriptsScript::Alloc, NScript::CDanyalTestScriptsScript::StatGetMasterScriptDataAlloc()), "");
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("ProximityPodiumTest", 1, false, NScript::CProximityPodiumTestScript::Alloc, NScript::CDanyalTestScriptsScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("ProximityPodiumTest", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("HitSwitchTest", 1, false, NScript::CHitSwitchTestScript::Alloc, NScript::CDanyalTestScriptsScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("HitSwitchTest", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("GuildSealTriggerTest", 1, false, NScript::CGuildSealTriggerTestScript::Alloc, NScript::CDanyalTestScriptsScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("GuildSealTriggerTest", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("PersonalScript_Aardvark", 1, false, NScript::CPersonalScript_AardvarkScript::Alloc, NScript::CDanyalTestScriptsScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("PersonalScript_Aardvark", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("HeroTurningDebugger", 1, false, NScript::CHeroTurningDebuggerScript::Alloc, NScript::CDanyalTestScriptsScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("HeroTurningDebugger", (EScriptActiveStatus)0);
    AddScript(CScriptInfo("joss_test_scripts", 1, true, NScript::Cjoss_test_scriptsScript::Alloc, NScript::Cjoss_test_scriptsScript::StatGetMasterScriptDataAlloc()), "");
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("guy_script", 1, false, NScript::Cguy_scriptScript::Alloc, NScript::Cjoss_test_scriptsScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("guy_script", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("bandit_level_cutscene", 1, false, NScript::Cbandit_level_cutsceneScript::Alloc, NScript::Cjoss_test_scriptsScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("bandit_level_cutscene", (EScriptActiveStatus)0);
    AddScript(CScriptInfo("Empty", 1, true, NScript::CEmptyScript::Alloc, NScript::CEmptyScript::StatGetMasterScriptDataAlloc()), "");
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("TestTheInventory", 1, false, NScript::CTestTheInventoryScript::Alloc, NScript::CEmptyScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("TestTheInventory", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("PersonalScript_Empty", 1, false, NScript::CPersonalScript_EmptyScript::Alloc, NScript::CEmptyScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("PersonalScript_Empty", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("TestNewNav1", 1, false, NScript::CTestNewNav1Script::Alloc, NScript::CEmptyScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("TestNewNav1", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("TestNewNav2", 1, false, NScript::CTestNewNav2Script::Alloc, NScript::CEmptyScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("TestNewNav2", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("TestNewNav3", 1, false, NScript::CTestNewNav3Script::Alloc, NScript::CEmptyScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("TestNewNav3", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("TestNewNav4", 1, false, NScript::CTestNewNav4Script::Alloc, NScript::CEmptyScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("TestNewNav4", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("TestNewNav5", 1, false, NScript::CTestNewNav5Script::Alloc, NScript::CEmptyScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("TestNewNav5", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("TestQuestCard1", 1, false, NScript::CTestQuestCard1Script::Alloc, NScript::CEmptyScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("TestQuestCard1", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("TestQuestCard2", 1, false, NScript::CTestQuestCard2Script::Alloc, NScript::CEmptyScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("TestQuestCard2", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("TestQuestCard3", 1, false, NScript::CTestQuestCard3Script::Alloc, NScript::CEmptyScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("TestQuestCard3", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("MarkTestScripts", 1, false, NScript::CMarkTestScriptsScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("MarkTestScripts", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("BenTestScripts", 1, false, NScript::CBenTestScriptsScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("BenTestScripts", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("TonyTestScripts", 1, false, NScript::CTonyTestScriptsScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("TonyTestScripts", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("ShowTargetedThingHealth", 1, false, NScript::CShowTargetedThingHealthScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("ShowTargetedThingHealth", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("OracleMinigameTest", 1, false, NScript::COracleMinigameTestScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("OracleMinigameTest", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("Q_CinemaTest", 1, false, NScript::CQ_CinemaTestScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("Q_CinemaTest", (EScriptActiveStatus)0);
    CheckSectionIsNotAlreadyRegistered("");
    AddScript(CScriptInfo("CS_PlayCutscene", 1, false, NScript::CCS_PlayCutsceneScript::Alloc, NScript::CQ_SunnyvaleMasterScript::StatGetMasterScriptDataAlloc()), "");
    SetScriptActiveStatus("CS_PlayCutscene", (EScriptActiveStatus)0);
}


static unsigned int AllocAddress(CScriptInfo::tAllocFunc f)
{
    unsigned int found = 0; int n = 0;
    for (size_t i = 0; i < sizeof(g_allocs) / sizeof(g_allocs[0]); ++i)
        if (g_allocs[i].Func == f) { found = g_allocs[i].Retail; ++n; }
    return n == 1 ? found : 0xFFFFFFFFu;
}
static unsigned int DataAllocAddress(CScriptInfo::tMasterScriptDataAllocFunc f)
{
    if (f == 0) return 0;
    unsigned int found = 0; int n = 0;
    for (size_t i = 0; i < sizeof(g_data_allocs) / sizeof(g_data_allocs[0]); ++i)
        if (g_data_allocs[i].Func == f) { found = g_data_allocs[i].Retail; ++n; }
    return n == 1 ? found : 0xFFFFFFFFu;
}

int main()
{
    const int expected_count = (int)(sizeof(g_expected) / sizeof(g_expected[0]));
    static CScriptInfoManager manager;
    const int live_before = g_live;
    manager.RegisterScripts();

    // (1) the call sequence
    if (g_count != expected_count) Fail("number of calls", g_count);
    for (int i = 0; i < expected_count && i < g_count; ++i) {
        const SExpected& e = g_expected[i]; const SRecorded& r = g_rec[i];
        if (r.Kind != e.Kind) { Fail("call kind", i); continue; }
        if (std::strcmp(r.Text, e.Text) != 0) Fail("text / name", i);
        switch (e.Kind) {
        case EV_PROGRESS:
            if (r.Progress != -1.0f || r.Flags != 0) Fail("progress arguments", i);
            break;
        case EV_CHECK:
            break;
        case EV_ADD:
            if (std::strcmp(r.Section, e.Section) != 0) Fail("code section", i);
            if (AllocAddress(r.Alloc) != e.Alloc) Fail("script allocator", i);
            if (DataAllocAddress(r.DataAlloc) != e.DataAlloc) Fail("master data allocator", i);
            if (r.Value != e.Value) Fail("id", i);
            if (r.Master != e.Master) Fail("master flag", i);
            break;
        case EV_STATUS:
            if (r.Value != e.Value) Fail("status", i);
            break;
        }
    }
    // every script is checked, added and given its status in that order, with the same section and name
    for (int i = 0; i + 2 < g_count && i + 2 < MAX_EVENTS; ++i) {
        if (g_rec[i].Kind != EV_CHECK) continue;
        if (g_rec[i + 1].Kind != EV_ADD || g_rec[i + 2].Kind != EV_STATUS) { Fail("check / add / status triple", i); continue; }
        if (std::strcmp(g_rec[i].Text, g_rec[i + 1].Section) != 0) Fail("checked section is the added section", i);
        if (std::strcmp(g_rec[i + 1].Text, g_rec[i + 2].Text) != 0) Fail("status goes to the added script", i);
        if (g_rec[i + 1].Master != 0) Fail("a script with a section is not a master script", i);
    }

    // (2) the table
    if (manager.ScriptCount != 161) Fail("161 scripts", manager.ScriptCount);
    if (manager.SectionCount != 118) Fail("118 code sections", manager.SectionCount);
    int masters = 0;
    for (int i = 0; i < manager.ScriptCount; ++i) {
        const CScriptInfo& s = manager.Scripts[i];
        if (s.ID != 1) Fail("script id 1", i);
        if (s.MasterScript) {
            ++masters;
            if (s.MasterScriptDataAlloc == 0) Fail("a master script has a data allocator", i);
        }
        for (int j = 0; j < i; ++j) {
            if (std::strcmp(manager.Scripts[j].Name.Text, s.Name.Text) == 0) Fail("unique script name", i);
            if (manager.Scripts[j].AllocFunc == s.AllocFunc) Fail("unique script allocator", i);
        }
        if (std::strcmp(manager.ScriptNames[i].Text, s.Name.Text) != 0) Fail("map key is the script name", i);
        // a script's data allocator is none or that of a master script in the table
        if (s.MasterScriptDataAlloc != 0) {
            int owners = 0;
            for (int j = 0; j < manager.ScriptCount; ++j)
                if (manager.Scripts[j].MasterScript && manager.Scripts[j].MasterScriptDataAlloc == s.MasterScriptDataAlloc) ++owners;
            if (owners != 1) Fail("data allocator belongs to exactly one master script", i);
        }
    }
    if (masters != 5) Fail("5 master scripts", masters);
    for (int i = 0; i < manager.SectionCount; ++i)
        for (int j = 0; j < i; ++j)
            if (std::strcmp(manager.CodeSectionNames[j].Text, manager.CodeSectionNames[i].Text) == 0) Fail("unique code section", i);

    // (3) the stored pointers are the functions retail stores: call each through the table
    static char game_storage[16];     // stands for the script interface the allocators take by reference (never read here)
    for (int i = 0; i < manager.ScriptCount; ++i) {
        const CScriptInfo& s = manager.Scripts[i];
        CScriptBase* made = s.AllocFunc(0, *(CGameScriptInterfaceBase*)game_storage);
        int hits = 0;
        for (size_t k = 0; k < sizeof(g_allocs) / sizeof(g_allocs[0]); ++k)
            if ((char*)made == g_allocs[k].Tag && g_allocs[k].Func == s.AllocFunc) ++hits;
        if (hits != 1) Fail("allocator returns its own script", i);
        if (s.MasterScriptDataAlloc != 0) {
            CScriptDataBase* data = s.MasterScriptDataAlloc();
            int dhits = 0;
            for (size_t k = 0; k < sizeof(g_data_allocs) / sizeof(g_data_allocs[0]); ++k)
                if ((char*)data == g_data_allocs[k].Tag && g_data_allocs[k].Func == s.MasterScriptDataAlloc) ++dhits;
            if (dhits != 1) Fail("data allocator returns its own data", i);
        }
    }
    if (sizeof(g_allocs) / sizeof(g_allocs[0]) != 161) Fail("161 allocators in the model", 0);

    // (4) temporaries: only the strings held by the table stay alive (the table's own slots were alive before the call)
    if (g_live != live_before) Fail("every temporary CCharString is destroyed", g_live - live_before);
    // per statement: progress 1, check 1, status 1, add 4 (name, CScriptInfo::Name, section, + the model AddScript copies none)
    if (g_built - 3 * MAX_SCRIPTS != 6 + 156 + 156 + 161 * 3) Fail("number of CCharString objects built", g_built);

    // (5) constructor order: the name is copied before the other members are stored
    {
        g_stamp = 0; g_name_copied_at = 0;
        CCharString name("OrderCheck");
        CScriptInfo info(name, 7, true, NScript::CEmptyScript::Alloc, NScript::CEmptyScript::StatGetMasterScriptDataAlloc());
        if (g_name_copied_at != 1 || g_stamp != 2) Fail("constructor order", 0);
        if (std::strcmp(info.Name.Text, "OrderCheck") != 0 || info.ID != 7 || !info.MasterScript || info.AllocFunc != NScript::CEmptyScript::Alloc
            || DataAllocAddress(info.MasterScriptDataAlloc) != 0x00CD4AC0u) Fail("constructor stores", 0);
    }

    if (g_fail) { std::printf("00cd52d0_TEST FAIL\n"); return 1; }
    std::printf("00cd52d0_TEST PASS\n");
    return 0;
}
