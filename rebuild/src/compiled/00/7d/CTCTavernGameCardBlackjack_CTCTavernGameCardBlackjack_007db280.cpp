// CTCTavernGameCardBlackjack::CTCTavernGameCardBlackjack(CThing* owner) -- constructor: card-base constructor, own vtable, then three empty card vectors (+0x318, +0x324, +0x330)
// Retail: Fable.exe @ 0x007DB280, 81 bytes (`ret 4`; vtable 0x0126F8F4).
// Callees: CTCTavernGameCardBase::CTCTavernGameCardBase(owner) 0x008F1CD0.
// Shape: the vtable store comes BEFORE the nine register-zero stores, so the zeroed words are member
//        objects with inline constructors (GOTCHAS wave 24i): three 12-byte vectors {first, last,
//        end-of-storage}. Init-list scalars would be scheduled before the vtable store. +0x33C (the
//        hand being scored, see HasGoneBust 0x007DA8B0) is left uninitialised.
class CThing;
struct CTavernCard;

struct CCardVector {                    // retail 12-byte vector<CTavernCard*>
    CTavernCard** m_first;
    CTavernCard** m_last;
    CTavernCard** m_endOfStorage;
    CCardVector() : m_first(0), m_last(0), m_endOfStorage(0) {}
};

struct CTCTavernGameCardBase {
    CTCTavernGameCardBase(CThing* owner);       // 0x008F1CD0
    virtual void v00();
    unsigned char _pad_0x04[0x314];
};

struct CTCTavernGameCardBlackjack : CTCTavernGameCardBase {
    CCardVector m_playerCards;          // +0x318
    CCardVector m_dealerCards;          // +0x324
    CCardVector m_deck;                 // +0x330
    CCardVector* m_pCurrentHand;        // +0x33c
    CTCTavernGameCardBlackjack(CThing* owner);
    virtual void v00();
};

CTCTavernGameCardBlackjack::CTCTavernGameCardBlackjack(CThing* owner)
    : CTCTavernGameCardBase(owner)
{
}