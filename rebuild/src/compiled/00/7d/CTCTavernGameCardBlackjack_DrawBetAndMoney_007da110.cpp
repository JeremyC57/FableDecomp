// CTCTavernGameCardBlackjack::DrawBetAndMoney(CEnginePrimitivePrepareInfo const&, float, float, float, float, float, float) -- blackjack draws the bet and money like any tavern game
// Leaf: ?DrawBetAndMoney@CTCTavernGameCardBlackjack@@UAEXABVCEnginePrimitivePrepareInfo@@MMMMMM@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007DA110, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 50 of CTCTavernGameCardBlackjack's vtable 0x0126F8F4 (the slot holds this body in 1 vtable(s), owner CTCTavernGameCardBlackjack); the Xbox 360 Test PDB virtual order (third source) aligned on 34 TLC-named slots of that vtable puts DrawBetAndMoney(CEnginePrimitivePrepareInfo const &, float, float, float, float, float, float) at this slot [gap kind GS]; Ego_r/FableWin name set: ?DrawBetAndMoney@CTCTavernGameCardBlackjack@@UAEXABVCEnginePrimitivePrepareInfo@@MMMMMM@Z; no `ret` to check (NORET).
// Was: CTCTavernGameCardBlackjack::vfunc_50 (rebuild/src/compiled/00/7d/gapscan_sub007da110_007da110.cpp).
// No shared header for CTCTavernGameCardBlackjack: minimal local definition on its RTTI base CTCTavernGameCardBase.
#define FABLE_RETAIL_CTCTAVERNGAME
#include "engine/CTCTavernGame.h"
#include "engine/card_sorting_types.h"
class CTCTavernGameCardBlackjack : public CTCTavernGameCardBase {
public:
    virtual void DrawBetAndMoney(const CEnginePrimitivePrepareInfo&, float, float, float, float, float, float);   // slot 50
};
void CTCTavernGameCardBlackjack::DrawBetAndMoney(const CEnginePrimitivePrepareInfo& info, float x, float y, float width, float height,
                                                 float textX, float textY)
{
    CTCTavernGame::DrawBetAndMoney(info, x, y, width, height, textX, textY);
}
