// Leaf: ?GetName@CCombatSequence_WillPhysicalShieldNonMelee@@UBE?AVCCharString@@XZ
// Class-model move (NF325, 2026-11-24): defined on the real class of engine/CCombatSequence_WillPhysicalShieldNonMelee.h under FABLE_RETAIL_CCOMBATSEQUENCE_WILLPHYSICALSHIELDNONMELEE_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CCombatSequence_WillPhysicalShieldNonMelee_Methods::GetName); the retail RTTI vftable placement names this PDB member here.
#define FABLE_RETAIL_CCOMBATSEQUENCE_WILLPHYSICALSHIELDNONMELEE_VIRTUAL_MODEL
#define FABLE_RETAIL_CCOMBATSEQUENCEBASE
#include "engine/CCombatSequence_WillPhysicalShieldNonMelee.h"

class CCharString { public: char* m_data; CCharString(); CCharString(const char*, long); CCharString(const CCharString&); ~CCharString(); };
CCharString CCombatSequence_WillPhysicalShieldNonMelee::GetName() const { return CCharString("COMBAT_SEQUENCE_WILL_PHYSICAL_SHIELD_NON_MELEE", -1); }
