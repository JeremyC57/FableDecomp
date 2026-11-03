#ifndef FABLE_ENGINE_CTESTQUEST_H
#define FABLE_ENGINE_CTESTQUEST_H
// CTestQuest (Ego_r.pdb, 28 bytes): a test-quest record CWorld::LoadQuestsFromFile reads from an AddTestQuest("quest", "hsp",
// colour, "display", "ini", "outi", "card") line and keeps in CWorld::ActiveTestQuests. Members at the PDB offsets (no STL member).
// Ego_r.pdb / FableWin.pdb ??0CTestQuest@@QAE@XZ / ??0CTestQuest@@QAE@ABV0@@Z / ??1CTestQuest@@QAE@XZ are the compiler-generated
// members (FableWin debug copies 0x01B46C30 / 0x01B46CE0 sit among the CWorld bodies); retail expands the default constructor and
// destructor in LoadQuestsFromFile (0x004A161D.., 0x004A1725..) and calls the copy constructor out of line (0x004A89D0).
#include "fable_string.h"
class CTestQuest {
public:
    CCharString QuestName;                                   // +0x00
    CCharString HSPName;                                     // +0x04
    long        ColourIndex;                                 // +0x08
    CCharString DisplayName;                                 // +0x0c
    CCharString IniFile;                                     // +0x10
    CCharString OutiFile;                                    // +0x14
    CCharString QuestCardDefName;                            // +0x18
};
#endif
