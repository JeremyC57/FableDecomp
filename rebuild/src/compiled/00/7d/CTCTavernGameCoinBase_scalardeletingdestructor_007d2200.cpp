struct CTCTavernGameCoinBase {
    void* vftable;
};

void __fastcall CTCTavernGameCoinBase_dtor(CTCTavernGameCoinBase* coin);
void operator delete(void*);

void* __fastcall CTCTavernGameCoinBase_scalar_deleting_destructor(CTCTavernGameCoinBase* coin, int edx_unused, unsigned int flags)
{
    CTCTavernGameCoinBase_dtor(coin);
    if (flags & 1)
        operator delete(coin);
    return coin;
}