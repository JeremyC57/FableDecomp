// CTCTavernGameCardPairs::CardsMainGame() -- the pairs main loop: flip back or clear a mismatched selected pair, run the timed-game clock, and once every card is face up and settled play the win/lose sound and show the round-over OK message (state 0x18)
// Leaf: ?CardsMainGame@CTCTavernGameCardPairs@@UAEXXZ
// Naming wave 18: the global stand-in g_FrameRate_01375550 (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x007D62D0, 656 bytes (+ int3 padding), plain `ret`.
// Callees: CCharString 0x0099EBF0 / ~CCharString 0x0099EAE0; CTCTavernGame::PlaySoundW 0x005EA950; CCard::Turn 0x008F0620;
//   CTCTavernGame::GetDef 0x005EA000 / GetText 0x005E9E70 / SetStateAfterOK 0x005EB080. Globals: the frame rate
//   0x01375550 (int), 1.0f 0x0122DED8, 0.0f 0x0122DEDC.
// Layout (retail): GameWon +0x21, NumRounds +0x24, GameFinished +0x28, TargetTime +0x98, GameStarted +0xa2, Cards +0x128,
//   Counter +0x154, TimerStopped +0x158, GameType +0x300, SelectedCard +0x308, AlternateSelectedCard +0x318; CCard Turning
//   +0x24, FaceDown +0x25; CTavernGameDef win/lose text +0xa4/+0xa8, body text +0xf8.
// Naming (NF159, 2026-11-20, q4273 / q4274 / q5079): ported onto engine/CTCTavernGameCardPairs.h (retail branch; the 0x007D5070 CardSorting
//   recipe) from local CTCTavernGameCardPairs / CCard / def / string stand-ins. Callees now bind the PDB spellings (Ego_r.pdb / FableWin.pdb):
//   ?PlaySoundW@CTCTavernGame@@IAEKABVCCharString@@@Z (was ?PlaySound@CTCTavernGameCardPairs@@QAEKABVCCharString@@@Z),
//   ?GetText@CTCTavernGame@@QAE?AVCWideString@@K@Z (was ...CardPairs...), ?GetDef@CTCTavernGame@@QAEXPAV?$CDefPointer@$$CBVCTavernGameDef@@@@@Z
//   (was ?GetTavernGameDef@CTCTavernGameCardPairs@@QAEXPAUCDefPointer@@@Z) and the protected
//   ?SetStateAfterOK@CTCTavernGame@@IAEXVCWideString@@0W4ETavernGameStates@1@@Z. Members: the local TimerStopped (+0x158) is
//   CTCTavernGameCardBase::RoundFinished, the def's +0xA4 / +0xA8 / +0xF8 texts WinPhrase / LosePhrase / Continue; 0x18 is ETGS_TURN_CARDS_BACK.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CTCTAVERNGAME_GETDEF_OUTOFLINE
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CTCTAVERNGAMECARDBASE_ECARDGAMETYPE_VALUES
#define FABLE_RETAIL_CTCTAVERNGAMECARDPAIRS_CARDSMAINGAME
#include "fable_game.h"
#include "engine/CTavernGameDef.h"
#include "engine/CTCTavernGameCardPairs.h"

void CTCTavernGameCardPairs::CardsMainGame()
{
    if (SelectedCard && AlternateSelectedCard && !SelectedCard->Turning && !AlternateSelectedCard->Turning) {
        if (SelectedCard->FaceDown && AlternateSelectedCard->FaceDown) {
            SelectedCard = 0;
            AlternateSelectedCard = 0;
        } else {
            if (!SelectedCard->FaceDown) {
                {
                    CCharString sound("TAVERN_GAME_CARD_FLIP");
                    PlaySoundW(sound);
                }
                SelectedCard->Turn();
            }
            if (!AlternateSelectedCard->FaceDown) {
                {
                    CCharString sound("TAVERN_GAME_CARD_FLIP");
                    PlaySoundW(sound);
                }
                AlternateSelectedCard->Turn();
            }
        }
    }
    bool stopped = RoundFinished;
    if (!stopped) {
        switch (GameType) {
        case ECGT_ROUND_LIMIT:
            if (GameStarted)
                Counter += 1.0f / (float)CMainGameComponent::GetConstantFPS();
            break;
        }
    }
    if (Counter < 0.0f)
        Counter = 0.0f;
    long type = GameType;
    switch (type) {
    case ECGT_ROUND_LIMIT:
        if (!stopped && Counter > TargetTime)
            RoundFinished = true;
        break;
    }
    if (!RoundFinished) {
        for (std::vector<CCard*>::iterator it = Cards.begin(); it != Cards.end(); ++it)
            if ((*it)->FaceDown)
                return;
    }
    RoundFinished = true;
    switch (type) {
    case ECGT_ROUND_LIMIT:
        if (NumRounds <= 1)
            GameFinished = true;
        GameWon = Counter <= TargetTime;
        break;
    }
    for (std::vector<CCard*>::iterator it = Cards.begin(); it != Cards.end(); ++it)
        if ((*it)->Turning == true)
            return;
    if (GameWon) {
        CCharString sound("TAVERN_GAME_WIN_GAME");
        PlaySoundW(sound);
    } else {
        CCharString sound("TAVERN_GAME_LOSE_GAME");
        PlaySoundW(sound);
    }
    CDefPointer<const CTavernGameDef> def(NULL_DEF);
    GetDef(&def);
    unsigned long text = GameWon ? def->WinPhrase : def->LosePhrase;
    SetStateAfterOK(GetText(text), GetText(def->Continue), ETGS_TURN_CARDS_BACK);
}
