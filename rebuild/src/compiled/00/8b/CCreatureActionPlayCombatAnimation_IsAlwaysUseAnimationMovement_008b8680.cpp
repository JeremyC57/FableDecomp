// Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `f_3` -> `AlwaysUseMovementInAnim`.
// Leaf: ?IsAlwaysUseAnimationMovement@CCreatureAction_PlayCombatAnimation@@UBE_NXZ
// Header route (NF319, 2026-11-23): defined on the real class of engine/CCreatureAction_PlayCombatAnimation.h (was ?IsAlwaysUseAnimationMovement@CCreatureAction_PlayCombatAnimation_Methods@@QAEEXZ); the retail RTTI vftable
//   placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address;
//   declared there under FABLE_RETAIL_CCREATUREACTION_PLAYCOMBATANIMATION + FABLE_RETAIL_JACK_STAGE1_UPDATE (switches landed rows define). Bytes unchanged.
#define FABLE_RETAIL_JACK_STAGE1_UPDATE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_PLAYCOMBATANIMATION
#include "engine/CThingAICreature.h"
#include "engine/CSoundPair.h"
class CAnimationEntry;
#include "engine/CCreatureAction_PlayCombatAnimation.h"  // retyped onto the PDB layout; byte parity re-verified
bool CCreatureAction_PlayCombatAnimation::IsAlwaysUseAnimationMovement() const { return this->AlwaysUseMovementInAnim; }
