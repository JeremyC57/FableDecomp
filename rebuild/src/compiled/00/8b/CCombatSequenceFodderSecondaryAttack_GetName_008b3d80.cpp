// Leaf: ?GetName@CCombatSequence_FodderSecondaryAttack@@UBE?AVCCharString@@XZ
// Literal (NF418, 2026-11-25): was a file-static array `kName` (table (none: object-local static) -> 0x01278954); now spelled at the use site:
//   "COMBAT_SEQUENCE_FODDER_SECONDARY_ATTACK", retail's pooled literal at 0x01278954 (/GF-merged ??_C@, NF418 CLASSIFICATION.tsv).
// Class-model move (NF325, 2026-11-24): defined on the real class of engine/CCombatSequence_FodderSecondaryAttack.h under FABLE_RETAIL_CCOMBATSEQUENCE_FODDERSECONDARYATTACK_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CCombatSequence_FodderSecondaryAttack_Methods::GetName); the retail RTTI vftable placement names this PDB member here.
#define FABLE_RETAIL_CCOMBATSEQUENCE_FODDERSECONDARYATTACK_VIRTUAL_MODEL
#define FABLE_RETAIL_CCOMBATSEQUENCEBASE
#include "engine/CCombatSequence_FodderSecondaryAttack.h"

class CCharString { public: char* m_buf; CCharString(const char*, long); };
CCharString CCombatSequence_FodderSecondaryAttack::GetName() const { return CCharString("COMBAT_SEQUENCE_FODDER_SECONDARY_ATTACK", -1); }
