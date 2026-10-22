struct CTCTrophy {
    void* vftable;
};

void __fastcall CTCTrophy_dtor(CTCTrophy* trophy);
void operator delete(void*);

void* __fastcall CTCTrophy_scalar_deleting_destructor(CTCTrophy* trophy, int edx_unused, unsigned int flags)
{
    CTCTrophy_dtor(trophy);
    if (flags & 1)
        operator delete(trophy);
    return trophy;
}