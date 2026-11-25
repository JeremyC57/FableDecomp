// CCombatSequence_TrollOpeningGroundPound_Methods::GetName  @ 008b40f0
// virtual public const, returns CCharString by value.
// Body constructs the return CCharString in place (NRVO into the hidden
// return buffer) from a fixed global literal pointer with length -1.
//
// CCharString(const char*, long) ctor lives at 0x0099ebf0.  'this' (ecx of
// GetName) is ignored by the body.

// Leaf: ?GetName@CCombatSequence_TrollOpeningGroundPound@@UBE?AVCCharString@@XZ
// Class-model move (NF325, 2026-11-24): defined on the real class of engine/CCombatSequence_TrollOpeningGroundPound.h under FABLE_RETAIL_CCOMBATSEQUENCE_TROLLOPENINGGROUNDPOUND_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CCombatSequence_TrollOpeningGroundPound_Methods::GetName); the retail RTTI vftable placement names this PDB member here.
#define FABLE_RETAIL_CCOMBATSEQUENCE_TROLLOPENINGGROUNDPOUND_VIRTUAL_MODEL
#define FABLE_RETAIL_CCOMBATSEQUENCEBASE
#include "engine/CCombatSequence_TrollOpeningGroundPound.h"  // retyped onto the PDB layout; byte parity re-verified
class CCharString { public:
    void* p;
    CCharString(const char* s, long len);   // 0x0099ebf0
};

CCharString CCombatSequence_TrollOpeningGroundPound::GetName() const
{
    return CCharString("COMBAT_SEQUENCE_TROLL_OPENING_GROUND_POUND", -1);
}
