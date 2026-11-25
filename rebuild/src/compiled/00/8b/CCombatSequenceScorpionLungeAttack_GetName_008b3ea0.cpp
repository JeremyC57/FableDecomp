// Leaf: ?GetName@CCombatSequence_ScorpionLungeAttack@@UBE?AVCCharString@@XZ
// Literal (NF418, 2026-11-25): was extern data `kScorpionLungeName` (table ?kScorpionLungeName@@3QBDB -> 0x012789A0); now spelled at the use site:
//   "COMBAT_SEQUENCE_SCORPION_LUNGE_ATTACK", retail's pooled literal at 0x012789A0 (/GF-merged ??_C@, NF418 CLASSIFICATION.tsv).
// Class-model move (NF325, 2026-11-24): defined on the real class of engine/CCombatSequence_ScorpionLungeAttack.h under FABLE_RETAIL_CCOMBATSEQUENCE_SCORPIONLUNGEATTACK_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CCombatSequence_ScorpionLungeAttack_Methods::GetName); the retail RTTI vftable placement names this PDB member here.
#define FABLE_RETAIL_CCOMBATSEQUENCE_SCORPIONLUNGEATTACK_VIRTUAL_MODEL
#define FABLE_RETAIL_CCOMBATSEQUENCEBASE
#include "engine/CCombatSequence_ScorpionLungeAttack.h"

class CCharString { public: char* m_data; CCharString(const char*, long); };
CCharString CCombatSequence_ScorpionLungeAttack::GetName() const { return CCharString("COMBAT_SEQUENCE_SCORPION_LUNGE_ATTACK", -1); }
