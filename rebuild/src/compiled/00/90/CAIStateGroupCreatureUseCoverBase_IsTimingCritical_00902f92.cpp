// Leaf: ?IsTimingCritical@CAIStateGroup_CreatureUseCoverBase@@UBE_NXZ
// Header route (NF319, 2026-11-23): defined on the real class of engine/CAIStateGroup_CreatureUseCoverBase.h (was ?IsTimingCritical@CAIStateGroup_CreatureUseCoverBase_Methods@@QAEEXZ); the retail RTTI vftable
//   placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address; declared by the add-only opt-in
//   FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE_RTTI_DECLS. Bytes unchanged.
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CAISTATEGROUP_ENEMYCREATUREBASE
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE
#define FABLE_RETAIL_CAISTATEGROUP_CREATUREUSECOVERBASE_RTTI_DECLS
class CThing;
class CThingCreatureBase;
#include "engine/CAIStateGroup_CreatureUseCoverBase.h"  // retyped onto the PDB layout; byte parity re-verified
bool CAIStateGroup_CreatureUseCoverBase::IsTimingCritical() const { return this->TimingCritical; }
