// Layout: retail STLport 4.6 container sizes put the PDB member on these bytes; renamed `f` -> `StillUnderground`.
// Leaf: ?IsToEvadeAllHits@CCreatureAction_UndeadRiseUp@@UBE_NXZ
// Header route (NF319, 2026-11-23): defined on the real class of engine/CCreatureAction_UndeadRiseUp.h (was ?IsToEvadeAllHits@CCreatureAction_UndeadRiseUp_Methods@@QAEEXZ); the retail RTTI vftable
//   placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address; declared by the add-only opt-in
//   FABLE_RETAIL_CCREATUREACTION_UNDEADRISEUP_RTTI_DECLS. Bytes unchanged.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCOMBATACTIONBASE_MELEE
#define FABLE_RETAIL_CCOMBATACTIONBASE_INITIALISE
#define FABLE_RETAIL_CCREATUREACTION_UNDEADRISEUP_CLASS
#define FABLE_RETAIL_CCREATUREACTION_UNDEADRISEUP_RTTI_DECLS
#include "engine/CThingCreatureBase.h"
#include "engine/CDefIndex.h"
#include "engine/CCreatureAction_RiseUp.h"
#include "engine/CCreatureAction_UndeadRiseUp.h"  // retyped onto the PDB layout; byte parity re-verified
bool CCreatureAction_UndeadRiseUp::IsToEvadeAllHits() const { return this->StillUnderground; }
