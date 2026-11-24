// Leaf: ?OnReplace@CAIStateGroup_MinionReactToNoise@@UAEXXZ
#define FABLE_RETAIL_CAISTATEGROUP_MINIONREACTTONOISE_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "engine/CAIStateGroup_MinionReactToNoise.h"  // retyped onto the PDB layout; byte parity re-verified
class CWorld { public: static long __fastcall GetFrame(); };   // 0x0049D870 ?GetFrame@CWorld@@SIJXZ
void CAIStateGroup_MinionReactToNoise::OnReplace()
{
    CAIStateGroup_MinionReactToNoise* stateGroup = this;
    stateGroup->FrameOfLastInteraction = CWorld::GetFrame();
}
// Flatten route (flatten_pilot v5, NF330): CAIStateGroup_MinionReactToNoise_OnReplace -> CAIStateGroup_MinionReactToNoise::OnReplace, the RTTI-placed PDB member; class model: engine/CAIStateGroup_MinionReactToNoise.h opt-in FABLE_RETAIL_CAISTATEGROUP_MINIONREACTTONOISE_VIRTUAL_MODEL, FABLE_RETAIL_CAISTATEGROUPBASE, FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS.
