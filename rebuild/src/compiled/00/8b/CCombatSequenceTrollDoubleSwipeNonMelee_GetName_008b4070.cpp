// Leaf: ?GetName@CCombatSequence_TrollDoubleSwipeNonMelee@@UBE?AVCCharString@@XZ
// Class-model move (NF325, 2026-11-24): defined on the real class of engine/CCombatSequence_TrollDoubleSwipeNonMelee.h under FABLE_RETAIL_CCOMBATSEQUENCE_TROLLDOUBLESWIPENONMELEE_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CCombatSequence_TrollDoubleSwipeNonMelee_Methods::GetName); the retail RTTI vftable placement names this PDB member here.
#define FABLE_RETAIL_CCOMBATSEQUENCE_TROLLDOUBLESWIPENONMELEE_VIRTUAL_MODEL
#define FABLE_RETAIL_CCOMBATSEQUENCEBASE
#include "engine/CCombatSequence_TrollDoubleSwipeNonMelee.h"

class CCharString { public: char* p; CCharString(const char*, long); };
CCharString CCombatSequence_TrollDoubleSwipeNonMelee::GetName() const { return CCharString("COMBAT_SEQUENCE_TROLL_DOUBLE_SWIPE_NON_MELEE", -1); }
