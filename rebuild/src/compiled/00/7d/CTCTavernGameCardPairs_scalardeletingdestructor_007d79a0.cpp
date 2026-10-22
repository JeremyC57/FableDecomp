struct CTCTavernGameCardPairs {
    void* vftable;
};

void __fastcall CTCTavernGameCardPairs_dtor(CTCTavernGameCardPairs* pairs);
void operator delete(void*);

void* __fastcall CTCTavernGameCardPairs_scalar_deleting_destructor(CTCTavernGameCardPairs* pairs, int edx_unused, unsigned int flags)
{
    CTCTavernGameCardPairs_dtor(pairs);
    if (flags & 1)
        operator delete(pairs);
    return pairs;
}