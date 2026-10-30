// NScript::CQ_NewOakValeIntroScript::WatchForGotGold() -- watch thread: frame by frame until the hero carries more than 2 gold, then (unless terminating) set the active quest card's objective to TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_03
// Leaf: WatchForGotGold
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DBE2E0, length 212 bytes, plain `ret`.
// Identity: Ego_r.pdb `?WatchForGotGold@CQ_NewOakValeIntroScript@NScript@@QAEXXZ`; CQ_NewOakValeIntroScript::DoMission
// (0x00DBE003) spawns it as a CSpawnedFunc0 thread named "WatchForGotGold" (0x012D9CE8). The catalog label
// "CQ_FireHeartScript::CheckGameEnding" is propagated (the real one is the landed 0x00D360E0).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940; CGameScriptInterface slots 7 NewScriptFrame (+0x1C),
// 127 GetHeroGold (+0x1FC), 296 SetQuestCardObjective(const CCharString& x4) (+0x4A0), 655 GetActiveQuestName
// (+0xA3C, CCharString by value); CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0.
// Strings: "TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_03" @ 0x012D9D2C, "" @ 0x0122D70E.
// Layout: retail quest base 0x48 bytes (GSI +0x40).
// Shape: the gold wait is the rotated script idiom; the objective call is one nested expression: the three literal
// arguments are implicit conversion temporaries built right to left, the outer call's vtable is fetched before the
// inner GetActiveQuestName call (its result in a frame temporary), and the four strings die after the call.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);
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

class CQ_NewOakValeIntroScript : public CScriptBase { public:
    CGameScriptInterface* GSI;                          // +0x40
    void*                 PMasterData;                  // +0x44
    void WatchForGotGold();
};

void CQ_NewOakValeIntroScript::WatchForGotGold()
{
    while (GSI->GetHeroGold() <= 2) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    GSI->SetQuestCardObjective(GSI->GetActiveQuestName(), "TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_03", "", "");
}
}