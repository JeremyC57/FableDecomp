// CTCTavernGameCoinBase::OnInitialActivate() -- a coin game activates like any tavern game
// Leaf: ?OnInitialActivate@CTCTavernGameCoinBase@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008EF1A0, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 6 of CTCTavernGameCoinBase's vtable 0x0126F224 (the slot holds this body in 1 vtable(s), owner CTCTavernGameCoinBase); the Xbox 360 Test PDB virtual order (third source) aligned on 26 TLC-named slots of that vtable puts OnInitialActivate(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?OnInitialActivate@CTCTavernGameCoinBase@@UAEXXZ; no `ret` to check (NORET).
// Was: CTCTavernGameCoinBase::vfunc_6 (rebuild/src/compiled/00/8e/gapscan_sub008ef1a0_008ef1a0.cpp).
// No shared header for CTCTavernGameCoinBase: minimal local definition on its RTTI base CTCTavernGame.
#define FABLE_RETAIL_CTCTAVERNGAME
#include "engine/CTCTavernGame.h"
class CTCTavernGameCoinBase : public CTCTavernGame {
public:
    virtual void OnInitialActivate();                        // slot 6
};
void CTCTavernGameCoinBase::OnInitialActivate()
{
    CTCTavernGame::OnInitialActivate();
}
