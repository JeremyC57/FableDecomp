// NCreatureMode::CModeExplosiveForcePushed::OnActivate() -- nothing of its own: the base activation (CModeBase::OnActivate 0x0073AAC0),
//   which VC7.1 emits as a 5-byte tail jump
// Leaf: ?OnActivate@CModeExplosiveForcePushed@NCreatureMode@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D6760, 5 bytes (`jmp 0x0073AAC0`).
// Name: Ego_r.pdb 0x00817AC0 + FableWin.pdb 0x028458D0 ?OnActivate@CModeExplosiveForcePushed@NCreatureMode@@UAEXXZ (public virtual, slot 5);
//   RTTI .?AVCModeExplosiveForcePushed@NCreatureMode@@ : CTCCreatureModeManager::CModeBase (ghidra_out/rtti_bases.tsv). The class model is
//   the one the landed siblings OnDeactivate 0x008D6820 / AnimUpdate 0x008D6780 use.
// Naming (NF126, 2026-11-20): was the extern "C" forwarder CModeExplosiveForcePushed_OnActivate -> ForwardTarget_0073aac0 (a placeholder
//   callee name); now the member on the shared nested CTCCreatureModeManager::CModeBase, binding ?OnActivate@CModeBase@CTCCreatureModeManager@@UAEXXZ.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCCreatureModeManager.h"
namespace NCreatureMode {
class CModeExplosiveForcePushed : public CTCCreatureModeManager::CModeBase {
public:
    CModeExplosiveForcePushed(CThing& thing);
    virtual void OnActivate();
};
void CModeExplosiveForcePushed::OnActivate()
{
    CModeBase::OnActivate();
}
}
