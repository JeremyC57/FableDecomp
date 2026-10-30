// CQ_MinionCampScript::DoMission() -- the minion camp quest's mission thread: wait for Darkwood_5 and the intro, show the quest start screen, run phase one, wait for the portal or failure; on failure play CS_MINIONCAMP_FAILED (past stage 2) and fail the quest, otherwise complete it; then re-enable the Darkwood3 creature generators and deactivate the quest
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DA0230, length 925 bytes.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CWideString(const unsigned short*)
//   0x0099B6B0 / ~CWideString 0x0099B510, CScriptBase::IsActiveThreadTerminating 0x00CB7940, PhaseOne 0x00DA05D0,
//   scripted-thing handle ctor 0x007E72A0 / dtor 0x007E74D0 / operator= 0x008ABD10, cast map ctor 0x00CDBF70 / dtor
//   0x00CDBFB0 / operator[] 0x00CD3D2E, movie ctor 0x006E7B60 / dtor 0x006E7B80, RunCutsceneMacro 0x00CBFB7D (__fastcall);
//   the game script interface slots 7, 8, 11, 70, 281, 288, 289, 309, 370, 371, 374, 379, 558, 655.
// Data: L"TEXT_QST_B08_QUEST_FAILED_BRIARDIED" 0x012D6E78.
// Layout: the landed CQ_MinionCampScript ctor 0x00D9E210 (retail = Ego_r.pdb - 0x14); handle / map / movie classes as
//   the landed CMK_GHC_HERO3::Main 0x00D29DB0, with every constructor and destructor out of line in this TU.
// Shape: retail zeroes a temporary-construction flag at entry (the dead store to [esp+0x14] beside the first "Darkwood_5"
//   string) but never tests it: a spawn the optimiser removed. A constant-false `if (false) AddSpawnedFunction(new CSpawnedFunc(...), CCharString(""))`
//   reproduces it exactly (and the loop alignment padding that follows); the dead thread's name and target are unknown
//   (the Kraken / CinemaTest DoMission template spawns WatchForLeaving there).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
void* __cdecl operator new(unsigned int size);          // 0x00BFEA1A
// Layout: CSpawnedFuncBase is the retail polymorphic suspendable-process chain from the shared headers
//   (FABLE_RETAIL_SUSPENDABLE_PROCESS opt-in: CSpawnedFuncBase : CSuspendableProcessMicrothread : CASuspendableProcess :
//   CBaseClassNonCopyable : CBaseClass, 0x34 bytes, vtable 0x012C2F9C; ctor 0x00CDD450).
#define FABLE_RETAIL_SUSPENDABLE_PROCESS
#include "engine/CSpawnedFuncBase.h"
#include "engine/CSpawnedFunc.h"                         // vtable per instantiation; ctor inline, slot 4 SuspendableProcess

class CWideString { public:
    void* PStringData;
    CWideString(const unsigned short* text);            // 0x0099B6B0
    ~CWideString();                                     // 0x0099B510
};
class CScriptThing;
class CScriptGameResourceObjectScriptedThingBase { public:     // vtable 0x0127094C, 0x10 bytes
    virtual void h000();
    unsigned char _body[0xc];
    CScriptGameResourceObjectScriptedThingBase();       // 0x007E72A0
    virtual ~CScriptGameResourceObjectScriptedThingBase();      // 0x007E74D0
    CScriptGameResourceObjectScriptedThingBase& operator=(const CScriptGameResourceObjectScriptedThingBase& other);   // 0x008ABD10
};
class CScriptGameResourceObjectMovieBase { public:             // vtable 0x01260EF4, 0x10 bytes
    virtual void m000();
    unsigned char _body[0xc];
    CScriptGameResourceObjectMovieBase();               // 0x006E7B60
    virtual ~CScriptGameResourceObjectMovieBase();              // 0x006E7B80
};
struct CCutsceneCastMap {                               // map<CCharString, CScriptGameResourceObjectScriptedThingBase>
    void* _body[3];
    CCutsceneCastMap();                                 // 0x00CDBF70
    ~CCutsceneCastMap();                                // 0x00CDBFB0
    CScriptGameResourceObjectScriptedThingBase& operator[](const CCharString& key);   // 0x00CD3D2E
};
struct CCutsceneFlagMap;
struct CCutsceneArgMap;
void __fastcall RunCutsceneMacro(const CCharString& name, CCutsceneCastMap& cast, CCutsceneFlagMap* flags, CCutsceneArgMap* args, bool a5, bool a6);   // 0x00CBFB7D
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_MinionCampScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void* Data;                                         // +0x44
    bool QuestStartScreenShown;                         // +0x48
    bool IntroDone;                                     // +0x49
    bool BriarRoseAtAltar;                              // +0x4a
    bool MinionsSummoned;                               // +0x4b
    bool PortalSealed;                                  // +0x4c
    bool MissionFailed;                                 // +0x4d
    long NumberOfMinions;                               // +0x50
    long MaxNumberOfMinions;                            // +0x54
    float MinMinionSpawnPeriod;                         // +0x58
    float MaxMinionSpawnPeriod;                         // +0x5c
    long ChantTimer;                                    // +0x60 CTimer
    long ChantTimerBar;                                 // +0x64
    bool ChantTimerPaused;                              // +0x68
    long PausedTime;                                    // +0x6c
    long MinionSpawnTimer;                              // +0x70 CTimer
    long CurrentStage;                                  // +0x74 (MissionStage)
    void DoMission();
    void PhaseOne();                                    // 0x00DA05D0
    void WatchForLeaving();                             // not referenced by retail (dead spawn only)
};
void CQ_MinionCampScript::DoMission()
{
    while (!Game->IsLevelLoaded("Darkwood_5")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (false)
        AddSpawnedFunction(new CSpawnedFunc<CQ_MinionCampScript>("WatchForLeaving", 0, this, &CQ_MinionCampScript::WatchForLeaving), CCharString(""));
    CurrentStage = 1;
    while (!IntroDone) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (!QuestStartScreenShown) {
        if (IsActiveThreadTerminating())
            return;
        Game->KickOffQuestStartScreen(Game->GetActiveQuestName(), true, false);
        QuestStartScreenShown = true;
    }
    Game->FadeScreenIn();
    PhaseOne();
    Game->NewScriptFrame();
    if (IsActiveThreadTerminating())
        return;
    while (!MissionFailed && CurrentStage != 5) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    if (MissionFailed) {
        if (IsActiveThreadTerminating())
            return;
        if (CurrentStage > 2) {
            if (IsActiveThreadTerminating())
                return;
            CScriptGameResourceObjectScriptedThingBase hero;
            Game->StartScriptingEntity(Game->GetHero(), hero, (EScriptAIPriority)4);
            CCutsceneCastMap cast;
            cast["HERO"] = hero;
            CScriptGameResourceObjectMovieBase movie;
            Game->StartMovieSequence("", movie);
            CGameScriptInterface* game = Game;
            game->PauseAllNonScriptedEntities(true);
            Game->FixMovieSequenceCamera(true);
            RunCutsceneMacro("CS_MINIONCAMP_FAILED", cast, 0, 0, false, true);
            Game->FixMovieSequenceCamera(false);
            game->PauseAllNonScriptedEntities(false);
        }
        Game->SetQuestAsFailed(Game->GetActiveQuestName(), true, (const unsigned short*)L"TEXT_QST_B08_QUEST_FAILED_BRIARDIED", true);
    } else {
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, true, false);
        Game->FadeScreenIn();
    }
    Game->SetCreatureGeneratorsEnabled("Darkwood3", true);
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
}
}