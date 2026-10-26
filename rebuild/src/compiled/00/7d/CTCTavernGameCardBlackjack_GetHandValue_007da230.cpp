// CTCTavernGameCardBlackjack::GetHandValue(vector<CCard*>* cards, bool includeFaceDown) -- blackjack hand value: sums the visible cards, then counts aces (11) down to 1 while the hand is bust
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DA230, length 84 bytes (ret 8). Ego_r.pdb
// `?GetHandValue@CTCTavernGameCardBlackjack@@QAEJPAV?$vector@PAVCCard@@V?$allocator@PAVCCard@@@std@@@std@@_N@Z`.
// No callees; `this` is not used.
// CCard (Ego_r.pdb): Value +0x08 (an ace is valued 11), Turning +0x24, FaceDown +0x25. A card counts when face-down cards
// are included, when it is turning over, or when it is face up.
// Shape: a pointer walk from first to last (both loaded once), `total` in eax and `aces` in edi zeroed before the empty
// test; the visibility test reads the flag byte register (bl) against the members; the bust correction is a
// `while (total > 21 && aces > 0)` loop after the walk (its entry test sits inside the walk's exit).
class CCard { public:
    void*         Thing;                     // +0x00 CThingObject*
    long          Name;                      // +0x04
    long          Value;                     // +0x08
    unsigned char _pad_0x0c[0x18];
    bool          Turning;                   // +0x24
    bool          FaceDown;                  // +0x25
};

template<class T> struct CRetailVector {     // retail std::vector: first, last, end
    T* first; T* last; T* end_of_storage;
};

struct CTCTavernGameCardBlackjack {
    long GetHandValue(CRetailVector<CCard*>* cards, bool includeFaceDown);
};

long CTCTavernGameCardBlackjack::GetHandValue(CRetailVector<CCard*>* cards, bool includeFaceDown)
{
    long total = 0;
    long aces = 0;
    for (CCard** it = cards->first; it != cards->last; ++it) {
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