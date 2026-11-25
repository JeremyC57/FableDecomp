// Leaf: ?GetName@CCombatSequence_GenericRangedAttack@@UBE?AVCCharString@@XZ
// Class-model move (NF325, 2026-11-24): defined on the real class of engine/CCombatSequence_GenericRangedAttack.h under FABLE_RETAIL_CCOMBATSEQUENCE_GENERICRANGEDATTACK_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CCombatSequence_GenericRangedAttack_Methods::GetName); the retail RTTI vftable placement names this PDB member here.
#define FABLE_RETAIL_CCOMBATSEQUENCE_GENERICRANGEDATTACK_VIRTUAL_MODEL
#define FABLE_RETAIL_CCOMBATSEQUENCEBASE
#include "engine/CCombatSequence_GenericRangedAttack.h"

class CCharString { public: char* m_buf; CCharString(const char*, long); };
static const char kName[] = "COMBAT_SEQUENCE_GENERIC_RANGED_ATTACK";
CCharString CCombatSequence_GenericRangedAttack::GetName() const { return CCharString(kName, -1); }
