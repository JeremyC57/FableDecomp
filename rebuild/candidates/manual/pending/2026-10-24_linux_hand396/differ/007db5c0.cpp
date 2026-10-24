// Grade: DIFFER(237v224) -- retail emits no destructor for the empty-hand vector passed to PlayersCards.insert (and so tests the index with `test ecx,ecx` and stores the empty vector with immediates); ours always re-tests and frees the temporary after the call (temporary, const local, or block-scoped const local), which shifts the register allocation (extra ebx, &Bets kept live)
// CTCTavernGameCardBlackjack::Split() -- split the current hand: insert a new empty hand before it, make it the current hand, move the old hand's first card into it, take a second base bet from the money and record it beside the hand's bet, then reorder the cards on the table
// Leaf: ?Split@CTCTavernGameCardBlackjack
// Retail: Fable.exe (TLC, VC7.1) @ 0x007DB5C0, 224 bytes, thiscall, plain `ret`.
// Identity: Ego_r.pdb name set ?Split@CTCTavernGameCardBlackjack@@QAEXXZ (donor 0x007D8330). The catalog label
//   "CTCTavernGameCardBlackjack_DealCards" names the wrong method (DealCards is the virtual at another address).
// Callees: vector<vector<CCard*> >::insert(iterator, const vector<CCard*>&) @ 0x007DCB10, vector<CCard*>::
//   _M_insert_overflow(CCard**, CCard* const&, const __true_type&, size_type, bool) @ 0x007D5D60 (as named by the
//   landed NewRound 0x007D70E0), memmove @ 0x00BFEAE6 (the inline erase), vector<long>::insert(iterator, const long&)
//   @ 0x007DC750, CTCTavernGameCardBlackjack::ReorderCards() @ 0x007DA810 (landed).
// Layout (retail, the landed CanSplit 0x007DA1A0 model; 12-byte vectors): CTCTavernGame BaseBet +0x34, CurrentMoney
//   +0x40; CTCTavernGameCardBlackjack PlayersCards +0x324, Bets +0x330, CurrentPlayerCards +0x33c,
//   CurrentPlayerCardsIndex +0x354, HasPlayerSplit +0x368.
// Shape: both iterators walk CurrentPlayerCardsIndex steps in one counted loop (VC7.1 folds it to `if (n > 0) { hand
//   += n; bet += n; }`); the empty hand is a value-initialised temporary vector; the moved card is pushed from the
//   following hand through the new current-hand pointer and erased from it (STLport erase: memmove of the tail).
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6
extern "C" void __cdecl free(void* block);                                          // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

struct CCard;
struct __true_type { __true_type() { } };

template <class T> struct CRetailVector {               // STLport vector (12 bytes)
    typedef T* iterator;
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CRetailVector()
    {
        if (_M_start)
            free(_M_start);
    }
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    T& operator[](unsigned int n) { return *(begin() + n); }
    iterator insert(iterator position, const T& x);     // 0x007DCB10 (vector<CCard*>), 0x007DC750 (long)
    void _M_insert_overflow(T* position, const T& x, const __true_type&, unsigned int fill_len, bool atend);   // 0x007D5D60
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            new (_M_finish) T(x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
    static void* __copy_trivial(const void* first, const void* last, void* result)
    {
        return (last == first) ? result
            : (char*)memmove(result, first, (const char*)last - (const char*)first) + ((const char*)last - (const char*)first);
    }
    iterator erase(iterator position)
    {
        if (position + 1 != end())
            __copy_trivial(position + 1, _M_finish, position);
        --_M_finish;
        return position;
    }
};

class CTCTavernGameCardBlackjack {
public:
    void*         __vftable;                                // +0x00
    unsigned char _pad_0x04[0x30];
    long          BaseBet;                                  // +0x34
    unsigned char _pad_0x38[0x8];
    long          CurrentMoney;                             // +0x40
    unsigned char _pad_0x44[0x324 - 0x44];
    CRetailVector<CRetailVector<CCard*> > PlayersCards;     // +0x324
    CRetailVector<long>   Bets;                             // +0x330
    CRetailVector<CCard*>* CurrentPlayerCards;              // +0x33c
    unsigned char _pad_0x340[0x354 - 0x340];
    long          CurrentPlayerCardsIndex;                  // +0x354
    unsigned char _pad_0x358[0x368 - 0x358];
    bool          HasPlayerSplit;                           // +0x368
    void ReorderCards();                                    // 0x007DA810
    void Split();
};

void CTCTavernGameCardBlackjack::Split()
{
    HasPlayerSplit = true;
    CRetailVector<CRetailVector<CCard*> >::iterator hand = PlayersCards.begin();
    CRetailVector<long>::iterator bet = Bets.begin();
    for (long i = CurrentPlayerCardsIndex; i > 0; --i) {
        ++hand;
        ++bet;
    }
    {
        const CRetailVector<CCard*> emptyHand;
        PlayersCards.insert(hand, emptyHand);
    }
    CRetailVector<CCard*>* current = &PlayersCards[CurrentPlayerCardsIndex];
    CurrentPlayerCards = current;
    CRetailVector<CCard*>* next = current + 1;
    current->push_back((*next)[0]);
    next->erase(next->begin());
    CurrentMoney -= BaseBet;
    Bets.insert(bet, BaseBet);
    ReorderCards();
}
