#ifndef FABLE_ENGINE_CMAINGAMECOMPONENTINIT_H
#define FABLE_ENGINE_CMAINGAMECOMPONENTINIT_H
// Chunk 2051 (row 0x0042EC7C CNewFrontendGameComponent::Run). Ego_r.pdb CMainGameComponentInit (0x10 bytes): InitialWorldName
// +0x00, InitialWorldHolySiteName +0x04, InitialQuestName +0x08, SaveGameName +0x0C; ctor ??0CMainGameComponentInit@@QAE@XZ
// 0x004131A0 and dtor ??1CMainGameComponentInit@@QAE@XZ 0x004131D0 out of line (both PDBs).
#include "fable_string.h"
class CMainGameComponentInit {
public:
#ifdef FABLE_RETAIL_CMAINGAMECOMPONENTINIT_INLINE_CTOR_DTOR
    // NF235 opt-in (2026-11-22, q12656; row 0x00496CF0 CFrontendGameComponent::Run): that TU expands the default ctor and the dtor in
    // place (0x00496DBC..0x00496DDB: CWideString() 0x0099AED0 x2, CCharString() 0x0099E4B0, CWideString() for the four members in order;
    // 0x00496E44..0x00496E69: ~CWideString 0x0099B510 / ~CCharString 0x0099EAE0 in reverse), where other TUs call the out-of-line copies
    // 0x004131A0 / 0x004131D0. Same symbols (??0CMainGameComponentInit@@QAE@XZ / ??1CMainGameComponentInit@@QAE@XZ), inline here; their
    // pick-any copies equal those owners. No layout change.
    CMainGameComponentInit() { }                                              // 0x004131A0
    CMainGameComponentInit(const CMainGameComponentInit& other);
    ~CMainGameComponentInit() { }                                             // 0x004131D0
#else
    CMainGameComponentInit();                                                 // 0x004131A0
    CMainGameComponentInit(const CMainGameComponentInit& other);
    ~CMainGameComponentInit();                                                // 0x004131D0
#endif
    CWideString InitialWorldName;                                             // +0x00
    CWideString InitialWorldHolySiteName;                                     // +0x04
    CCharString InitialQuestName;                                             // +0x08
    CWideString SaveGameName;                                                 // +0x0C
};
#endif
