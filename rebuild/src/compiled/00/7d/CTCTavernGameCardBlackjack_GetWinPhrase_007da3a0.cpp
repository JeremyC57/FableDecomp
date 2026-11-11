// CTCTavernGameCardBlackjack::GetWinPhrase() -- blackjack has no win phrase: fetches the tavern-game def (unused) and returns an empty string
// Leaf: ?GetWinPhrase@CTCTavernGameCardBlackjack@@UAE?AVCWideString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007DA3A0, thiscall, `ret 4`.
// Identity (agentFAM03 proof, re-verified by agentFAM05): slot 42 of CTCTavernGameCardBlackjack's vtable 0x0126F8F4 (the slot holds this body in 1 vtable(s), owner CTCTavernGameCardBlackjack); the Xbox 360 Test PDB virtual order (third source) aligned on 34 TLC-named slots of that vtable puts GetWinPhrase(void) at this slot [gap kind GS]; Ego_r/FableWin name set: ?GetWinPhrase@CTCTavernGameCardBlackjack@@UAE?AVCWideString@@XZ; `ret` matches the signature.
// Was: CTCTavernGameCardBlackjack::vfunc_42 (rebuild/src/compiled/00/7d/CTCTavernGameCardBlackjack_vfunc42_007da3a0.cpp).
// No shared header for CTCTavernGameCardBlackjack: minimal local definition on its RTTI base CTCTavernGameCardBase.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTAVERNGAME_GETDEF_OUTOFLINE
#define FABLE_RETAIL_CTAVERNGAMECARDBASEDEF
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#include "fable_game.h"
#include "engine/CTavernGameDef.h"
#include "engine/CTavernGameCardBaseDef.h"
#include "engine/card_sorting_types.h"
class CTCTavernGameCardBlackjack : public CTCTavernGameCardBase {
public:
    virtual CWideString GetWinPhrase();                     // slot 42
};
CWideString CTCTavernGameCardBlackjack::GetWinPhrase()
{
    CDefPointer<const CTavernGameDef> pdef(NULL_DEF);
    GetDef(&pdef);
    return CCharString("").ToWideString();
}
