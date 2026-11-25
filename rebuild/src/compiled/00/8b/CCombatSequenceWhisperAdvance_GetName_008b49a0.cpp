// CCombatSequenceWhisperAdvance_Methods::GetName  (retail 0x008b49a0)
// __fastcall const method, returns CCharString by value (hidden return-buffer ptr).
// Body:  CCharString::CCharString(retbuf, "literal", -1);  return retbuf;
// The original 'this' is never read.

// CCharString::CCharString(const char*, long) @ 0x0099ebf0 is a real __fastcall ctor
// (ecx = this, args const char* and int on the stack, no edx).
// Leaf: ?GetName@CCombatSequenceWhisperAdvance@@UBE?AVCCharString@@XZ
// Class-model move (NF325, 2026-11-24): defined on the real class of engine/CCombatSequenceWhisperAdvance.h under FABLE_RETAIL_CCOMBATSEQUENCEWHISPERADVANCE_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CCombatSequenceWhisperAdvance_Methods::GetName); the retail RTTI vftable placement names this PDB member here.
#define FABLE_RETAIL_CCOMBATSEQUENCEWHISPERADVANCE_VIRTUAL_MODEL
#define FABLE_RETAIL_CCOMBATSEQUENCEBASE
#include "engine/CCombatSequenceWhisperAdvance.h"  // retyped onto the PDB layout; byte parity re-verified
class CCharString { public:
    char* p;
    CCharString(const char* s, long len);
};

// literal pointer baked in retail as 0x1278d00

CCharString CCombatSequenceWhisperAdvance::GetName() const
{
    return CCharString("COMBAT_SEQUENCE_WHISPER_ADVANCE", -1);
}
