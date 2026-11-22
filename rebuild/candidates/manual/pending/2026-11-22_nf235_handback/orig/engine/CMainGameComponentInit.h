#ifndef FABLE_ENGINE_CMAINGAMECOMPONENTINIT_H
#define FABLE_ENGINE_CMAINGAMECOMPONENTINIT_H
// Chunk 2051 (row 0x0042EC7C CNewFrontendGameComponent::Run). Ego_r.pdb CMainGameComponentInit (0x10 bytes): InitialWorldName
// +0x00, InitialWorldHolySiteName +0x04, InitialQuestName +0x08, SaveGameName +0x0C; ctor ??0CMainGameComponentInit@@QAE@XZ
// 0x004131A0 and dtor ??1CMainGameComponentInit@@QAE@XZ 0x004131D0 out of line (both PDBs).
#include "fable_string.h"
class CMainGameComponentInit {
public:
    CMainGameComponentInit();                                                 // 0x004131A0
    CMainGameComponentInit(const CMainGameComponentInit& other);
    ~CMainGameComponentInit();                                                // 0x004131D0
    CWideString InitialWorldName;                                             // +0x00
    CWideString InitialWorldHolySiteName;                                     // +0x04
    CCharString InitialQuestName;                                             // +0x08
    CWideString SaveGameName;                                                 // +0x0C
};
#endif
