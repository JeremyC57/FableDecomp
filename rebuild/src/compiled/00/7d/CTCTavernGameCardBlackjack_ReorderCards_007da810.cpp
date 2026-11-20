// CTCTavernGameCardBlackjack::ReorderCards() -- for every card of every player hand play the deal sound and move the card to its laid-out position for that hand and slot
// Leaf: ?ReorderCards@CTCTavernGameCardBlackjack@@QAEXXZ
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DA810, length 160 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 ("TAVERN_GAME_CARD_DEAL" @ 0x0126F568), CTCTavernGame::PlaySoundW(const
// CCharString&) @ 0x005EA950, ~CCharString @ 0x0099EAE0, CTCTavernGameCardBlackjack::GetPosition(long hand, long card) @ 0x007DA760 (C3DVector
// through the hidden pointer), CCard::Move(const C3DVector&, bool) @ 0x008F0660 (false).
// Layout (retail, as 0x007DA120): PlayersCards (vector<vector<CCard*> >, 12-byte elements) +0x324/+0x328.
// Shape: the hand index is stepped in the outer for header before the hand pointer (`++index, ++hand`, kept in the frame; the other
// order puts the index in ecx); the inner bound
// is `size()` re-read each pass (`sar ; jb`); the sound name is a named CCharString in its own block; STLport operator[] as `*(begin()+n)`
// (begin() read before the Move argument is built).
// Naming (NF159, 2026-11-20, q4273): ported onto engine/CTCTavernGameCardBlackjack.h (retail branch; NF159 opt-in
//   FABLE_RETAIL_CTCTAVERNGAMECARDBLACKJACK_REORDERCARDS; GetPosition from FABLE_RETAIL_BLACKJACK_DEAL_PASS23) from local CTCTavernGameCardBlackjack /
//   CCard / CVector / CCharString stand-ins: ?PlaySoundW@CTCTavernGame@@IAEKABVCCharString@@@Z (was
//   ?PlaySound@CTCTavernGameCardBlackjack@@QAEKABVCCharString@@@Z). Own leaf: Ego_r.pdb 0x007D7220 / FableWin.pdb 0x0275AEB0
//   ?ReorderCards@CTCTavernGameCardBlackjack@@QAEXXZ (unchanged).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_BLACKJACK_DEAL_PASS23
#define FABLE_RETAIL_CTCTAVERNGAMECARDBLACKJACK_REORDERCARDS
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CTCTavernGameCardBlackjack.h"

void CTCTavernGameCardBlackjack::ReorderCards()
{
    long handIndex = 0;
    for (std::vector<std::vector<CCard*> >::iterator hand = PlayersCards.begin(); hand != PlayersCards.end(); ++handIndex, ++hand) {
        for (unsigned int i = 0; i < hand->size(); ++i) {
            {
                CCharString sound("TAVERN_GAME_CARD_DEAL");
                PlaySoundW(sound);
            }
            (*hand)[i]->Move(GetPosition(handIndex, i), false);
        }
    }
}
