// NScript::CHeroBoastsScript::BoastUseNoHealthPotions(CCharString questName) -- boast thread "use no health potions": track the hero's health each frame; a rise (a potion) shows the failure game-info and fails boast 8 once it is clicked past; otherwise the quest's failure fails the boast and its completion completes it
// Leaf: BoastUseNoHealthPotions
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CE4600, length 353 bytes, `ret 4` (the CCharString is passed by value and
//   destroyed here).
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940, CCharString(const char*, long) @ 0x0099EBF0 /
//   ~CCharString @ 0x0099EAE0 ("TEXT_QST_BOAST_STATE_FAILED_USENOHEALTHPOTIONS" @ 0x012C3CC0); CGameScriptInterface
//   (vtable 0x01260F0C) slots 7 NewScriptFrame, 28 MsgOnQuestCompletedBeforeScreenShown, 29
//   MsgOnQuestFailedBeforeScreenShown, 40 MsgIsGameInfoClickedPast, 115 DisplayGameInfo, 257 GetHeroHealth, 305
//   SetBoastAsFailed, 306 SetBoastAsCompleted.
// Layout: as the landed sibling BoastArenaShowNoMercy 0x00CE5EF0 (CScriptBase retail 0x40 bytes, GSI +0x40).
// Shape: the watch is a rotated `while (!IsActiveThreadTerminating())` whose health compare is `health > lastHealth`
//   (`test ah,41h ; je`); the potion path and the quest-failed path share the SetBoastAsFailed tail.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

namespace NScript {
class CQ_SunnyvaleMasterData;

}   // namespace NScript (CScriptBase is global in retail)
class CScriptBase { public:                               // retail 0x40 bytes
    virtual void v00();
    unsigned char _body[0x3c];                     // +0x04 .. +0x40
public:
    bool IsActiveThreadTerminating();   // 0x00CB7940
};
namespace NScript {

enum { BOAST_USE_NO_HEALTH_POTIONS = 8 };

class CHeroBoastsScript : public CScriptBase { public:
    CGameScriptInterface*   GSI;                   // +0x40
    CQ_SunnyvaleMasterData* PMasterData;           // +0x44
    void BoastUseNoHealthPotions(CCharString questName);
};

void CHeroBoastsScript::BoastUseNoHealthPotions(CCharString questName)
{
    float lastHealth = GSI->GetHeroHealth();
    while (!IsActiveThreadTerminating()) {
        float health = GSI->GetHeroHealth();
        if (health > lastHealth) {
            if (IsActiveThreadTerminating())
                return;
            GSI->DisplayGameInfo("TEXT_QST_BOAST_STATE_FAILED_USENOHEALTHPOTIONS");
            while (!GSI->MsgIsGameInfoClickedPast()) {
                GSI->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            GSI->SetBoastAsFailed(BOAST_USE_NO_HEALTH_POTIONS, questName);
            return;
        }
        if (IsActiveThreadTerminating())
            return;
        lastHealth = health;
        if (GSI->MsgOnQuestFailedBeforeScreenShown(questName)) {
            if (IsActiveThreadTerminating())
                return;
            GSI->SetBoastAsFailed(BOAST_USE_NO_HEALTH_POTIONS, questName);
            return;
        }
        if (GSI->MsgOnQuestCompletedBeforeScreenShown(questName)) {
            if (IsActiveThreadTerminating())
                return;
            GSI->SetBoastAsCompleted(BOAST_USE_NO_HEALTH_POTIONS, questName);
            return;
        }
        GSI->NewScriptFrame();
    }
}
}