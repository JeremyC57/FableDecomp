// CV_ParanoidWhispersScript::CheckForNecklaceAndQuit() -- each frame, once the hero holds Lady Grey's stolen necklace (and the timer has run out), point the Mayor's Invitation quest card at Bowerstone and deactivate the active quest
// Leaf: ?CheckForNecklaceAndQuit@CV_ParanoidWhispersScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00EBF1E0, 432 bytes, plain `ret`.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CScriptBase::IsActiveThreadTerminating
//   0x00CB7940; the global script interface 0x0143E8F8 slots RegisterTimer (87), DeregisterTimer (88), SetTimer (89),
//   GetTimer (90) behind the inline CTimer; through the script's interface (+0x40) NewScriptFrame (slot 7), GetHero
//   (slot 70), IsObjectInThingsPossession (slot 184), DeactivateQuestLater (slot 281), SetQuestCardObjective (slot 296),
//   GetActiveQuestName (slot 655).
// Strings: "OBJECT_LADY_GREY_STOLEN_NECKLACE" 0x012F0A78, "" 0x0122D70E, "BowerstonePosh" 0x012C6C6C,
//   "TEXT_QUEST_MAYORS_INVITATION_OBJECTIVE_06" 0x012F0FE4, "V_MayorsInvitation" 0x012C5008.
// Shape: the timer index lives in edi; the found test is one named bool whose possession-name temporary is built under
//   a construction flag (zeroed at entry); the timer's inline destructor is expanded at both exits.
// Layout: CScriptBase 0x40 bytes, interface pointer +0x40.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
class CScriptThing;
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

extern CGameScriptInterface* g_pGameScriptInterface;        // 0x0143E8F8
// CTimer: the retail model from engine/CTimer.h (Ego_r.pdb CTimer {long TimerIndex}, 0x4; the setter is the retail
//   operator=(long), Ego_r 0x00CCE720). Bodies the row expands inline follow the include.
#include "engine/CTimer.h"
inline CTimer::CTimer() : TimerIndex(g_pGameScriptInterface->RegisterTimer()) { }
inline CTimer::~CTimer() { g_pGameScriptInterface->DeregisterTimer(TimerIndex); }
inline long CTimer::GetTime() const { return g_pGameScriptInterface->GetTimer(TimerIndex); }
inline CTimer& CTimer::operator=(long time) { g_pGameScriptInterface->SetTimer(TimerIndex, time); return *this; }
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct CQuestScriptBase : CScriptBase {
    CGameScriptInterface* Game;                             // +0x40
    void*                 Data;                             // +0x44
};
namespace NScript {
class CV_ParanoidWhispersScript : public CQuestScriptBase {
public:
    void CheckForNecklaceAndQuit();
};
} // namespace NScript
using NScript::CV_ParanoidWhispersScript;

void NScript::CV_ParanoidWhispersScript::CheckForNecklaceAndQuit()
{
    CTimer timer;
    for (;;) {
        if (IsActiveThreadTerminating())
            break;
        bool found = timer.GetTime() == 0
            && Game->IsObjectInThingsPossession("OBJECT_LADY_GREY_STOLEN_NECKLACE", Game->GetHero());
        if (found) {
            if (IsActiveThreadTerminating())
                return;
            timer = 3;
            Game->SetQuestCardObjective("V_MayorsInvitation", "TEXT_QUEST_MAYORS_INVITATION_OBJECTIVE_06", "BowerstonePosh", "");
            Game->DeactivateQuestLater(Game->GetActiveQuestName(), 0);
        }
        Game->NewScriptFrame();
    }
}