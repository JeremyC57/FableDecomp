// CTCTavernGame::SetState(ETavernGameStates state) -- change the tavern game's state: a pending quit request turns any new
//   state into the quit state 47; state 5 marks the game as played; 47 leaves the game (virtual LeaveGame) without storing.
// Leaf: ?SetState@CTCTavernGame@@QAEXW4ETavernGameStates@1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005E9B20, thiscall, `ret 4`. Name and signature in FableWin.pdb + Ego_r name sets
//   (NAME_FIXES_QUEUE cx11p); the same body is defined in the 0x005EB9E0 FrameUpdate TU, where /O2 inlines it at the
//   constant-state sites (retail's own TU layout); this row is its out-of-line copy.
// Callees: virtual LeaveGame [+0x78] (vtable slot 30, 0x005EC650). Data: 0x013BAE1C, the quit-request flag (descriptive
//   g_bTavernGameQuitRequested, no PDB data name; the spelling six landed rows bind). States stay numeric (no enumerator
//   names proven).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCTAVERNGAME
#include "engine/CTCTavernGame.h"

extern bool g_bTavernGameQuitRequested;                  // 0x013BAE1C

void CTCTavernGame::SetState(ETavernGameStates state)
{
    if (g_bTavernGameQuitRequested) {
        g_bTavernGameQuitRequested = false;
        state = (ETavernGameStates)47;
    }
    if (state == 5)
        GamePlayed = true;
    else if (state == 47) {
        LeaveGame();
        return;
    }
    PreviousTavernGameState = TavernGameState;
    TavernGameState = state;
}