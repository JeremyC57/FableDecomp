// Leaf: ?IsActionReadyForInterruption@CCreatureAction_ActivateQuest@@UBE_NXZ
// Switch route (NF318, 2026-11-23): defined on the real class of engine/CCreatureAction_ActivateQuest.h (class-model switch FABLE_RETAIL_CCREATUREACTION_ACTIVATEQUEST; was ?IsActionReadyForInterruption@CCreatureAction_ActivateQuest_Methods@@QAEEXZ);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address. Bytes unchanged.
// Preamble: the define / include block of the landed precedent CCreatureActionActivateQuest_scalardeletingdestructor_0084b500.cpp.
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTIONBASE_FULL_MEMBERS
#define FABLE_RETAIL_CCREATUREACTION_ACTIVATEQUEST
#include "fable_string.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CCreatureActionBase.h"
#include "engine/CCreatureAction_ActivateQuest.h"
bool CCreatureAction_ActivateQuest::IsActionReadyForInterruption() const{ return Initialised; }   // CCreatureActionBase::Initialised +0x63 (the flat model's `f`)
