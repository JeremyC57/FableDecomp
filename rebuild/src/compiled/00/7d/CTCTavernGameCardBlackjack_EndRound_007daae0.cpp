// CTCTavernGameCardBlackjack::EndRound() -- base card-game EndRound, clear the dealer's cards and the bets, reset the win state, switch the game to state 8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DAAE0, length 117 bytes (+11 int3 padding). Vtable slot 33 of
// Leaf: EndRound
// CTCTavernGameCardBlackjack (0x0126F8F4); the base's slot 33 is 0x008F1720.
// Label: the manifest says CSwappingStateComponent::Initialise (propagated); rtti-vtables.tsv puts the body in
// CTCTavernGameCardBlackjack slot 33, the same slot as CTCTavernGameCardBase "EndRound" 0x008F1720 that it calls first.
// Callees: CTCTavernGameCardBase::EndRound @ 0x008F1720 (qualified call), memmove @ 0x00BFEAE6, CTCTavernGame::SetState
// @ 0x005E9B20 (non-virtual, state 8).
// Shape: each clear() is erase(begin(), end()) = `finish = CopyDown(end, finish, begin)` with the copy-down written as an
// early-out (`if (src == srcEnd) return dest;`), the byte count unscaled (char-pointer difference), as in the landed byte
// vector Clear 0x00567AC0: both `end` and `finish` are the same load, giving retail's `cmp eax,eax ; jne`.
// Model: the landed CTCTavernGameCardBlackjack ctor 0x007DB280 (base 0x318 bytes, three 12-byte vectors at +0x318/+0x324/
// +0x330); PDB names DealersCards, PlayersCards, Bets; +0x364 is WinState (PDB +0x390 less the 16->12 byte vector and
// base-size shrink).
#include <string.h>
class CThing;
class CCard;

template <class T> struct CRetailVector {      // retail 12-byte vector {first, last, end-of-storage}
    T* m_first;
    T* m_last;
    T* m_endOfStorage;
    static T* CopyDown(T* src, T* srcEnd, T* dest)
    {
        if (src == srcEnd)
            return dest;
        unsigned long count = (const char*)srcEnd - (const char*)src;
        return (T*)((char*)memmove(dest, src, count) + count);
    }
    void erase(T* eraseFirst, T* eraseLast) { m_last = CopyDown(eraseLast, m_last, eraseFirst); }
    void clear() { erase(m_first, m_last); }
};

class CTCTavernGame { public:                          // polymorphic root (vptr at +0)
    virtual void v00();
    void SetState(long state);                  // 0x005E9B20
};
struct CTCTavernGameCardBase : CTCTavernGame {
    unsigned char _pad_0x04[0x314];
public:
    virtual void EndRound();   // 0x008F1720
};

struct CTCTavernGameCardBlackjack : CTCTavernGameCardBase {
    CRetailVector<CCard*> DealersCards;                 // +0x318
    CRetailVector<CRetailVector<CCard*> > PlayersCards; // +0x324
    CRetailVector<long> Bets;                           // +0x330
    CRetailVector<CCard*>* CurrentPlayerCards;          // +0x33c
    CRetailVector<CCard*>* CurrentDealingCards;         // +0x340
    CCard* NewlyDealtCard;                              // +0x344
    float MostMoneyWon;                                 // +0x348
    float MoneyWonThisSitting;                          // +0x34c
    long Answer;                                        // +0x350
    long CurrentPlayerCardsIndex;                       // +0x354
    long CurrentDealingCardsIndex;                      // +0x358
    long CurrentPackCardIndex;                          // +0x35c
    float CurrentBet;                                   // +0x360
    long WinState;                                      // +0x364 EBlackjackWinStates
    void EndRound();
};

void CTCTavernGameCardBlackjack::EndRound()
{
    CTCTavernGameCardBase::EndRound();
    DealersCards.clear();
    Bets.clear();
    WinState = 0;
    SetState(8);
}