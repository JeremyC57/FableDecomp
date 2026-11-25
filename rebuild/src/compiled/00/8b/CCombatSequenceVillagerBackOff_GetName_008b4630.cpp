// Leaf: ?GetName@CCombatSequence_VillagerBackOff@@UBE?AVCCharString@@XZ
// Class-model move (NF325, 2026-11-24): defined on the real class of engine/CCombatSequence_VillagerBackOff.h under FABLE_RETAIL_CCOMBATSEQUENCE_VILLAGERBACKOFF_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CCombatSequence_VillagerBackOff_Methods::GetName); the retail RTTI vftable placement names this PDB member here.
#define FABLE_RETAIL_CCOMBATSEQUENCE_VILLAGERBACKOFF_VIRTUAL_MODEL
#define FABLE_RETAIL_CCOMBATSEQUENCEBASE
#include "engine/CCombatSequence_VillagerBackOff.h"

class CCharString { public: char* m_pData; CCharString(const char*, long); };
CCharString CCombatSequence_VillagerBackOff::GetName() const { return CCharString("COMBAT_SEQUENCE_VILLAGER_BACK_OFF", -1); }
