// Leaf: ?GetName@CCombatSequence_BanditAttackSide@@UBE?AVCCharString@@XZ
// Class-model move (NF325, 2026-11-24): defined on the real class of engine/CCombatSequence_BanditAttackSide.h under FABLE_RETAIL_CCOMBATSEQUENCE_BANDITATTACKSIDE_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CCombatSequence_BanditAttackSide_Methods::GetName(CCharString* result) calling the stand-in CCharString::Init); the retail RTTI vftable placement names this PDB member here.
//   The by-value PDB return builds the result through CCharString::CCharString(const char*, long) (0x0099EBF0, both PDBs ??0CCharString@@QAE@PBDJ@Z, its landed self row).
#define FABLE_RETAIL_CCOMBATSEQUENCE_BANDITATTACKSIDE_VIRTUAL_MODEL
#define FABLE_RETAIL_CCOMBATSEQUENCEBASE
#include "engine/CCombatSequence_BanditAttackSide.h"

class CCharString { public:
    char* m_data;
    CCharString(const char* s, long n);
};

CCharString CCombatSequence_BanditAttackSide::GetName() const
{
    return CCharString("COMBAT_SEQUENCE_BANDIT_ATTACK_SIDE", -1);
}
