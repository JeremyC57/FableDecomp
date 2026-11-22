// CTCNymph::ClearReadyToAttackFlags() -- clear CanAttack / ReadyToAttack, then drop the nymph's target (PTarget = 0).
// Leaf: ?ClearReadyToAttackFlags@CTCNymph@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0091C080, length 74 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, SetPItem @ 0x00A01B90, GetPItem
// @ 0x00A01B50, ~CBaseIntelligentPointer @ 0x00A01C10 -- all on the 8-byte frame temporary (vtable 0x0123E7D8).
// Shape: the two byte flags at +0x58 / +0x59 are cleared first (scheduled between the temporary's address formation and its constructor
//   call), then `PTarget = 0` goes through a converting CIntelligentPointer<CThingCreatureBase> temporary (base constructor, vtable store,
//   SetPItem(0)), the inline assignment (GetPItem on the temporary, SetPItem) and the temporary's destructor.
// Names (agentNF250, 2026-11-22, CODEXP27 L86): was the descriptive stand-in ?Cleanup@CAIStateGroupWithBuilding_0091c080@@QAEXXZ over a
//   row-local owner and CIntelligentPointer model (the manifest labels "CAIStateGroup_MinionPutOutFires::Cleanup" / "CTCNymph::ClearTarget"
//   were propagated). FableWin.pdb S_PUB32 ?ClearReadyToAttackFlags@CTCNymph@@QAEXXZ; the Ego_r.pdb CTCNymph field list (engine/CTCNymph.h:
//   CanAttack +0x58, ReadyToAttack +0x59, PTarget +0x68 in the retail layout, sizeof 0x70) is the body this function writes; the landed
//   caller 0x0091C6A0 (CAIStateGroup_NymphAttack::Update, `pTCNymph->ClearReadyToAttackFlags()`) already binds it. Now on the shared
//   engine/CTCNymph.h and engine/CIntelligentPointer.h (the caller's prelude + FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN).
#define FABLE_RETAIL_NYMPH_ATTACK_PASS27
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_FORCEINLINE_CTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_COMBATPERFORMACTION
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CCHARSTRING
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroup_NymphAttack.h"
#include "engine/CAIBrain.h"
#include "engine/CTCNymph.h"
#include "engine/CMainGameComponent.h"
#include "engine/CWorld.h"
#include "engine/ThingAlive.h"
#include "engine/EAIAction.h"
#include "engine/CCreatureAction_NymphSummonInto.h"
#include "engine/CCreatureAction_NymphSummonOutOf.h"
#include "engine/CCreatureAction_SummonCreatures.h"
#include "engine/CCreatureAction_NymphIdleAfterShot.h"
#include "engine/CCreatureAction_NymphShot.h"
#include "engine/CTCNymph.h"

void CTCNymph::ClearReadyToAttackFlags()
{
    CanAttack = false;
    ReadyToAttack = false;
    PTarget = 0;
}
