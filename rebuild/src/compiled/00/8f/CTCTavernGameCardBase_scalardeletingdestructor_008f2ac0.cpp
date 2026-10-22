struct CTCTavernGameCardBase {
    void* vftable;
};

void __fastcall CTCTavernGameCardBase_dtor(CTCTavernGameCardBase* card);
void operator delete(void*);

void* __fastcall CTCTavernGameCardBase_scalar_deleting_destructor(CTCTavernGameCardBase* card, int edx_unused, unsigned int flags)
{
    CTCTavernGameCardBase_dtor(card);
    if (flags & 1)
        operator delete(card);
    return card;
}