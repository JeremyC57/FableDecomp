// CTCTavernGameCardBlackjack::ProcessInput(CInputEvent const&, CProcessedInput&, CInputProcessTavernGame*) -- blackjack takes the tavern game's input handling (not the card base's)
// Leaf: ?ProcessInput@CTCTavernGameCardBlackjack@@UAEXABVCInputEvent@@AAVCProcessedInput@@PAVCInputProcessTavernGame@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007DA010, thiscall, tail jump (no ret).
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 28 of CTCTavernGameCardBlackjack's vtable 0x0126F8F4 (the slot holds this body in 1 vtable(s), owner CTCTavernGameCardBlackjack); the Xbox 360 Test PDB virtual order (third source) aligned on 34 TLC-named slots of that vtable puts ProcessInput(CInputEvent const &, CProcessedInput &, CInputProcessTavernGame *) at this slot [gap kind GS]; Ego_r/FableWin name set: ?ProcessInput@CTCTavernGameCardBlackjack@@UAEXABVCInputEvent@@AAVCProcessedInput@@PAVCInputProcessTavernGame@@@Z; no `ret` to check (NORET).
// Was: CTCTavernGameCardBlackjack::vfunc_28 (rebuild/src/compiled/00/7d/gapscan_sub007da010_007da010.cpp).
// No shared header for CTCTavernGameCardBlackjack: minimal local definition on its RTTI base CTCTavernGameCardBase.
#define FABLE_RETAIL_CTCTAVERNGAME
#include "engine/CTCTavernGame.h"
#include "engine/card_sorting_types.h"
class CTCTavernGameCardBlackjack : public CTCTavernGameCardBase {
public:
    virtual void ProcessInput(const CInputEvent&, CProcessedInput&, CInputProcessTavernGame*);   // slot 28
};
void CTCTavernGameCardBlackjack::ProcessInput(const CInputEvent& event, CProcessedInput& processed, CInputProcessTavernGame* process)
{
    CTCTavernGame::ProcessInput(event, processed, process);
}
