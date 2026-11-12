// CCombatSequenceWhisperSortYourselfOut::StatGetName() -- the sequence's registry name "COMBAT_SEQUENCE_WHISPER_SORT_YOURSELF_OUT"
// Leaf: ?StatGetName@CCombatSequenceWhisperSortYourselfOut@@SI?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008B0440, 19 bytes, static __fastcall with the hidden CCharString result in ecx, plain `ret`.
// Identity (proven, agentFAM07): the body builds a CCharString from the literal 0x01278DE4 "COMBAT_SEQUENCE_WHISPER_SORT_YOURSELF_OUT" into the hidden result.
//   The same literal is the one CCombatSequenceWhisperSortYourselfOut::GetName 0x008B4D20 returns and the one CCombatManager::RegisterCombatSequences 0x008B0D10
//   registers (0x008B13A3); Ego_r.pdb has ?StatGetName@CCombatSequenceWhisperSortYourselfOut@@SI?AVCCharString@@XZ, and the 0x20-spaced StatGetName run
//   0x008B0000..0x008B05E0 follows the registration order (0x008B0000 CCombatSequence_BanditKingSpinAround::StatGetName landed).
//   No caller in Fable.exe (the xref hits are immediates inside other instructions). Was the placeholder
//   CUnprovenCombatNameFactory_008b0440.
// Callees: 0x0099EBF0 ??0CCharString@@QAE@PBDJ@Z (Ego_r.pdb spelling, as the landed 0x008B0000).
// Shape: `push -1 ; push offset literal ; mov esi,ecx ; call ctor ; mov eax,esi`.
// Model (agentNF24 port): fable_string.h (CCharString) and engine/CCombatSequenceWhisperSortYourselfOut.h with the opt-in FABLE_RETAIL_COMBATSEQUENCE_STATGETNAME
//   (the static StatGetName declaration) replace the draft's local CCharString and class models.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_COMBATSEQUENCE_STATGETNAME
#include "fable_string.h"
#include "engine/CCombatSequenceWhisperSortYourselfOut.h"

CCharString FABLE_FASTCALL CCombatSequenceWhisperSortYourselfOut::StatGetName()
{
    return CCharString("COMBAT_SEQUENCE_WHISPER_SORT_YOURSELF_OUT", -1);
}
