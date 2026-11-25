// CCombatSequenceWhisperSortYourselfOut_Methods::GetName
// retail 0x008b4d20 : returns CCharString by value from a fixed string literal.
// The hidden struct-return buffer is constructed in place via the CCharString
// ctor at 0x99ebf0 (const char*, long len = -1). 'this' (ecx) is unused.

// Leaf: ?GetName@CCombatSequenceWhisperSortYourselfOut@@UBE?AVCCharString@@XZ
// Class-model move (NF325, 2026-11-24): defined on the real class of engine/CCombatSequenceWhisperSortYourselfOut.h under FABLE_RETAIL_CCOMBATSEQUENCEWHISPERSORTYOURSELFOUT_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CCombatSequenceWhisperSortYourselfOut_Methods::GetName); the retail RTTI vftable placement names this PDB member here.
#define FABLE_RETAIL_CCOMBATSEQUENCEWHISPERSORTYOURSELFOUT_VIRTUAL_MODEL
#define FABLE_RETAIL_CCOMBATSEQUENCEBASE
#include "engine/CCombatSequenceWhisperSortYourselfOut.h"  // retyped onto the PDB layout; byte parity re-verified
class CCharString
{ public:
    void* buf;
    CCharString(const char* s, long len);   // ctor @ 0x0099ebf0 (not defined here)
};

CCharString CCombatSequenceWhisperSortYourselfOut::GetName() const
{
    return CCharString("COMBAT_SEQUENCE_WHISPER_SORT_YOURSELF_OUT", -1);
}
