// CGameScriptInterface::PrepareQuestsWhenFinalQuestIsCompleted -- script call: forward to the quest manager singleton's PrepareQuestsWhenFinalQuestIsCompleted (no arguments)
// Leaf: ?PrepareQuestsWhenFinalQuestIsCompleted@CGameScriptInterface@@UBEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00892F30, 11 bytes: `mov ecx,[0x013B89FC] ; jmp 0x004AF270`.
// Identity: retail CGameScriptInterface vtable 0x01260F0C slot 283 (engine/CGameScriptInterface.h; Ego_r.pdb / FableWin.pdb
//   ?PrepareQuestsWhenFinalQuestIsCompleted@CGameScriptInterface@@UBEXXZ); the callee is FableWin.pdb ?PrepareQuestsWhenFinalQuestIsCompleted@CQuestManager@@QAEXXZ at 0x004AF270.
//   0x004AF270 sets World->MapTableShowQuestCardsOnUsed (+0x7c World, CWorld +0xda) = true; its catalog label
//   "CGameScriptInterface::DontPopulateNextLoadedRegion" is propagated (that slot-59 body is 0x0088E380) -- NAME_FIXES.tsv.
// Body: CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get() (inline, reads the private static PClass 0x013B89FC,
//   ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@0V?$CCountedPointer@VCQuestManager@@@@A), then a tail jump.
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
    void PrepareQuestsWhenFinalQuestIsCompleted();                                     // 0x004AF270
};
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::PrepareQuestsWhenFinalQuestIsCompleted() const
{
    CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().PrepareQuestsWhenFinalQuestIsCompleted();
}
