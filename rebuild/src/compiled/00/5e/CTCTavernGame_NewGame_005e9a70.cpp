// CTCTavernGame::NewGame() -- starts a new game: the base bet is taken from the player's money and the round flags are cleared
// Leaf: ?NewGame@CTCTavernGame@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005E9A70, thiscall, `ret`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 31 of CTCTavernGame's vtable 0x012560BC (the slot holds this body in 1 vtable(s), owner CTCTavernGame); the Xbox 360 Test PDB virtual order (third source) aligned on 28 TLC-named slots of that vtable puts NewGame(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?NewGame@CTCTavernGame@@UAEXXZ; `ret` matches the signature.
// Was: CTCTavernGame::vfunc_31 (rebuild/src/compiled/00/5e/CTCTavernGame_NewGame_005e9a70.cpp).
#define FABLE_RETAIL_CTCTAVERNGAME
#include "engine/CTCTavernGame.h"
void CTCTavernGame::NewGame()
{
    CurrentMoney -= BaseBet;
    NewBestScore = false;
    GameStarted = false;
}
