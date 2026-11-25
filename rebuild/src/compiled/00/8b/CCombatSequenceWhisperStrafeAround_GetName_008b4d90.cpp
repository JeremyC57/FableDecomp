// CCombatSequenceWhisperStrafeAround_Methods::GetName  (retail 0x008b4d90)
// Virtual method returning CCharString by value from a fixed string literal.

// Leaf: ?GetName@CCombatSequenceWhisperStrafeAround@@UBE?AVCCharString@@XZ
// Class-model move (NF325, 2026-11-24): defined on the real class of engine/CCombatSequenceWhisperStrafeAround.h under FABLE_RETAIL_CCOMBATSEQUENCEWHISPERSTRAFEAROUND_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CCombatSequenceWhisperStrafeAround_Methods::GetName); the retail RTTI vftable placement names this PDB member here.
#define FABLE_RETAIL_CCOMBATSEQUENCEWHISPERSTRAFEAROUND_VIRTUAL_MODEL
#define FABLE_RETAIL_CCOMBATSEQUENCEBASE
#include "engine/CCombatSequenceWhisperStrafeAround.h"  // retyped onto the PDB layout; byte parity re-verified
class CCharString { public:
    char* m_buf;
    // CCharString::CCharString(char const*, int)  @ 0x0099ebf0  (__fastcall)
    CCharString(const char* s, long n);
};

CCharString CCombatSequenceWhisperStrafeAround::GetName() const
{
    return CCharString("COMBAT_SEQUENCE_WHISPER_STRAFE_AROUND", -1);
}
