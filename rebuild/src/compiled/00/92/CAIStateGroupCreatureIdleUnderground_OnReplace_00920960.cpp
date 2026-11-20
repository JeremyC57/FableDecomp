// CAIStateGroup_CreatureIdleUnderground::OnReplace() -- make the creature rise up when the group is replaced
// Leaf: ?OnReplace@CAIStateGroup_CreatureIdleUnderground@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00920960, length 48 bytes.
// Callees: CCreatureAction_RiseUp::CCreatureAction_RiseUp(CThingCreatureBase&) @ 0x008BD880,
// CThingCreatureBase::SetCurrentAction(const CCreatureActionBase&) @ 0x006644F0,
// CCombatActionBase::~CCombatActionBase @ 0x0062E900 (the temporary's compiler-generated destructor).
// The creature (PBrain +0x4 -> Creature +0x18) is read once into esi: it is both the
// constructor argument and the object of SetCurrentAction. The action is a temporary in the
// call expression, destroyed after the call; it is the only frame slot (0x118 bytes).
// Naming (NF191, 2026-11-20, q4411 / q6405): ported off the row-local stand-ins (N_0062e900::CCreatureActionBase,
//   CThingCreatureBase, CCreatureAction_RiseUp, CAIBrain, CAIStateGroupBase) onto the shared headers, in the shape of the landed
//   same-TU sibling 0x00920DC0 (which expands this body). The relocations now bind the PDB spellings: Ego_r.pdb + FableWin.pdb
//   ??1CCombatActionBase@@UAE@XZ (0x0062E900; was ??1CCreatureActionBase@N_0062e900@@QAE@XZ),
//   ??0CCreatureAction_RiseUp@@QAE@AAVCThingCreatureBase@@@Z (0x008BD880) and
//   ?SetCurrentAction@CThingCreatureBase@@QAE_NABVCCreatureActionBase@@@Z (0x006644F0). CAIStateGroup_CreatureIdleUnderground has no
//   header under rebuild/include: the row keeps its local class token on the shared base CAIStateGroupBase, as 0x00920DC0 does.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCombatActionBase.h"
#include "engine/CCreatureAction_RiseUp.h"

class CAIStateGroup_CreatureIdleUnderground : public CAIStateGroupBase {
public:
    virtual void OnReplace();
};

void CAIStateGroup_CreatureIdleUnderground::OnReplace()
{
    GetCreature().SetCurrentAction(CCreatureAction_RiseUp(GetCreature()));
}
