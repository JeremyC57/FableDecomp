// Leaf: ?GetName@CCombatSequence_FodderSecondaryAttack@@UBE?AVCCharString@@XZ
// Class-model move (NF325, 2026-11-24): defined on the real class of engine/CCombatSequence_FodderSecondaryAttack.h under FABLE_RETAIL_CCOMBATSEQUENCE_FODDERSECONDARYATTACK_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CCombatSequence_FodderSecondaryAttack_Methods::GetName); the retail RTTI vftable placement names this PDB member here.
#define FABLE_RETAIL_CCOMBATSEQUENCE_FODDERSECONDARYATTACK_VIRTUAL_MODEL
#define FABLE_RETAIL_CCOMBATSEQUENCEBASE
#include "engine/CCombatSequence_FodderSecondaryAttack.h"

class CCharString { public: char* m_buf; CCharString(const char*, long); };
static const char kName[] = "COMBAT_SEQUENCE_FODDER_SECONDARY_ATTACK";
CCharString CCombatSequence_FodderSecondaryAttack::GetName() const { return CCharString(kName, -1); }
