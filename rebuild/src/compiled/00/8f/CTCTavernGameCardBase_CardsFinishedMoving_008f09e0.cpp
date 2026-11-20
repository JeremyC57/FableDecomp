// CTCTavernGameCardBase::CardsFinishedMoving() -- for every card: play the flip sound and start a half turn (0 <-> 0.5) towards the other face, then go to ETGS_CARDS_TURNING_UP
// Leaf: ?CardsFinishedMoving@CTCTavernGameCardBase@@UAEXXZ
// Retail: Fable.exe @ 0x008F09E0, 136 bytes; CTCTavernGameCardBase vtable 0x01288294 slot 53 (engine/CTCTavernGameCardBase.h; inherited by
//         CTCTavernGameCardSorting 0x0126F5B4 slot 53, overridden by CTCTavernGameCardPairs 0x007D6260).
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0, CTCTavernGame::PlaySoundW(const CCharString&) 0x005EA950,
//          CCharString::~CCharString() 0x0099EAE0, CTCTavernGame::SetState(ETavernGameStates) 0x005E9B20. String
//          0x0126F6C4 "TAVERN_GAME_CARD_FLIP".
// Layout: retail CTCTavernGameCardBase::Cards (vector<CCard*>, 12 bytes) +0x128 (PDB +0x140); CCard: InitialRotation +0x0c, FinalRotation
//         +0x10, TurnFraction +0x18, Turning +0x24, FaceDown +0x25.
// Shape: the sound name is a named local (an unnamed temporary reuses the ctor's eax); `cmp cl,1 ; jne` is the explicit `== true` test of the bool; the zero and 0.5 live in ebx/ebp across the loop.
// Naming (NF159, 2026-11-20, q4271 / q4273): ported onto engine/CTCTavernGameCardBase.h (retail branch) from local CTCTavernGameCardBase / CCard /
//   CCharString stand-ins. Own name: Ego_r.pdb 0x009161D0 / FableWin.pdb 0x02AF2330 ?CardsFinishedMoving@CTCTavernGameCardBase@@UAEXXZ, the
//   header's slot 53 (was the descriptive TurnAllCards, non-virtual). Callees: ?PlaySoundW@CTCTavernGame@@IAEKABVCCharString@@@Z (protected; was
//   ?PlaySound@CTCTavernGameCardBase@@QAEKABVCCharString@@@Z) and ?SetState@CTCTavernGame@@QAEXW4ETavernGameStates@1@@Z (was
//   ?SetState@CTCTavernGameCardBase@@QAEXJ@Z); 22 is ETGS_CARDS_TURNING_UP.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CRIGHTHANDEDSET
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CTCTavernGameCardBase.h"

void CTCTavernGameCardBase::CardsFinishedMoving()
{
    for (std::vector<CCard*>::iterator it = Cards.begin(); it != Cards.end(); ++it) {
        {
            CCharString sound("TAVERN_GAME_CARD_FLIP");
            PlaySoundW(sound);
        }
        CCard* card = *it;
        card->Turning = true;
        card->TurnFraction = 0.0f;
        if (card->FaceDown == true) {
            card->InitialRotation = 0.5f;
            card->FinalRotation = 0.0f;
        } else {
            card->InitialRotation = 0.0f;
            card->FinalRotation = 0.5f;
        }
        card->FaceDown = !card->FaceDown;
    }
    SetState(ETGS_CARDS_TURNING_UP);
}
