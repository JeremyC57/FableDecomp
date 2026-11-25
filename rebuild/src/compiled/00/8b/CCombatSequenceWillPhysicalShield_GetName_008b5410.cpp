// Leaf: ?GetName@CCombatSequence_WillPhysicalShield@@UBE?AVCCharString@@XZ
// Class-model move (NF325, 2026-11-24): defined on the real class of engine/CCombatSequence_WillPhysicalShield.h under FABLE_RETAIL_CCOMBATSEQUENCE_WILLPHYSICALSHIELD_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CCombatSequence_WillPhysicalShield_Methods::GetName); the retail RTTI vftable placement names this PDB member here.
#define FABLE_RETAIL_CCOMBATSEQUENCE_WILLPHYSICALSHIELD_VIRTUAL_MODEL
#define FABLE_RETAIL_CCOMBATSEQUENCEBASE
#include "engine/CCombatSequence_WillPhysicalShield.h"

class CCharString { public: char* m_data; CCharString(); CCharString(const char*, long); CCharString(const CCharString&); ~CCharString(); };
CCharString CCombatSequence_WillPhysicalShield::GetName() const { return CCharString("COMBAT_SEQUENCE_WILL_PHYSICAL_SHIELD", -1); }
