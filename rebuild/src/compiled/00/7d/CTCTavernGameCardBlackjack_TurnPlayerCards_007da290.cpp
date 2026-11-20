// CTCTavernGameCardBlackjack::TurnPlayerCards() -- flips the face-up cards of the current hand face down (with the flip sound), then moves on to the next player hand, or ends the player's round after the last one
// Leaf: ?TurnPlayerCards@CTCTavernGameCardBlackjack@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DA290, length 207 bytes (plain `ret`). Ego_r.pdb
// `?TurnPlayerCards@CTCTavernGameCardBlackjack@@QAEXXZ`.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 ("TAVERN_GAME_CARD_FLIP" @ 0x0126F6C4),
// CTCTavernGame::PlaySoundW(const CCharString&) @ 0x005EA950,
// ~CCharString @ 0x0099EAE0, CCard::Turn @ 0x008F0620, CTCTavernGame::SetState(ETavernGameStates) @ 0x005E9B20 (0x16).
// Layout (retail, as the landed 0x007DA120 / 0x007DA810 model: 12-byte STLport vectors and the 0x318-byte retail
// CTCTavernGameCardBase, so the members sit 0x20..0x2c below Ego_r.pdb): PlayersCards +0x324 (12-byte elements, hence
// the divide by 12), CurrentPlayerCards +0x33c, CurrentPlayerCardsIndex +0x354, HasPlayerFinishedRound +0x369.
// CCard::FaceDown +0x25.
// Shape: the hand walk re-reads `CurrentPlayerCards->end()` every pass; the sound name is a named CCharString in its own
// block (the `push ecx` frame word); the index is a `long` against the unsigned size() (`jb`); STLport operator[] as
// `*(begin() + n)`. The single SetState call after the if/else is tail-duplicated into both arms by VC7.1.
// Naming (NF159, 2026-11-20, q4271 / q4273): ported onto engine/CTCTavernGameCardBlackjack.h (retail branch; NF159 opt-in
//   FABLE_RETAIL_CTCTAVERNGAMECARDBLACKJACK_TURNPLAYERCARDS) from local CTCTavernGameCardBlackjack / CCard / CVector / CCharString stand-ins:
//   ?PlaySoundW@CTCTavernGame@@IAEKABVCCharString@@@Z (was ?PlaySound@CTCTavernGameCardBlackjack@@QAEKABVCCharString@@@Z) and
//   ?SetState@CTCTavernGame@@QAEXW4ETavernGameStates@1@@Z (was ?SetState@CTCTavernGameCardBlackjack@@QAEXJ@Z); 0x16 is ETGS_CARDS_TURNING_UP.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CTCTAVERNGAMECARDBLACKJACK_TURNPLAYERCARDS
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CTCTavernGameCardBlackjack.h"

void CTCTavernGameCardBlackjack::TurnPlayerCards()
{
    for (std::vector<CCard*>::iterator it = CurrentPlayerCards->begin(); it != CurrentPlayerCards->end(); ++it) {
        if (!(*it)->FaceDown) {
            {
                CCharString sound("TAVERN_GAME_CARD_FLIP");
                PlaySoundW(sound);
            }
            (*it)->Turn();
        }
    }
    ++CurrentPlayerCardsIndex;
    if (CurrentPlayerCardsIndex >= PlayersCards.size()) {
        CurrentPlayerCards = 0;
        HasPlayerFinishedRound = true;
        CurrentPlayerCardsIndex = 0;
    } else {
        CurrentPlayerCards = &PlayersCards[CurrentPlayerCardsIndex];
    }
    SetState(ETGS_CARDS_TURNING_UP);
}
