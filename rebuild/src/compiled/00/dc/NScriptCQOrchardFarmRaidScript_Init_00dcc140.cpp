// CQ_OrchardFarmRaidScript::Init() -- register the farm as a quest region of whichever side's quest is active, reset the master-data flags, the crate count, timers and mission state, pair the two crate teams as enemies with no carriers, set the fail reasons, then pick the hero's team, text prefix, objective and the minimap routes for the evil or the good quest
// Leaf: Init
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DCC140, 1298 bytes (int3 padded to 1312).
// Callees: CCharString 0x0099EBF0 / ~ 0x0099EAE0 / operator=(const char*) 0x0099EFE0, CWideString::operator=(const wchar_t*)
//   0x0099B800, CBaseClass 0x0099A2D0 / ~ 0x0099A2E0, operator delete 0x00BFE9BC; g_pGameScriptInterface 0x0143E8F8
//   SetTimer (slot 89); Game slots 271 AddQuestRegion, 284 IsQuestActive, 296 SetQuestCardObjective, 354
//   MiniMapAllowRouteBetweenRegions.
// Layout (retail = Ego_r.pdb - 0x14, and -4 more after the 12-byte STLport CrateList): CrateCount +0x48, CommentTimer +0x4c,
//   DoneIntroduction +0x50 .. WhisperSpawned +0x52, MissionSucceeded +0x60, MissionFailed +0x64, FailReasons +0x68, HeroTeam
//   +0x78, BanditWavesSpawned +0x7c, TextSystemScriptCode +0x84, HeroMetWhisperBeforeFarm +0x88 .. WhisperInCutscene +0x8b,
//   Teams[2] +0x8c (0x40 each: TeamCrateCarrier +0x30, EnemyTeam +0x3c). CQ_SunnyvaleMasterData (no shift): OFBRCratesStolen
//   +0x109, OFBR_NoCratesWereStolen +0x10a, OrchardFarmBanditKilled +0x118, OrchardFarmGuardKilled +0x119.
// Shape: `X = CScriptThing();` is the landed CV_BordelloScript (0x00E399D0) temporary + inline counted operator= model.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const char* text);           // 0x0099EFE0
};
class CWideString { public:
    void* PStringData;
    const CWideString& operator=(const unsigned short* text);        // 0x0099B800
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
struct SScriptThingInfo {
    long m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
};
class CScriptThing : public CBaseClass { public:                           // vtable 0x01238C8C, 12 bytes
    void* PImp_Data;
    SScriptThingInfo* PImp_Info;
    CScriptThing() : PImp_Data(0), PImp_Info(0) { }
    ~CScriptThing()
    {
        if (PImp_Info) {
            --PImp_Info->m_references;
            if (PImp_Info->m_references == 0) {
                PImp_Info->m_pfnDestroy(PImp_Info->m_pObject);
                delete PImp_Info;
            }
        }
        PImp_Data = 0;
        PImp_Info = 0;
    }
    CScriptThing& operator=(const CScriptThing& other)
    {
        SScriptThingInfo* info = other.PImp_Info;
        void* data = other.PImp_Data;
        if (PImp_Info != info) {
            if (PImp_Info) {
                --PImp_Info->m_references;
                if (PImp_Info->m_references == 0) {
                    PImp_Info->m_pfnDestroy(PImp_Info->m_pObject);
                    delete PImp_Info;
                }
            }
            PImp_Data = data;
            PImp_Info = info;
            if (info)
                ++info->m_references;
        }
        return *this;
    }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer& CTimer::operator=(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); return *this; }
struct CQ_SunnyvaleMasterData {
    unsigned char _pad[0x109];
    bool OFBRCratesStolen;                              // +0x109
    bool OFBR_NoCratesWereStolen;                       // +0x10a
    unsigned char _pad10b[0x118 - 0x10b];
    bool OrchardFarmBanditKilled;                       // +0x118
    bool OrchardFarmGuardKilled;                        // +0x119
};
struct CCrateTeamManager {                              // 0x40 bytes
    long StateCounter[6];                               // +0x00
    long MemberCount;                                   // +0x18
    CTimer TeamReinforcementsTimer;                     // +0x1c
    long MaxTeamMembers;                                // +0x20
    unsigned char CrateDropPos[0x0c];                   // +0x24 (CScriptThing)
    CScriptThing TeamCrateCarrier;                      // +0x30
    CCrateTeamManager* EnemyTeam;                       // +0x3c
};
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_OrchardFarmRaidScript : public CScriptBase {
public:
    CGameScriptInterface*   Game;                       // +0x40
    CQ_SunnyvaleMasterData* PMasterData;                // +0x44
    long                    CrateCount;                 // +0x48
    CTimer                  CommentTimer;               // +0x4c
    bool                    DoneIntroduction;           // +0x50
    bool                    HeroAtWrongEntrance;        // +0x51
    bool                    WhisperSpawned;             // +0x52
    void*                   CrateList[3];               // +0x54 (12-byte STLport vector<CScriptThing>)
    bool                    MissionSucceeded;           // +0x60
    long                    MissionFailed;              // +0x64
    CWideString             FailReasons[4];             // +0x68
    long                    HeroTeam;                   // +0x78
    long                    BanditWavesSpawned;         // +0x7c
    CTimer                  RemindHeroOfObjectivesTimer;// +0x80
    CCharString             TextSystemScriptCode;       // +0x84
    bool                    HeroMetWhisperBeforeFarm;   // +0x88
    bool                    ShownCombatMultiplierTutorial;  // +0x89
    bool                    ShownCombatFlourishTutorial;    // +0x8a
    bool                    WhisperInCutscene;          // +0x8b
    CCrateTeamManager       Teams[2];                   // +0x8c
    void Init();
};
} // namespace NScript
using NScript::CQ_OrchardFarmRaidScript;

