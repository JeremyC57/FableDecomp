#ifndef FABLE_ENGINE_CINPUTPROCESSGAMEBASEINIT_H
#define FABLE_ENGINE_CINPUTPROCESSGAMEBASEINIT_H
// Hand-written (chunk 873, row 0x00488D10 CPlayer::InitInterfaces): the argument block every game input process is constructed from.
// Ego_r.pdb CInputProcessGameBaseInit, 16 bytes: World +0x00, MyPlayer +0x04, DefinitionManager +0x08, Component +0x0c
// (ghidra_out/struct_layouts_egor.tsv); the landed CInputProcessGameBase constructor 0x00687A30 copies them in this order. FableWin.pdb
// lists only the default constructor ??0CInputProcessGameBaseInit@@QAE@XZ (inline in retail, no store: InitInterfaces fills the four
// pointers itself).
#include "rebuild_abi.h"
class CGameDefinitionManager;
class CMainGameComponent;
class CPlayer;
class CWorld;
class CInputProcessGameBaseInit {
public:
    CInputProcessGameBaseInit() { }
    CWorld*                 World;                                       // +0x00
    CPlayer*                MyPlayer;                                    // +0x04
    CGameDefinitionManager* DefinitionManager;                           // +0x08
    CMainGameComponent*     Component;                                   // +0x0c
};
FABLE_STATIC_ASSERT(sizeof(CInputProcessGameBaseInit) == 0x10);
#endif // FABLE_ENGINE_CINPUTPROCESSGAMEBASEINIT_H
