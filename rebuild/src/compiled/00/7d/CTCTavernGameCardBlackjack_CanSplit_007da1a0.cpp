// CTCTavernGameCardBlackjack::CanSplit() -- the hand may be split when the hero can cover the base bet, has not split yet and holds exactly two cards of the same rank
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DA1A0, length 81 bytes. Ego_r.pdb `?CanSplit@CTCTavernGameCardBlackjack@@QAE_NXZ`.
// No callees.
// Layout (retail offsets): CTCTavernGame BaseBet +0x34, CurrentMoney +0x40 (PDB +0x3c/+0x48, the retail UIComponents vector is
// 8 bytes shorter); CTCTavernGameCardBlackjack CurrentPlayerCards (vector<CCard*>*) +0x33c, HasPlayerSplit +0x368 (see
// EnterGame 0x007DA020). CCard (Ego_r.pdb): Index +0x54 (0..51; the rank is Index % 13).
// Shape: ONE `if (a && b && c && d) return true; return false;` -- the early-return chain duplicates the false exits
// (89v81) and a nested if with a vector reference CSEs `first` (80v81); the single chain shares the `xor al,al` tail,
// pushes esi at entry and re-reads `[eax]` after the size test. `size() == 2` on a pointer vector is
// `(last - first) & ~3 == 8`; elements through the retail vector's iterator object (landed 0x004AF3C0 model); the second
// card's rank is computed first (`cdq ; idiv` by 13 into esi), `rank0 == rank1` keeps the first card's remainder on the left.
class CCard { public:
    unsigned char _pad_0x00[0x54];
    long          Index;                     // +0x54
};

template<class T> struct CRetailVector {     // retail std::vector: first, last, end (landed 0x004AF3C0 model)
    T* first; T* last; T* end_of_storage;
    struct iterator {
        T* p;
        explicit iterator(T* q) : p(q) {}
        iterator operator+(long n) const { return iterator(p + n); }
        T& operator*() const { return *p; }
    };
    iterator begin() { return iterator(first); }
    unsigned long size() const { return (unsigned long)(last - first); }
    T& operator[](unsigned long n) { return *(begin() + n); }
};

struct CTCTavernGameCardBlackjack {
    void*         __vftable;                 // +0x00
    unsigned char _pad_0x04[0x30];
    long          BaseBet;                   // +0x34
    unsigned char _pad_0x38[0x8];
    long          CurrentMoney;              // +0x40
    unsigned char _pad_0x44[0x2f8];
    CRetailVector<CCard*>* CurrentPlayerCards;   // +0x33c
    unsigned char _pad_0x340[0x28];
    bool          HasPlayerSplit;            // +0x368
    bool CanSplit();
};

bool CTCTavernGameCardBlackjack::CanSplit()
{
    if (CurrentMoney >= BaseBet && !HasPlayerSplit && CurrentPlayerCards->size() == 2
        && (*CurrentPlayerCards)[0]->Index % 13 == (*CurrentPlayerCards)[1]->Index % 13)
        return true;
    return false;
}