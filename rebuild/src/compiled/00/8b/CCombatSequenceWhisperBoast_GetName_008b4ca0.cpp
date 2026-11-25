// Leaf: ?GetName@CCombatSequenceWhisperBoast@@UBE?AVCCharString@@XZ
// Class-model move (NF325, 2026-11-24): defined on the real class of engine/CCombatSequenceWhisperBoast.h under FABLE_RETAIL_CCOMBATSEQUENCEWHISPERBOAST_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CCombatSequenceWhisperBoast_Methods::GetName); the retail RTTI vftable placement names this PDB member here.
#define FABLE_RETAIL_CCOMBATSEQUENCEWHISPERBOAST_VIRTUAL_MODEL
#define FABLE_RETAIL_CCOMBATSEQUENCEBASE
#include "engine/CCombatSequenceWhisperBoast.h"

class CCharString { public: char* m_pData; int m_nLength; CCharString(const char*, long); CCharString(const CCharString&); ~CCharString(); };
CCharString CCombatSequenceWhisperBoast::GetName() const { return CCharString("COMBAT_SEQUENCE_WHISPER_BOAST", -1); }
