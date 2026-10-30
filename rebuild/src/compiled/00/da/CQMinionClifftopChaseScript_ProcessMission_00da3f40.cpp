// CQ_MinionClifftopChaseScript::ProcessMission() -- after the intro cutscene: switch off the Gibbet Woods / Deserted Farm creature generators, wait for the outcome, restore them, fade out and complete or fail the quest
// Leaf: ?ProcessMission@CQ_MinionClifftopChaseScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00DA3F40, 0x302 bytes, plain `ret`.
// Label: the manifest's "CV_HeroDuelScript::WatchForHeroEnteringCave" is propagated. The only reference is the CSpawnedFunc
//   built at 0x00DA3EE0 (vtable 0x012D716C) and the strings ("Q_MinionClifftopChase_Barriers", TEXT_QST_045_*) and members
//   fit CQ_MinionClifftopChaseScript; FableWin.pdb lists ProcessMission and WatchForTermination as its thread bodies.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CWideString(const unsigned short*)
//   0x0099B6B0 / ~CWideString 0x0099B510, IsActiveThreadTerminating 0x00CB7940; CGameScriptInterface slots 7
//   NewScriptFrame, 280 DeactivateQuest, 281 DeactivateQuestLater, 288 SetQuestAsCompleted, 289 SetQuestAsFailed,
//   373 FadeScreenOut, 374 FadeScreenIn, 376 Pause, 558 SetCreatureGeneratorsEnabled, 655 GetActiveQuestName,
//   697 StopOverrideMusic.
// Data: L"TEXT_QST_045_QUEST_FAILED_ARCH_DIED" 0x012D7378, L"TEXT_QST_045_QUEST_FAILED_TIME_RAN_OUT" 0x012D7328, L"" 0x0122D70C.
// Shape: the fade colour needs a user copy ctor to be built in its argument slot (push ecx; mov eax,esp).
//   The three failure arms each pass a wide literal to the `const CWideString&` reason; VC7.1 tail-merges them into
//   one CWideString construction + SetQuestAsFailed. The fade colour is a by-value CRGBColour built in its argument slot.
// Layout (retail = Ego_r.pdb - 0x14): MapKeeperSaidDialog +0x48, MissionFailed +0x49, MissionSucceeded +0x4a,
//   IsIntroCSFinished +0x4d, ReasonForFailure +0x64.
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
class CWideString { public:
    void* PStringData;
    CWideString(const unsigned short* text);             // 0x0099B6B0
    ~CWideString();                                      // 0x0099B510
};
class CRGBColour { public:                                      // B,G,R,A bytes (D3DCOLOR order)
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& other) { R = other.R; G = other.G; B = other.B; A = other.A; }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct CQ_MinionClifftopChaseScript : CScriptBase {
    CGameScriptInterface* Game;                          // +0x40
    void* PMasterData;                                   // +0x44
    bool MapKeeperSaidDialog;                            // +0x48
    bool MissionFailed;                                  // +0x49
    bool MissionSucceeded;                               // +0x4a
    bool QuestStartScreened;                             // +0x4b
    bool KilledMinionFieldGenerator;                     // +0x4c
    bool IsIntroCSFinished;                              // +0x4d
    unsigned char _pad4e[0x64 - 0x4e];
    long ReasonForFailure;                               // +0x64
    void ProcessMission();
};

void CQ_MinionClifftopChaseScript::ProcessMission()
{
    while (!IsIntroCSFinished) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetCreatureGeneratorsEnabled("GibbetWoods", false);
    Game->SetCreatureGeneratorsEnabled("DesertedFarm", false);
    while (!(MissionSucceeded || MissionFailed || MapKeeperSaidDialog)) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->StopOverrideMusic(false);
    if (MapKeeperSaidDialog) {
        if (IsActiveThreadTerminating())
            return;
        while (!MissionSucceeded) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
    }
    Game->SetCreatureGeneratorsEnabled("GibbetWoods", true);
    Game->SetCreatureGeneratorsEnabled("DesertedFarm", true);
    Game->FadeScreenOut(0.5f, 0.5f, CRGBColour(0, 0, 0, 255));
    if (MissionSucceeded) {
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, false, false);
    } else {
        if (IsActiveThreadTerminating())
            return;
        Game->DeactivateQuest("Q_MinionClifftopChase_Barriers", 0);
        if (ReasonForFailure == 0) {
            if (IsActiveThreadTerminating())
                return;
            Game->SetQuestAsFailed(Game->GetActiveQuestName(), true, (const unsigned short*)L"TEXT_QST_045_QUEST_FAILED_ARCH_DIED", true);
        } else if (ReasonForFailure == 1) {
            if (IsActiveThreadTerminating())
                return;
            Game->SetQuestAsFailed(Game->GetActiveQuestName(), true, (const unsigned short*)L"TEXT_QST_045_QUEST_FAILED_TIME_RAN_OUT", true);
        } else {
            if (IsActiveThreadTerminating())
                return;
            Game->SetQuestAsFailed(Game->GetActiveQuestName(), true, (const unsigned short*)L"", true);
        }
    }
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
    Game->Pause(0.3f);
    Game->FadeScreenIn();
}