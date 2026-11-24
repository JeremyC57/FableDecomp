// Leaf: ?GetName@CCombatSequence_ScorpionLungeAttack@@UBE?AVCCharString@@XZ
// Class-model move (NF325, 2026-11-24): defined on the real class of engine/CCombatSequence_ScorpionLungeAttack.h under FABLE_RETAIL_CCOMBATSEQUENCE_SCORPIONLUNGEATTACK_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CCombatSequence_ScorpionLungeAttack_Methods::GetName); the retail RTTI vftable placement names this PDB member here.
#define FABLE_RETAIL_CCOMBATSEQUENCE_SCORPIONLUNGEATTACK_VIRTUAL_MODEL
#define FABLE_RETAIL_CCOMBATSEQUENCEBASE
#include "engine/CCombatSequence_ScorpionLungeAttack.h"

class CCharString { public: char* m_data; CCharString(const char*, long); };
extern const char kScorpionLungeName[];
CCharString CCombatSequence_ScorpionLungeAttack::GetName() const { return CCharString(kScorpionLungeName, -1); }
