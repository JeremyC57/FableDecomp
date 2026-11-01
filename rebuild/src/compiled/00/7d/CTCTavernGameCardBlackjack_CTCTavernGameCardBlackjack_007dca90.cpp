// CTCTavernGameCardBlackjack::~CTCTavernGameCardBlackjack() -- the compiler-generated destructor: free the Bets block, destroy the PlayersCards vector of hands, free the DealersCards block, then run ~CTCTavernGameCardBase
// Leaf: ??1CTCTavernGameCardBlackjack@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007DCA90, 60 bytes, thiscall, tail `jmp ~CTCTavernGameCardBase`.
// Identity (proven): sole caller is the slot-0 `??_G` 0x007DCA70 of vtable 0x0126F8F4 (RTTI .?AVCTCTavernGameCardBlackjack@@ :
//   CTCTavernGameCardBase : CTCTavernGame : CTCBase, rtti_bases.tsv); Ego_r.pdb (0x007D9050) and FableWin.pdb (0x02756BD0)
//   spell ??1CTCTavernGameCardBlackjack@@UAE@XZ. No 0x0126F8F4 store: the destructor is the implicit one.
// Callees: free 0x00BFEA14, vector<vector<CCard*> >::~vector 0x007DC890 (FableWin 0x0275BD20 spells it; out of line here),
//   CTCTavernGameCardBase::~CTCTavernGameCardBase 0x007D5E80 (Ego_r 0x007D3200 / FableWin 0x02745B30; it frees the base's
//   Cards / Pack / ParticleEffects vectors).
// Layout (retail offsets from this body): the CTCTavernGameCardBase subobject 0x318 bytes, then the Ego_r.pdb members in
//   order with STLport 12-byte vectors: DealersCards +0x318, PlayersCards +0x324, Bets +0x330, the rest trivially destructible.
//   engine/CTCTavernGameCardBlackjack.h carries Ego_r (Dinkumware) offsets (header debt).
// Shape: members destroyed in reverse order: ~vector<long> and ~vector<CCard*> expand (trivial _Destroy + ~_Vector_base's
//   null-tested free); ~vector<vector<CCard*> > keeps its element loop and stays out of line. DestroyOne (a destruction by
//   name, ungraded) makes VC7.1 emit this out-of-line implicit `??1` (chunks 656 / 657 / 696 / 697).
extern "C" void __cdecl free(void* block);              // 0x00BFEA14

class CCard;
class CThing;

namespace std {

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    typedef _Alloc allocator_type;
    ~_Vector_base() {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
protected:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, allocator_type> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};

}   // namespace std

class CTCTavernGameCardBase {                            // retail 0x318 bytes (polymorphic, CTCTavernGame : CTCBase)
public:
    virtual ~CTCTavernGameCardBase();                    // 0x007D5E80
    unsigned char _base[0x314];
};

enum EBlackjackWinStates { BLACKJACK_WIN_STATE_NONE = 0 };

class CTCTavernGameCardBlackjack : public CTCTavernGameCardBase {   // vtable 0x0126F8F4
public:
    std::vector<CCard*> DealersCards;                    // +0x318
    std::vector<std::vector<CCard*> > PlayersCards;      // +0x324
    std::vector<long> Bets;                              // +0x330
    std::vector<CCard*>* CurrentPlayerCards;             // +0x33c
    std::vector<CCard*>* CurrentDealingCards;
    CCard* NewlyDealtCard;
    float MostMoneyWon;
    float MoneyWonThisSitting;
    long Answer;
    long CurrentPlayerCardsIndex;
    long CurrentDealingCardsIndex;
    long CurrentPackCardIndex;
    float CurrentBet;
    EBlackjackWinStates WinState;
    bool HasPlayerSplit;
    bool HasPlayerFinishedRound;
    bool HasDealerStartedTakingCards;
    long AmountWonThisRound;
    long NumHandsWonThisRound;
    bool Doubled;
};

// Emission helper: destroying the object by name is what makes VC7.1 emit the out-of-line implicit `??1` (chunk 656/657).
void DestroyOne(CTCTavernGameCardBlackjack* game)
{
    game->CTCTavernGameCardBlackjack::~CTCTavernGameCardBlackjack();
}