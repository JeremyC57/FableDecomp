// CGameScriptInterface::SetQuestCardRenownReward -- script call: forward to the quest manager singleton's SetQuestCardRenownReward with the same arguments
// Leaf: ?SetQuestCardRenownReward@CGameScriptInterface@@UBEXABVCCharString@@J@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00891500, 11 bytes: `mov ecx,[0x013B89FC] ; jmp 0x004B0EB0`.
// Identity: retail CGameScriptInterface vtable 0x01260F0C slot 298 (engine/CGameScriptInterface.h; Ego_r.pdb / FableWin.pdb
//   ?SetQuestCardRenownReward@CGameScriptInterface@@UBEXABVCCharString@@J@Z); the callee is FableWin.pdb ?SetQuestCardRenownReward@CQuestManager@@QAEXABVCCharString@@J@Z at 0x004B0EB0.
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
    void SetQuestCardRenownReward(const CCharString& questName, long renown);                                     // 0x004B0EB0
};
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::SetQuestCardRenownReward(const CCharString& questName, long renown) const
{
    CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().SetQuestCardRenownReward(questName, renown);
}