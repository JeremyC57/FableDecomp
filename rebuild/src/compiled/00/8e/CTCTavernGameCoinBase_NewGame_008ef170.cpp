// CTCTavernGameCoinBase::NewGame() -- a coin game starts like any tavern game
// Leaf: ?NewGame@CTCTavernGameCoinBase@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008EF170, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 31 of CTCTavernGameCoinBase's vtable 0x0126F224 (the slot holds this body in 1 vtable(s), owner CTCTavernGameCoinBase); the Xbox 360 Test PDB virtual order (third source) aligned on 26 TLC-named slots of that vtable puts NewGame(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?NewGame@CTCTavernGameCoinBase@@UAEXXZ; no `ret` to check (NORET).
// Was: CTCTavernGameCoinBase::vfunc_31 (rebuild/src/compiled/00/8e/CTCTavernGame_NewGame_008ef170.cpp).
// No shared header for CTCTavernGameCoinBase: minimal local definition on its RTTI base CTCTavernGame.
#define FABLE_RETAIL_CTCTAVERNGAME
#include "engine/CTCTavernGame.h"
class CTCTavernGameCoinBase : public CTCTavernGame {
public:
    virtual void NewGame();                                  // slot 31
};
void CTCTavernGameCoinBase::NewGame()
{
    CTCTavernGame::NewGame();
}
