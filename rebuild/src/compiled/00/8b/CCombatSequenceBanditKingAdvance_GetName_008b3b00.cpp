// CCombatSequence_BanditKingAdvance_Methods::GetName -- returns CCharString by value.
// Retail: builds a CCharString from a string literal (len = -1) into the
// hidden return buffer and returns it. 'this' is unused.

// Leaf: ?GetName@CCombatSequence_BanditKingAdvance@@UBE?AVCCharString@@XZ
// Class-model move (NF325, 2026-11-24): defined on the real class of engine/CCombatSequence_BanditKingAdvance.h under FABLE_RETAIL_CCOMBATSEQUENCE_BANDITKINGADVANCE_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CCombatSequence_BanditKingAdvance_Methods::GetName); the retail RTTI vftable placement names this PDB member here.
#define FABLE_RETAIL_CCOMBATSEQUENCE_BANDITKINGADVANCE_VIRTUAL_MODEL
#define FABLE_RETAIL_CCOMBATSEQUENCEBASE
#include "engine/CCombatSequence_BanditKingAdvance.h"  // retyped onto the PDB layout; byte parity re-verified
class CCharString { public:
    char* m_ptr;
    CCharString(const char* s, long len);
};

CCharString CCombatSequence_BanditKingAdvance::GetName() const {
    return CCharString("COMBAT_SEQUENCE_BANDIT_KING_ADVANCE", -1);
}