void NScript::CQ_OrchardFarmRaidScript::Init()
{
    if (Game->IsQuestActive("Q_OrchardFarmRaidGood"))
        Game->AddQuestRegion("Q_OrchardFarmRaidGood", "OrchardFarm");
    else
        Game->AddQuestRegion("Q_OrchardFarmRaidEvil", "OrchardFarm");
    PMasterData->OrchardFarmGuardKilled = false;
    PMasterData->OrchardFarmBanditKilled = false;
    HeroMetWhisperBeforeFarm = false;
    CrateCount = 0;
    PMasterData->OFBRCratesStolen = false;
    PMasterData->OFBR_NoCratesWereStolen = false;
    CommentTimer = 0;
    DoneIntroduction = false;
    HeroAtWrongEntrance = false;
    WhisperSpawned = false;
    MissionSucceeded = false;
    MissionFailed = 0;
    BanditWavesSpawned = 0;
    ShownCombatMultiplierTutorial = false;
    ShownCombatFlourishTutorial = false;
    WhisperInCutscene = false;
    Teams[0].EnemyTeam = &Teams[1];
    Teams[1].EnemyTeam = &Teams[0];
    Teams[0].TeamCrateCarrier = CScriptThing();
    Teams[1].TeamCrateCarrier = CScriptThing();
    FailReasons[0] = L"PROBLEM: Tell Ben problem with Orchard Farm fail reasons";
    FailReasons[1] = L"TEXT_QST_051_FAILED_HERO_KILLED";
    FailReasons[2] = L"TEXT_QST_051_FAILED_CRATES_STOLEN";
    FailReasons[3] = L"TEXT_QST_051_FAILED_TEAM_KILLED";
    if (Game->IsQuestActive("Q_OrchardFarmRaidEvil")) {
        HeroTeam = 1;
        TextSystemScriptCode = "TEXT_QST_051_";
        Game->SetQuestCardObjective("Q_OrchardFarmRaidEvil", "TEXT_QUEST_PROTECT_FARM_EVIL_OBJECTIVE_01", "", "HeroGuildComplexInside");
        Game->MiniMapAllowRouteBetweenRegions("OrchardFarm", "GreatwoodLake", false);
        Game->MiniMapAllowRouteBetweenRegions("OrchardFarm", "GreatwoodEntrance", true);
    } else if (Game->IsQuestActive("Q_OrchardFarmRaidGood")) {
        HeroTeam = 0;
        TextSystemScriptCode = "TEXT_QST_052_";
        Game->SetQuestCardObjective("Q_OrchardFarmRaidGood", "TEXT_QUEST_PROTECT_FARM_OBJECTIVE_01", "", "Greatwood");
        Game->MiniMapAllowRouteBetweenRegions("OrchardFarm", "GreatwoodEntrance", false);
        Game->MiniMapAllowRouteBetweenRegions("OrchardFarm", "GreatwoodLake", true);
    }
}