// Leaf: ?Update@CAIStateGroup_ChickenBeg@@UAEXXZ
#define FABLE_RETAIL_CAISTATEGROUP_CHICKENBEG_VIRTUAL_MODEL
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#include "engine/CAIStateGroup_ChickenBeg.h"

struct ChickenBegStageA; class CThingPlayerCreature;
class CPlayer { public: const CThingPlayerCreature* PeekPControlledCreature() const; };   // 0x00487DD0
ChickenBegStageA* __fastcall f1(void* stateGroup);
// CPlayerManager: the shared model engine/CPlayerManager.h (Ego_r.pdb CPlayerManager; the five accessors declared
//   there with their retail addresses: GetPlayer 0x004498C0 / 0x00449910, GetMainPlayer 0x00449970 / 0x00449960,
//   GetMainPlayerNumber 0x00449700).
#include "engine/CPlayerManager.h"
 /* 0x00449970 CPlayerManager::GetMainPlayer (Ego_r 0x004450F0) */
void __fastcall f4(void* stateGroup);

void CAIStateGroup_ChickenBeg::Update()
{
    CAIStateGroup_ChickenBeg* stateGroup = this;
    (&((CPlayerManager*)(f1(stateGroup)))->GetMainPlayer())->PeekPControlledCreature();
    f4(stateGroup);
}
// Flatten route (flatten_pilot v5, NF330): CAIStateGroup_ChickenBeg_Update -> CAIStateGroup_ChickenBeg::Update, the RTTI-placed PDB member; class model: engine/CAIStateGroup_ChickenBeg.h opt-in FABLE_RETAIL_CAISTATEGROUP_CHICKENBEG_VIRTUAL_MODEL, FABLE_RETAIL_CAISTATEGROUPBASE, FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS.
