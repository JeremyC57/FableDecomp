// Behaviour test for 007da290: a self-contained copy of the candidate's model and body, run over stub callees.
// CTCTavernGameCardBlackjack::TurnPlayerCards() -- flips the face-up cards of the current hand face down (with the flip sound), then moves on to the next player hand, or ends the player's round after the last one
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#define __declspec(x)
#endif
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CCard {
    void*         Thing;                     // +0x00
    long          Name;                      // +0x04
    long          Value;                     // +0x08
    unsigned char _pad_0x0c[0x18];
    bool          Turning;                   // +0x24
    bool          FaceDown;                  // +0x25
    void Turn();                             // 0x008F0620
};
template<class T> struct CVector {              // STLport vector<T> (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    T* end() const { return _M_finish; }
    unsigned int size() const { return end() - begin(); }
    T& operator[](unsigned int n) const { return *(begin() + n); }
};
struct CTCTavernGameCardBlackjack {
    void* __vfptr;                              // +0x00 (CTCBase)
    unsigned char _base04[0x318 - 0x04];        // CTCTavernGameCardBase (retail 0x318)
    CVector<CCard*> DealersCards;               // +0x318
    CVector< CVector<CCard*> > PlayersCards;    // +0x324
    CVector<long> Bets;                         // +0x330
    CVector<CCard*>* CurrentPlayerCards;        // +0x33c
    CVector<CCard*>* CurrentDealingCards;       // +0x340
    CCard* NewlyDealtCard;                      // +0x344
    float MostMoneyWon;                         // +0x348
    float MoneyWonThisSitting;                  // +0x34c
    long  Answer;                               // +0x350
    long  CurrentPlayerCardsIndex;              // +0x354
    long  CurrentDealingCardsIndex;             // +0x358
    long  CurrentPackCardIndex;                 // +0x35c
    float CurrentBet;                           // +0x360
    long  WinState;                             // +0x364
    bool  HasPlayerSplit;                       // +0x368
    bool  HasPlayerFinishedRound;               // +0x369
    unsigned long PlaySound(const CCharString& name);   // 0x005EA950 (CTCTavernGame)
    void SetState(long state);                  // 0x005E9B20 (CTCTavernGame)
    void TurnPlayerCards();
};

void CTCTavernGameCardBlackjack::TurnPlayerCards()
{
    for (CCard** it = CurrentPlayerCards->begin(); it != CurrentPlayerCards->end(); ++it) {
        if (!(*it)->FaceDown) {
            {
                CCharString sound("TAVERN_GAME_CARD_FLIP");
                PlaySound(sound);
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
    SetState(0x16);
}

// ---- stubs and driver ----
static int g_sounds, g_turns, g_state;
CCharString::CCharString(const char* text, long) { m_pData = (void*)text; }
CCharString::~CCharString() {}
unsigned long CTCTavernGameCardBlackjack::PlaySound(const CCharString& name)
{ if (std::strcmp((const char*)name.m_pData, "TAVERN_GAME_CARD_FLIP") == 0) ++g_sounds; return 0; }
void CTCTavernGameCardBlackjack::SetState(long state) { g_state = state; }
void CCard::Turn() { ++g_turns; FaceDown = !FaceDown; }

static CCard g_cards[4];
static CCard* g_hand0[3];
static CCard* g_hand1[1];
static CVector<CCard*> g_hands[2];
static CTCTavernGameCardBlackjack g_game;

int main()
{
    std::memset(g_cards, 0, sizeof(g_cards));
    g_cards[1].FaceDown = true;
    g_hand0[0] = &g_cards[0]; g_hand0[1] = &g_cards[1]; g_hand0[2] = &g_cards[2];
    g_hand1[0] = &g_cards[3];
    g_hands[0]._M_start = g_hand0; g_hands[0]._M_finish = g_hand0 + 3; g_hands[0]._M_end_of_storage = g_hand0 + 3;
    g_hands[1]._M_start = g_hand1; g_hands[1]._M_finish = g_hand1 + 1; g_hands[1]._M_end_of_storage = g_hand1 + 1;
    g_game.PlayersCards._M_start = g_hands; g_game.PlayersCards._M_finish = g_hands + 2; g_game.PlayersCards._M_end_of_storage = g_hands + 2;
    g_game.CurrentPlayerCards = &g_hands[0];
    g_game.CurrentPlayerCardsIndex = 0;
    g_game.HasPlayerFinishedRound = false;
    g_state = -1;
    g_game.TurnPlayerCards();
    if (g_turns != 2 || g_sounds != 2 || !g_cards[0].FaceDown || !g_cards[1].FaceDown || !g_cards[2].FaceDown
        || g_game.CurrentPlayerCards != &g_hands[1] || g_game.CurrentPlayerCardsIndex != 1 || g_game.HasPlayerFinishedRound
        || g_state != 0x16) { std::printf("007da290_TEST FAIL (first hand)\n"); return 1; }
    g_state = -1;
    g_game.TurnPlayerCards();
    if (g_turns != 3 || g_sounds != 3 || !g_cards[3].FaceDown || g_game.CurrentPlayerCards != 0
        || g_game.CurrentPlayerCardsIndex != 0 || !g_game.HasPlayerFinishedRound || g_state != 0x16)
        { std::printf("007da290_TEST FAIL (last hand)\n"); return 1; }
    std::printf("007da290_TEST PASS\n");
    return 0;
}
