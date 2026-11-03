// CGameScriptInterface::SetQuestCardGoldReward -- script call: forward to the quest manager singleton's SetQuestCardGoldReward with the same arguments
// Leaf: ?SetQuestCardGoldReward@CGameScriptInterface@@UBEXABVCCharString@@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008914F0, 11 bytes: `mov ecx,[0x013B89FC] ; jmp 0x004B0E50`.
// Identity: retail CGameScriptInterface vtable 0x01260F0C slot 297 (engine/CGameScriptInterface.h; Ego_r.pdb / FableWin.pdb
//   ?SetQuestCardGoldReward@CGameScriptInterface@@UBEXABVCCharString@@J@Z); the callee is FableWin.pdb ?SetQuestCardGoldReward@CQuestManager@@QAEXABVCCharString@@J@Z at 0x004B0E50.
// Body: CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get() (inline, reads the private static PClass 0x013B89FC,
//   ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@0V?$CCountedPointer@VCQuestManager@@@@A), then a tail jump that reuses
//   the caller's stack arguments unchanged.
// Layout: engine/CQuestManager.h (retail STLport offsets) included under a layout name; the class below adds only the
//   PDB method this body calls.
class CCharString;
#include "engine/CCountedPointer.h"
#include "engine/CTBaseSingletonNoDefaultAlloc.h"
#define CQuestManager CQuestManagerDonor
#include "engine/CQuestManager.h"
#undef CQuestManager
class CQuestManager : public CQuestManagerDonor {
public:
    void SetQuestCardGoldReward(const CCharString& questName, long gold);                                     // 0x004B0E50
};
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::SetQuestCardGoldReward(const CCharString& questName, long gold) const
{
    CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().SetQuestCardGoldReward(questName, gold);
}
