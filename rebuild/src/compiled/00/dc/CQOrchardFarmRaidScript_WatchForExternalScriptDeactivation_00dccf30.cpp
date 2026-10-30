// CQ_OrchardFarmRaidScript::WatchForExternalScriptDeactivation() -- wait until neither the good nor the evil farm-raid quest is active, then re-add both quest cards and deactivate the parent quest
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DCCF30, length 407 bytes (padded to 416).
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940; CCharString(const char*, long) @ 0x0099EBF0,
//   ~CCharString @ 0x0099EAE0; through the script's interface (+0x40): NewScriptFrame (slot 7), DeactivateQuestLater
//   (slot 281, +0x464), IsQuestActive (slot 284, +0x470), AddQuestCard (slot 292, +0x490).
// Shape: the wait condition is a named `bool deactivated = !(A || B);` tested at the top of a `for (;;)` loop: VC keeps
//   the two conversion temporaries' construction flags in ebx and the bool in a frame byte (1 = both inactive); a
//   `while (!(...))` form inverts the stored byte; each string literal is passed straight to a `const CCharString&` parameter.
class CCharString { public:
    void* Data;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptBase { public:                                    // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                   // 0x00CB7940
};
struct CQ_OrchardFarmRaidScript : CScriptBase {
    CGameScriptInterface* Game;                         // +0x40
    void WatchForExternalScriptDeactivation();
};

void CQ_OrchardFarmRaidScript::WatchForExternalScriptDeactivation()
{
    if (IsActiveThreadTerminating())
        return;
    for (;;) {
        bool deactivated = !(Game->IsQuestActive("Q_OrchardFarmRaidGood") || Game->IsQuestActive("Q_OrchardFarmRaidEvil"));
        if (deactivated)
            break;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM", "Q_OrchardFarmRaidGood", false, false);
    Game->AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM_EVIL", "Q_OrchardFarmRaidEvil", false, false);
    Game->DeactivateQuestLater("Q_OrchardFarmRaid", 0);
}