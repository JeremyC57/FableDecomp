// Leaf: ?GetName@CCombatSequenceWhisperLunge@@UBE?AVCCharString@@XZ
// Class-model move (NF325, 2026-11-24): defined on the real class of engine/CCombatSequenceWhisperLunge.h under FABLE_RETAIL_CCOMBATSEQUENCEWHISPERLUNGE_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CCombatSequenceWhisperLunge_Methods::GetName); the retail RTTI vftable placement names this PDB member here.
#define FABLE_RETAIL_CCOMBATSEQUENCEWHISPERLUNGE_VIRTUAL_MODEL
#define FABLE_RETAIL_CCOMBATSEQUENCEBASE
#include "engine/CCombatSequenceWhisperLunge.h"

class CCharString { public: char* m_data; CCharString(); CCharString(const char*, long); CCharString(const CCharString&); ~CCharString(); };
CCharString CCombatSequenceWhisperLunge::GetName() const { return CCharString("COMBAT_SEQUENCE_WHISPER_LUNGE", -1); }
