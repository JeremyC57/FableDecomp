// NScript::CQ_BountyHuntScript::RemoveBoastQuestInfo() -- take the boast timer off the quest-info panel: hide the quest
//   info, clear BoastQuestInfoDisplayed, then remove the timer element BoastTimerID (the inverse of AddBoastQuestInfo
//   0x00D17EA0)
// Leaf: ?RemoveBoastQuestInfo@CQ_BountyHuntScript@NScript@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D17E70, 40 bytes (+ int3 padding), plain `ret`.
// Identity: FableWin.pdb ?RemoveBoastQuestInfo@CQ_BountyHuntScript@NScript@@QAEXXZ (void, no arguments: `ret`); retail
//   order in the CQ_BountyHuntScript block is WatchForQuestInfoDisplayConditions 0x00D17DA0 -> this -> AddBoastQuestInfo
//   0x00D17EA0, and the only caller (0x00D17DF2) is WatchForQuestInfoDisplayConditions after it tests
//   BoastQuestInfoDisplayed (+0x84). The catalog name carried an `_00d17e70` suffix and the landed source called it
//   CBowerstoneIntroGuard::Init (a local model whose members were unnamed); the members are the Ego_r.pdb ones.
// Callees: through the quest's game script interface (+0x40): DisplayQuestInfo (slot 321, +0x504, 0x00891920) and
//   RemoveQuestInfoElement (slot 338, +0x548, 0x00891C10) -- slot names from ghidra_out/cgsi_slots_aligned.tsv.
// Layout: Ego_r.pdb CQ_BountyHuntScript - 0x14 (as AddBoastQuestInfo): BoastTimerID +0x7c (PDB 144), BoastTime +0x80,
//   BoastQuestInfoDisplayed +0x84 (PDB 152).
// Shape: the interface pointer is re-read for the second call; the flag store is scheduled between its loads.
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

class CScriptBase { public:                                        // retail 0x40 bytes (PDB 0x54: debug containers)
    unsigned char _body[0x40];
};

struct CQuestScriptBase : CScriptBase {                     // retail 0x48 bytes
    CGameScriptInterface* GSI;                              // +0x40
    void*                 Owner;                            // +0x44
};

namespace NScript {
class CQ_BountyHuntScript : public CQuestScriptBase {
public:
    unsigned char _pad48[0x7c - 0x48];                      // +0x48 quest flags, BanditGruntTextCounter, boast flags
    long          BoastTimerID;                             // +0x7c
    long          BoastTime;                                // +0x80 (CTimer: its Index)
    bool          BoastQuestInfoDisplayed;                  // +0x84
    void RemoveBoastQuestInfo();
};
} // namespace NScript

void NScript::CQ_BountyHuntScript::RemoveBoastQuestInfo()
{
    GSI->DisplayQuestInfo(false);
    BoastQuestInfoDisplayed = false;
    GSI->RemoveQuestInfoElement(BoastTimerID);
}
