// Leaf: ?GetName@CCombatSequence_GenericRangedAttack@@UBE?AVCCharString@@XZ
// Literal (NF418, 2026-11-25): was a file-static array `kName` (table _kName -> 0x012786AC); now spelled at the use site:
//   "COMBAT_SEQUENCE_GENERIC_RANGED_ATTACK", retail's pooled literal at 0x012786AC (/GF-merged ??_C@, NF418 CLASSIFICATION.tsv).
// Class-model move (NF325, 2026-11-24): defined on the real class of engine/CCombatSequence_GenericRangedAttack.h under FABLE_RETAIL_CCOMBATSEQUENCE_GENERICRANGEDATTACK_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CCombatSequence_GenericRangedAttack_Methods::GetName); the retail RTTI vftable placement names this PDB member here.
#define FABLE_RETAIL_CCOMBATSEQUENCE_GENERICRANGEDATTACK_VIRTUAL_MODEL
#define FABLE_RETAIL_CCOMBATSEQUENCEBASE
#include "engine/CCombatSequence_GenericRangedAttack.h"

class CCharString { public: char* m_buf; CCharString(const char*, long); };
CCharString CCombatSequence_GenericRangedAttack::GetName() const { return CCharString("COMBAT_SEQUENCE_GENERIC_RANGED_ATTACK", -1); }
