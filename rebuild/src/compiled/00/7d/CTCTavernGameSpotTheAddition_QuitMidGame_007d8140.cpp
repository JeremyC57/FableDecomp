// CTCTavernGameSpotTheAddition::QuitMidGame() -- marks the game finished and not won, slides the cover fully over (1.0) and enters state 6
// Retail: Fable.exe @ 0x007D8140, 32 bytes (vtable 0x0126F814 slot 34 = QuitMidGame; the landed
//         ShoveHaPenny slot-34 body 0x007CF520 is the same shape without the cover).
// Callees: CTCTavernGameSpotTheAddition::PositionCover(float) 0x007D7F10, CTCTavernGame::SetState(long)
//          0x005E9B20.
// Members: GameFinished +0x28 and GameWon +0x21 (CTCTavernGame, PDB +0x30/+0x29: retail 8 lower).
// Shape: the constant argument push (`push 3F800000h`) is hoisted above the two byte stores.
// Layout (supersedes any local-model note above): engine/CTCTavernGame.h now has the retail STLport 4.6 offsets, so the data
//   members come from the shared header; the class below keeps only the declarations this body needs.
#define CTCTavernGame CTCTavernGameDonor
#include "engine/CTCTavernGame.h"
#undef CTCTavernGame
class CTCTavernGame : public CTCTavernGameDonor {
 public:
    void SetState(long state);          // 0x005E9B20
};

struct CTCTavernGameSpotTheAddition : CTCTavernGame {
    void PositionCover(float amount);   // 0x007D7F10
    void QuitMidGame();
};

void CTCTavernGameSpotTheAddition::QuitMidGame()
{
    GameFinished = true;
    GameWon = false;
    PositionCover(1.0f);
    SetState(6);
}