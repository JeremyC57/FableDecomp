// CCombatSequenceWhisperTooCloseLeapBack_Methods::GetName -> returns CCharString by value.
// Leaf: ?GetName@CCombatSequenceWhisperTooCloseLeapBack@@UBE?AVCCharString@@XZ
// Class-model move (NF325, 2026-11-24): defined on the real class of engine/CCombatSequenceWhisperTooCloseLeapBack.h under FABLE_RETAIL_CCOMBATSEQUENCEWHISPERTOOCLOSELEAPBACK_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CCombatSequenceWhisperTooCloseLeapBack_Methods::GetName); the retail RTTI vftable placement names this PDB member here.
#define FABLE_RETAIL_CCOMBATSEQUENCEWHISPERTOOCLOSELEAPBACK_VIRTUAL_MODEL
#define FABLE_RETAIL_CCOMBATSEQUENCEBASE
#include "engine/CCombatSequenceWhisperTooCloseLeapBack.h"  // retyped onto the PDB layout; byte parity re-verified
class CCharString { public:
    char* p;
    // ctor at 0x99ebf0: (char const*, int)
    CCharString(const char* s, long n);
};

CCharString CCombatSequenceWhisperTooCloseLeapBack::GetName() const
{
    return CCharString("COMBAT_SEQUENCE_WHISPER_TOO_CLOSE_LEAP_BACK", -1);
}
