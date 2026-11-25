// Leaf: ?GetName@CCombatSequence_BanditAdvanceFar@@UBE?AVCCharString@@XZ
// Class-model move (NF325, 2026-11-24): defined on the real class of engine/CCombatSequence_BanditAdvanceFar.h under FABLE_RETAIL_CCOMBATSEQUENCE_BANDITADVANCEFAR_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CCombatSequence_BanditAdvanceFar_Methods::GetName); the retail RTTI vftable placement names this PDB member here.
#define FABLE_RETAIL_CCOMBATSEQUENCE_BANDITADVANCEFAR_VIRTUAL_MODEL
#define FABLE_RETAIL_CCOMBATSEQUENCEBASE
#include "engine/CCombatSequence_BanditAdvanceFar.h"

class CCharString { public: char* p; CCharString(const char*, long); };
CCharString CCombatSequence_BanditAdvanceFar::GetName() const { return CCharString("COMBAT_SEQUENCE_BANDIT_ADVANCE_LONG", -1); }
