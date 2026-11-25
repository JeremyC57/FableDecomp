// Leaf: ?GetName@CCombatSequenceWhisperStrikeStrafeBack@@UBE?AVCCharString@@XZ
// Class-model move (NF325, 2026-11-24): defined on the real class of engine/CCombatSequenceWhisperStrikeStrafeBack.h under FABLE_RETAIL_CCOMBATSEQUENCEWHISPERSTRIKESTRAFEBACK_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CCombatSequenceWhisperStrikeStrafeBack_Methods::GetName); the retail RTTI vftable placement names this PDB member here.
#define FABLE_RETAIL_CCOMBATSEQUENCEWHISPERSTRIKESTRAFEBACK_VIRTUAL_MODEL
#define FABLE_RETAIL_CCOMBATSEQUENCEBASE
#include "engine/CCombatSequenceWhisperStrikeStrafeBack.h"

class CCharString { public: char* m_data; CCharString(); CCharString(const char*, long); CCharString(const CCharString&); ~CCharString(); };
CCharString CCombatSequenceWhisperStrikeStrafeBack::GetName() const { return CCharString("COMBAT_SEQUENCE_WHISPER_STRIKE_STRAFE_BACK", -1); }
