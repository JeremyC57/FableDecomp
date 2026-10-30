// CQ_RansomVictimScript::WatchForTermination() -- wait for the ransom-victim mission to end, put the chief's hut back under normal house rules, record the kidnappers-killed boast, complete or fail the active quest, deactivate the follow-up quests, re-enable the Witchwood creature generators and retire the quest
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DE14A0, length 711 bytes. Identity: the thread spawned (CSpawnedFunc
//   vtable 0x012DCF1C) by the ransom-victim quest's Main at 0x00DE0CAC; the strings name the RansomVictim quests (manifest
//   "CQ_HobbeCaveScript::WatchForTermination" is propagated).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CWideString(const wchar_t*) 0x0099B6B0 /
//   ~CWideString 0x0099B510 (L"TEXT_QST_B06_QUEST_FAILED_VICTIM_DIED" 0x012DD138), ~CScriptThing 0x004AA840,
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940; through the interface (+0x40): NewScriptFrame (7),
//   GetThingWithScriptName (72), DeactivateQuest (280), DeactivateQuestLater (281), SetQuestAsCompleted (288),
//   SetQuestAsFailed (289), FadeScreenIn (374), OverrideAutomaticHouseLocking (431), SetBuyableHouseAsScripted (433),
//   SetCreatureGeneratorsEnabled (558), GetActiveQuestName (655).
// Layout (retail = Ego_r.pdb CQ_RansomVictimScript - 0x14): MissionSucceeded +0x49, MissionFailed +0x4a,
//   KidnappersKilled +0x60; PMasterData +0x44 (CQ_SunnyvaleMasterData::RansomVictimKidnappersKilled +0x12b).
// Shape: every literal goes straight to a `const CCharString&` / `const CWideString&` parameter; the house handle is a
//   by-value CScriptThing temporary destroyed out of line.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CWideString { public:
    void* PStringData;
    CWideString(const unsigned short* text);            // 0x0099B6B0 (wchar_t is unsigned short under VC7.1)
    ~CWideString();                                     // 0x0099B510
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
struct CQ_SunnyvaleMasterData {
    unsigned char _pad0[0x12b];
    bool RansomVictimKidnappersKilled;                  // +0x12b
};
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_RansomVictimScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    CQ_SunnyvaleMasterData* PMasterData;                // +0x44
    bool QuestStartScreened;                            // +0x48
    bool MissionSucceeded;                              // +0x49
    bool MissionFailed;                                 // +0x4a
    unsigned char _pad4b[0x60 - 0x4b];
    long KidnappersKilled;                              // +0x60
    void WatchForTermination();
};
} // namespace NScript
using NScript::CQ_RansomVictimScript;

void NScript::CQ_RansomVictimScript::WatchForTermination()
{
    while (!MissionSucceeded && !MissionFailed) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetBuyableHouseAsScripted(Game->GetThingWithScriptName("ChiefsHut"), false);
    Game->OverrideAutomaticHouseLocking(Game->GetThingWithScriptName("ChiefsHut"), false);
    if (KidnappersKilled == 3) {
        if (IsActiveThreadTerminating())
            return;
        PMasterData->RansomVictimKidnappersKilled = true;
    }
    if (MissionSucceeded) {
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestAsCompleted(Game->GetActiveQuestName(), true, true, false);
    } else {
        if (IsActiveThreadTerminating())
            return;
        Game->SetQuestAsFailed(Game->GetActiveQuestName(), true, L"TEXT_QST_B06_QUEST_FAILED_VICTIM_DIED", true);
    }
    Game->DeactivateQuest("Q_RansomVictimNaturesRevenge", 0);
    Game->DeactivateQuest("Q_RansomVictimRevenge", 0);
    Game->DeactivateQuest("Q_RansomVictimStarted", 0);
    Game->SetCreatureGeneratorsEnabled("Witchwood2", true);
    Game->SetCreatureGeneratorsEnabled("Witchwood3", true);
    Game->SetCreatureGeneratorsEnabled("Witchwood4", true);
    Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
    Game->FadeScreenIn();
}