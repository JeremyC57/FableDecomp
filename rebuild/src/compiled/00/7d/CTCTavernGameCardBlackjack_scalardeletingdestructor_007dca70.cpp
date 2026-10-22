struct CTCTavernGameCardBlackjack {
    void* vftable;
};

void __fastcall CTCTavernGameCardBlackjack_dtor(CTCTavernGameCardBlackjack* blackjack);
void operator delete(void*);

void* __fastcall CTCTavernGameCardBlackjack_scalar_deleting_destructor(CTCTavernGameCardBlackjack* blackjack, int edx_unused, unsigned int flags)
{
    CTCTavernGameCardBlackjack_dtor(blackjack);
    if (flags & 1)
        operator delete(blackjack);
    return blackjack;
}