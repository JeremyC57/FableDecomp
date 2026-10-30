// CQ_DragonBossFightScript::JackTaunts() -- script thread: every 20 seconds of script timer, Jack taunts the hero with the next of the TEXT_QST_B05_JACK_TAUNT_10 .. _50 lines (a one-line conversation spoken by and to the hero), five taunts in all
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00D26A50, length 325 bytes, plain `ret`.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; the global game script interface 0x0143E8F8 (the CTimer
// local): RegisterTimer (slot 87), DeregisterTimer (88), SetTimer (89), GetTimer (90); IntToCharString(long)
// 0x0099F830 and operator+(const char*, const CCharString&) 0x0099F690 (both __fastcall with the hidden result in
// ecx), ~CCharString 0x0099EAE0; through the quest's interface (+0x40): NewScriptFrame (7), GetHero (70),
// AddNewConversation (364), AddLineToConversation (366).
// Strings: "TEXT_QST_B05_JACK_TAUNT_" 0x012CB9B8.
// Shape: the landed CV_MurderTwistFinalScript::WaitForEnflameReactionIfNecessary (0x00EB0700) CTimer local (index kept in
// edi and its frame slot, deregistered on every exit); the taunt counter only advances when a taunt is spoken, so the
// loop is `while (taunt < 60)` with the step inside the timer arm (the first test folds away); the line is
// `"TEXT_..." + IntToCharString(taunt)` built as two argument temporaries; the counter is declared before the first
// SetTime (VC then schedules its `mov ebx,10` ahead of that call).
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (struct->class; already NScript). Same bytes.
class CScriptThing;
class CCharString { public:
    void* m_pStringData;
    ~CCharString();                                     // 0x0099EAE0
};
CCharString __fastcall GFIntToCharString(long);       // 0x0099F830
CCharString __fastcall operator+(const char* left, const CCharString& right);   // 0x0099F690

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern CGameScriptInterface* g_pGameScriptInterface;    // 0x0143E8F8

class CTimer { public:                                         // a script timer: the interface's timer index
    long Index;
    CTimer() : Index(g_pGameScriptInterface->RegisterTimer()) { }
    ~CTimer() { g_pGameScriptInterface->DeregisterTimer(Index); }
    void SetTime(long seconds) { g_pGameScriptInterface->SetTimer(Index, seconds); }
    long GetTime() { return g_pGameScriptInterface->GetTimer(Index); }
};

namespace NScript {
}   // namespace NScript (CScriptBase is global in retail)
class CScriptBase { public:                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};
namespace NScript {
class CQ_DragonBossFightScript : public CScriptBase {
public:
    CGameScriptInterface* GSI;                          // +0x40
    void*                 Data;                         // +0x44
    void JackTaunts();
};
void CQ_DragonBossFightScript::JackTaunts()
{
    CTimer timer;
    long taunt = 10;
    timer.SetTime(20);
    while (taunt < 60) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (timer.GetTime() == 0) {
            if (IsActiveThreadTerminating())
                return;
            long conversation = GSI->AddNewConversation(GSI->GetHero(), false, false);
            GSI->AddLineToConversation(conversation, "TEXT_QST_B05_JACK_TAUNT_" + GFIntToCharString(taunt), false,
                                       GSI->GetHero(), GSI->GetHero());
            timer.SetTime(20);
            taunt += 10;
        }
    }
    if (IsActiveThreadTerminating())
        return;
}
}