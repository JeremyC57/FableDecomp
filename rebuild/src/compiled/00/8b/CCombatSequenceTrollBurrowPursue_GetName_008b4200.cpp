// Leaf: ?GetName@CCombatSequence_TrollBurrowPursue@@UBE?AVCCharString@@XZ
// Literal (NF418, 2026-11-25): was a file-static array `kName` (table (none: object-local static) -> 0x01278A9C); now spelled at the use site:
//   "COMBAT_SEQUENCE_TROLL_BURROW_PURSUE", retail's pooled literal at 0x01278A9C (/GF-merged ??_C@, NF418 CLASSIFICATION.tsv).
// Class-model move (NF325, 2026-11-24): defined on the real class of engine/CCombatSequence_TrollBurrowPursue.h under FABLE_RETAIL_CCOMBATSEQUENCE_TROLLBURROWPURSUE_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CCombatSequence_TrollBurrowPursue_Methods::GetName); the retail RTTI vftable placement names this PDB member here.
#define FABLE_RETAIL_CCOMBATSEQUENCE_TROLLBURROWPURSUE_VIRTUAL_MODEL
#define FABLE_RETAIL_CCOMBATSEQUENCEBASE
#include "engine/CCombatSequence_TrollBurrowPursue.h"

class CCharString { public: char* m_buf; CCharString(const char*, long); };
CCharString CCombatSequence_TrollBurrowPursue::GetName() const { return CCharString("COMBAT_SEQUENCE_TROLL_BURROW_PURSUE", -1); }
