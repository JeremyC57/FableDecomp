// CQ_BanditCampScript::WatchForEndOfScript() -- script thread: whenever the Q_TraderConflictEvil quest is active, suspend the V_ExposeMayorRhodri quest until Q_TraderConflictEvil is no longer active, then re-activate it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E75EF0, length 329 bytes (+ int3 padding), plain `ret`.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CCharString(const char*, long) 0x0099EBF0 / ~CCharString
// 0x0099EAE0; through the quest's interface (+0x40): NewScriptFrame (slot 7), ActivateQuest (276), DeactivateQuest
// (280), IsQuestActive (284).
// Strings: "Q_TraderConflictEvil" 0x012C52DC, "V_ExposeMayorRhodri" 0x012E978C.
// Shape: `while (!IsActiveThreadTerminating())` around one `if (IsQuestActive(..))` arm holding the script wait idiom;
// the quest names are argument temporaries. The inner wait is the scripts' WAIT_UNTIL(cond) = `while (!(cond))` over
// `!IsQuestActive(..)`: the double negation is what gives retail's `test al,al ; setne bl` (a plain
// `while (IsQuestActive(..))` gives `mov bl,al`, as the outer `if` does).
class CCharString { public:
    void* m_pStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

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
class CQ_BanditCampScript : public CScriptBase { public:
    CGameScriptInterface* GSI;                          // +0x40
    void*                 Data;                         // +0x44
    void WatchForEndOfScript();
};
void CQ_BanditCampScript::WatchForEndOfScript()
{
    while (!IsActiveThreadTerminating()) {
        if (GSI->IsQuestActive("Q_TraderConflictEvil")) {
            if (IsActiveThreadTerminating())
                return;
            GSI->DeactivateQuest("V_ExposeMayorRhodri", 0);
            while (!(!GSI->IsQuestActive("Q_TraderConflictEvil"))) {
                GSI->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            GSI->ActivateQuest("V_ExposeMayorRhodri");
        }
        GSI->NewScriptFrame();
    }
}
}