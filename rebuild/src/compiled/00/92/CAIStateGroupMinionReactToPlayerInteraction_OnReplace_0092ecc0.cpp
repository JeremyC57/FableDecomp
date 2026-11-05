// Clear the two byte flags at this+0x2c / this+0x2d. __fastcall this=ecx.
// Leaf: ?OnReplace@CAIStateGroup_MinionReactToPlayerInteraction_Methods@@QAEXXZ
#include "engine/CAIStateGroup_MinionReactToPlayerInteraction.h"  // retyped onto the PDB layout; byte parity re-verified
#pragma pack(push,1)
struct CAIStateGroup_MinionReactToPlayerInteraction_Methods : CAIStateGroup_MinionReactToPlayerInteraction {
    void OnReplace();
};
#pragma pack(pop)
void CAIStateGroup_MinionReactToPlayerInteraction_Methods::OnReplace() { this->WaitingForConfirmation = false; this->DidTraderInteraction = false; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?OnReplace@CAIStateGroup_MinionReactToPlayerInteraction@@UAEXXZ); evidence: RTTI vtable slot + PDB name set.
