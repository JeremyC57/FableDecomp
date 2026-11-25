// Leaf: ?GetName@CCombatSequence_TrollBurrowPursue@@UBE?AVCCharString@@XZ
// Class-model move (NF325, 2026-11-24): defined on the real class of engine/CCombatSequence_TrollBurrowPursue.h under FABLE_RETAIL_CCOMBATSEQUENCE_TROLLBURROWPURSUE_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CCombatSequence_TrollBurrowPursue_Methods::GetName); the retail RTTI vftable placement names this PDB member here.
#define FABLE_RETAIL_CCOMBATSEQUENCE_TROLLBURROWPURSUE_VIRTUAL_MODEL
#define FABLE_RETAIL_CCOMBATSEQUENCEBASE
#include "engine/CCombatSequence_TrollBurrowPursue.h"

class CCharString { public: char* m_buf; CCharString(const char*, long); };
static const char kName[] = "COMBAT_SEQUENCE_TROLL_BURROW_PURSUE";
CCharString CCombatSequence_TrollBurrowPursue::GetName() const { return CCharString(kName, -1); }
