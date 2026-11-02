// CTCTrophy::$E_InstructionsText() -- dynamic initializer of the static CCharString CTCTrophy::InstructionsText[2] (the trophy rules tutorial text keys "TEXT_QST_TUTORIAL_TROPHYRULES_PC_10" / "_20"), then its array atexit destructor is registered
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0121E3A0, 46 bytes, in `.text$yc`, reached from the .CRT$XCU table entry
//   0x01374464, plain `ret`: two `push -1 ; push <literal> ; mov ecx,<element> ; call 0x0099EBF0` into 0x013BAB84 / 0x013BAB88,
//   then `push 0x012294D0 ; call _atexit ; pop ecx` (one atexit for the whole array).
// Identity (proven, two sources): (1) Ego_r.pdb 0x01010D10 / 0x01010E30 / 0x01010E50 / 0x01010E80 and FableWin.pdb run
//   CTCWife::WifeAppearanceDialogueGroupStemNames -> REACTION_MANAGER_DEF_INSTANTIATION_NAME -> `dynamic initializer for
//   'CTCTrophy::InstructionsText'' -> MIN_SPEECH_FRAMES; retail runs the landed 0x0121E260 (WifeAppearanceDialogueGroupStemNames)
//   -> 0x0121E380 ("OPINION_REACTION_MANAGER_INSTANCE") -> this -> 0x0121E3D0 (fps * 5), and the atexit pair is
//   `dynamic atexit destructor for 'CTCTrophy::InstructionsText'' (Ego_r 0x0101C200) = retail 0x012294D0, a two-element reverse
//   ~CCharString loop over 0x013BAB84; Ego_r's initializer / atexit pair are 46 / 32 bytes, as retail's (and its next entry
//   MIN_SPEECH_FRAMES 14 bytes = retail 0x0121E3D0); (2) the literals are the trophy rules tutorial keys and the one reader of 0x013BAB84,
//   the CTCTrophy member at 0x006DEEE0 (0x006DF1F6 / 0x006DF306), indexes the table. Catalog row "_gapscan::sub_0121e3a0" carried
//   no name; the atexit thunk 0x012294D0 is landed under the wrong label `CFontBankStateBlock::CreateCRC` over an invented
//   `g_AutoTinyReverseLoopObjects_013bab84` array (NAME_FIXES.tsv / labels).
// Data symbol: ?InstructionsText@CTCTrophy@@2PAVCCharString@@A -- public static assumed (house style, as the landed
//   CTCWife / CTCFireheartMinigame tables; no PDB data-symbol dump). CTCTrophy is declared here for its static only (class key V).
// Literals (checked against Fable.exe, the grader masks them): element 0 0x0125FEB4 "TEXT_QST_TUTORIAL_TROPHYRULES_PC_10",
//   element 1 0x0125FE90 "TEXT_QST_TUTORIAL_TROPHYRULES_PC_20".
// Callees: CCharString::CCharString(const char*, long = -1) 0x0099EBF0 (??0CCharString@@QAE@PBDJ@Z), _atexit 0x004012BC.
// Shape: a braced array initializer of a class with a converting constructor: each element is built in place through the
//   implicit CCharString(const char*, long = -1). Header: fable_string.h with the FABLE_RETAIL_CCHARSTRING opt-in (hdr/README.md:
//   retail's constructor has the default length; the default branch declares it without one).
#define FABLE_RETAIL_CCHARSTRING
#include "fable_string.h"      // CCharString(const char*, long = -1) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0

class CTCTrophy {                                        // class key V (pdb_class_keys.tsv)
public:
    static CCharString InstructionsText[2];              // 0x013BAB84
};

CCharString CTCTrophy::InstructionsText[2] = {
    "TEXT_QST_TUTORIAL_TROPHYRULES_PC_10",               // 0x0125FEB4
    "TEXT_QST_TUTORIAL_TROPHYRULES_PC_20"                // 0x0125FE90
};