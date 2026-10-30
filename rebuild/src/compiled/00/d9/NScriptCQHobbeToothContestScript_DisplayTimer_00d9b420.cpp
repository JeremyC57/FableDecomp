// CQ_HobbeToothContestScript::DisplayTimer() -- run the contest clock: show the timer and the hero / Whisper tooth counters, and while the clock runs keep the counters current and, whenever the hero is away from Orchard Farm East, credit Whisper a tooth every 5 seconds; then wait for the hero to return and take the HUD elements down
// Leaf: ?DisplayTimer@CQ_HobbeToothContestScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D9B420, length 913 bytes; called from 0x00D9AC69.
// Callees: __ftol2 0x00BFEA70, CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0,
//   CScriptBase::IsActiveThreadTerminating 0x00CB7940, ~CTimer 0x00CD4470 (the out-of-line copy); the timer service
//   g_pGameScriptInterface 0x0143E8F8: RegisterTimer (87), DeregisterTimer (88, the inline ~CTimer copies), SetTimer (89),
//   GetTimer (90); through the interface (+0x40): NewScriptFrame (7), IsLevelLoaded (11), DisplayQuestInfo (321),
//   AddQuestInfoTimer (326), AddQuestInfoCounter (327), UpdateQuestInfoCounter (335), RemoveQuestInfoElement (338).
// Data: pGlobalScriptDef 0x0143E90C HOB_TotalTime (+0xF20, float).
// Strings: "HUD_CLOCK_ICON" 0x012CA7C8, "HUD_HERO_ICON" 0x012D6824, "HUD_WHISPER_ICON" 0x012CEBC8, "OrchardFarmEast"
//   0x012D67B8.
// Layout (retail = Ego_r.pdb - 0x14): NumberOfHobbeTeethCollectedByHero +0x4c, NumberOfHobbeTeethCollectedByWhisper +0x50.
// Shape: CTimer model of the landed NScript::CQ_HobbeCaveScript::CHobbeLarder::Main 0x00D8F6D0 (inline ctor and dtor).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CTimer;
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8
class CTimer { public:                                         // 4 bytes: an index into the global script timers
    long TimerIndex;
    CTimer() { TimerIndex = g_pGameScriptInterface->RegisterTimer(); }
    ~CTimer() { g_pGameScriptInterface->DeregisterTimer(TimerIndex); }      // out-of-line copy 0x00CD4470
    void SetTime(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); }
    long GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
};
class CScriptDef { public:
    unsigned char _pad[0xf20];
    float HOB_TotalTime;                                // +0xF20
};
extern CScriptDef* pGlobalScriptDef;                        // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
class CScriptBase { public:                                    // retail 0x40 bytes
    unsigned char _body[0x40];
public:
    bool IsActiveThreadTerminating();   // 0x00CB7940
};
namespace NScript {
class CQ_HobbeToothContestScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void*                 Data;                         // +0x44
    long NumberOfHobbesInTotal;                         // +0x48
    long NumberOfHobbeTeethCollectedByHero;             // +0x4c
    long NumberOfHobbeTeethCollectedByWhisper;          // +0x50
    void DisplayTimer();
};
void CQ_HobbeToothContestScript::DisplayTimer()
{
    CTimer contestTimer;
    contestTimer.SetTime((long)pGlobalScriptDef->HOB_TotalTime);
    Game->DisplayQuestInfo(true);
    long timerElement = Game->AddQuestInfoTimer(contestTimer, "HUD_CLOCK_ICON", 1.0f);
    long heroElement = Game->AddQuestInfoCounter("HUD_HERO_ICON", 0, 1.0f);
    long whisperElement = Game->AddQuestInfoCounter("HUD_WHISPER_ICON", 0, 1.0f);
    CTimer whisperTimer;
    whisperTimer.SetTime(10);
    bool heroInArea = true;
    while (contestTimer.GetTime() > 0) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        Game->UpdateQuestInfoCounter(heroElement, NumberOfHobbeTeethCollectedByHero, -1);
        Game->UpdateQuestInfoCounter(whisperElement, NumberOfHobbeTeethCollectedByWhisper, -1);
        if (!Game->IsLevelLoaded("OrchardFarmEast")) {
            if (IsActiveThreadTerminating())
                return;
            if (heroInArea == true) {
                if (IsActiveThreadTerminating())
                    return;
                whisperTimer.SetTime(5);
                heroInArea = false;
            }
            if (whisperTimer.GetTime() <= 0) {
                if (IsActiveThreadTerminating())
                    return;
                ++NumberOfHobbeTeethCollectedByWhisper;
                whisperTimer.SetTime(5);
            }
        } else {
            if (IsActiveThreadTerminating())
                return;
            if (!heroInArea) {
                if (IsActiveThreadTerminating())
                    return;
                heroInArea = true;
            }
        }
    }
    if (IsActiveThreadTerminating())
        return;
    while (!Game->IsLevelLoaded("OrchardFarmEast")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
#pragma inline_depth(0)
    if (IsActiveThreadTerminating())
        return;
    Game->RemoveQuestInfoElement(timerElement);
    Game->RemoveQuestInfoElement(whisperElement);
    Game->RemoveQuestInfoElement(heroElement);
    Game->DisplayQuestInfo(false);
}
#pragma inline_depth()
}