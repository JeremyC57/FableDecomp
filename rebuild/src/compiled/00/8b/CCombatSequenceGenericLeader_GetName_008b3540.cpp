// CCombatSequence_GenericLeader_Methods::GetName @ 008b3540
// __fastcall (modeled as member) returning CCharString by value:
// hidden retptr on stack, this in ecx, ret 4. Constructs a CCharString
// from the string literal at 0x12786d4 with length -1.

// Leaf: ?GetName@CCombatSequence_GenericLeader@@UBE?AVCCharString@@XZ
// Class-model move (NF325, 2026-11-24): defined on the real class of engine/CCombatSequence_GenericLeader.h under FABLE_RETAIL_CCOMBATSEQUENCE_GENERICLEADER_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CCombatSequence_GenericLeader_Methods::GetName); the retail RTTI vftable placement names this PDB member here.
#define FABLE_RETAIL_CCOMBATSEQUENCE_GENERICLEADER_VIRTUAL_MODEL
#define FABLE_RETAIL_CCOMBATSEQUENCEBASE
#include "engine/CCombatSequence_GenericLeader.h"  // retyped onto the PDB layout; byte parity re-verified
class CCharString { public:
    char* p;
    CCharString(const char* s, long len);
};

CCharString CCombatSequence_GenericLeader::GetName() const
{
    return CCharString("COMBAT_SEQUENCE_GENERIC_LEADER", -1);
}
