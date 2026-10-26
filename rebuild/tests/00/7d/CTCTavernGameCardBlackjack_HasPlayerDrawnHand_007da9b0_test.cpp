// behaviour test for 007da9b0 CTCTavernGameCardBlackjack::HasPlayerDrawnHand (self-contained copy of the model + body)
#include <cstdio>
#include <cstring>
class CCard { public:
    void*         Thing;                     // +0x00 CThingObject*
    long          Name;                      // +0x04
    long          Value;                     // +0x08
    unsigned char _pad_0x0c[0x18];
    bool          Turning;                   // +0x24
    bool          FaceDown;                  // +0x25
};

template<class T> struct CRetailVector {     // retail STLport vector: start, finish, end of storage (12 bytes)
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
};

struct CTCTavernGameCardBlackjack {
    unsigned char               _pad_0x000[0x318];
    CRetailVector<CCard*>       DealersCards;          // +0x318
    unsigned char               PlayersCards[0xc];     // +0x324 vector<vector<CCard*> >
    unsigned char               Bets[0xc];             // +0x330 vector<long>
    CRetailVector<CCard*>*      CurrentPlayerCards;    // +0x33c
    CRetailVector<CCard*>*      CurrentDealingCards;   // +0x340
    CCard*                      NewlyDealtCard;        // +0x344
    float                       MostMoneyWon;          // +0x348
    float                       MoneyWonThisSitting;   // +0x34c
    long                        Answer;                // +0x350
    long                        CurrentPlayerCardsIndex;   // +0x354
    long                        CurrentDealingCardsIndex;  // +0x358
    long                        CurrentPackCardIndex;      // +0x35c
    float                       CurrentBet;            // +0x360
    long                        WinState;              // +0x364 EBlackjackWinStates
    bool                        HasPlayerSplit;        // +0x368

    long GetHandValue(CRetailVector<CCard*>* cards, bool includeFaceDown)   // 0x007DA230 out of line
    {
        long total = 0;
        long aces = 0;
        for (CCard** it = cards->_M_start; it != cards->_M_finish; ++it) {
            if (includeFaceDown || (*it)->Turning || !(*it)->FaceDown) {
                long value = (*it)->Value;
                if (value == 11)
                    ++aces;
                total += value;
            }
        }
        while (total > 21 && aces > 0) {
            total -= 10;
            --aces;
        }
        return total;
    }
    bool HasPlayerDrawnHand();
};

bool CTCTavernGameCardBlackjack::HasPlayerDrawnHand()
{
    long dealer = GetHandValue(&DealersCards, true);
    long player = GetHandValue(CurrentPlayerCards, true);
    if (dealer == 21 && DealersCards.size() == 2)
        return false;
    return dealer == player;
}

static CCard g_cards[16];
static CCard* g_dealer[8];
static CCard* g_player[8];
static CTCTavernGameCardBlackjack g_game;
static CRetailVector<CCard*> g_hand;

static bool Run(const long* d, int nd, const long* p, int np, bool split)
{
    std::memset(&g_game, 0xcd, sizeof(g_game));
    int c = 0;
    for (int i = 0; i < nd; ++i) { std::memset(&g_cards[c], 0, sizeof(CCard)); g_cards[c].Value = d[i]; g_cards[c].FaceDown = (i == 0); g_dealer[i] = &g_cards[c++]; }
    for (int i = 0; i < np; ++i) { std::memset(&g_cards[c], 0, sizeof(CCard)); g_cards[c].Value = p[i]; g_player[i] = &g_cards[c++]; }
    g_game.DealersCards._M_start = g_dealer; g_game.DealersCards._M_finish = g_dealer + nd; g_game.DealersCards._M_end_of_storage = g_dealer + 8;
    g_hand._M_start = g_player; g_hand._M_finish = g_player + np; g_hand._M_end_of_storage = g_player + 8;
    g_game.CurrentPlayerCards = &g_hand;
    g_game.HasPlayerSplit = split;
    return g_game.HasPlayerDrawnHand();
}
static int Fail(int i) { std::printf("007da9b0_TEST FAIL (case %d)\n", i); return 1; }

int main()
{
    { long d[] = {10,9}; long p[] = {10,9}; if (Run(d, 2, p, 2, false) != true) return Fail(0); }
    { long d[] = {10,9}; long p[] = {10,8}; if (Run(d, 2, p, 2, false) != false) return Fail(1); }
    { long d[] = {11,10}; long p[] = {11,10}; if (Run(d, 2, p, 2, false) != false) return Fail(2); }
    { long d[] = {5,6,10}; long p[] = {11,10}; if (Run(d, 3, p, 2, false) != true) return Fail(3); }
    { long d[] = {11,11,9}; long p[] = {10,11}; if (Run(d, 3, p, 2, false) != true) return Fail(4); }
    { long d[] = {10,13}; long p[] = {10,13}; if (Run(d, 2, p, 2, false) != true) return Fail(5); }
    std::printf("007da9b0_TEST PASS\n");
    return 0;
}