// CGameScriptInterface::PrepareQuestsWhenFinalQuestIsActivated -- script call: forward to the quest manager singleton's PrepareQuestsWhenFinalQuestIsActivated with the same arguments
// Leaf: ?PrepareQuestsWhenFinalQuestIsActivated@CGameScriptInterface@@UBEXABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00892F20, 11 bytes: `mov ecx,[0x013B89FC] ; jmp 0x004B0F10`.
// Identity: retail CGameScriptInterface vtable 0x01260F0C slot 282 (engine/CGameScriptInterface.h; Ego_r.pdb / FableWin.pdb
//   ?PrepareQuestsWhenFinalQuestIsActivated@CGameScriptInterface@@UBEXABVCCharString@@@Z); the callee is FableWin.pdb ?PrepareQuestsWhenFinalQuestIsActivated@CQuestManager@@QAEXABVCCharString@@@Z at 0x004B0F10.
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
    void PrepareQuestsWhenFinalQuestIsActivated(const CCharString& finalQuestName);                                     // 0x004B0F10
};
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::PrepareQuestsWhenFinalQuestIsActivated(const CCharString& finalQuestName) const
{
    CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().PrepareQuestsWhenFinalQuestIsActivated(finalQuestName);
}