// NScript::CHeroBoastsScript::BoastUseOnlyFryingPan(CCharString questName) -- boast thread "use only the frying pan": each frame, wielding anything but the frying pan (or failing the quest) fails boast 2 -- the weapon case first shows the failure game-info until it is clicked past -- and completing the quest completes it
// Leaf: BoastUseOnlyFryingPan
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CE40D0, length 438 bytes, `ret 4` (the CCharString is passed by value and
//   destroyed here).
// Callees: CScriptBase::IsActiveThreadTerminating @ 0x00CB7940, CCharString(const char*, long) @ 0x0099EBF0 /
//   ~CCharString @ 0x0099EAE0 ("OBJECT_FRYING_PAN" @ 0x012C3C2C, "TEXT_QST_BOAST_STATE_FAILED_FRYINGPAN" @ 0x012C3C04);
//   CGameScriptInterface slots 7 NewScriptFrame, 28 MsgOnQuestCompletedBeforeScreenShown, 29
//   MsgOnQuestFailedBeforeScreenShown, 40 MsgIsGameInfoClickedPast, 115 DisplayGameInfo, 194 IsPlayerCarryingItemOfType,
//   195 IsPlayerWieldingWeapon, 305 SetBoastAsFailed, 306 SetBoastAsCompleted (ghidra_out/cgsi_slots_aligned.tsv).
// Layout: as the landed sibling BoastUseNoHealthPotions 0x00CE4600 (CScriptBase retail 0x40 bytes, GSI +0x40).
// Shape: the failure test is one `||` expression whose middle operand holds the conditionally built "OBJECT_FRYING_PAN"
//   temporary (destruction flag in the first frame word, result in bl); a quest-failed failure skips the game-info.
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
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {

enum { BOAST_USE_ONLY_FRYING_PAN = 2 };

class CHeroBoastsScript : public CScriptBase { public:
    CGameScriptInterface*   GSI;                   // +0x40
    CQ_SunnyvaleMasterData* PMasterData;           // +0x44
    void BoastUseOnlyFryingPan(CCharString questName);
};

void CHeroBoastsScript::BoastUseOnlyFryingPan(CCharString questName)
{
    while (!IsActiveThreadTerminating()) {
        if ((GSI->IsPlayerWieldingWeapon() && !GSI->IsPlayerCarryingItemOfType("OBJECT_FRYING_PAN")) || GSI->MsgOnQuestFailedBeforeScreenShown(questName)) {
            if (IsActiveThreadTerminating())
                return;
            if (!GSI->MsgOnQuestFailedBeforeScreenShown(questName)) {
                if (IsActiveThreadTerminating())
                    return;
                GSI->DisplayGameInfo("TEXT_QST_BOAST_STATE_FAILED_FRYINGPAN");
                while (!GSI->MsgIsGameInfoClickedPast()) {
                    GSI->NewScriptFrame();
                    if (IsActiveThreadTerminating())
                        return;
                }
                if (IsActiveThreadTerminating())
                    return;
            }
            GSI->SetBoastAsFailed(BOAST_USE_ONLY_FRYING_PAN, questName);
            return;
        }
        if (GSI->MsgOnQuestCompletedBeforeScreenShown(questName)) {
            if (IsActiveThreadTerminating())
                return;
            GSI->SetBoastAsCompleted(BOAST_USE_ONLY_FRYING_PAN, questName);
            return;
        }
        GSI->NewScriptFrame();
    }
}
}