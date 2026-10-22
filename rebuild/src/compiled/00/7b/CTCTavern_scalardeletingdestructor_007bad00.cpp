struct CTCTavern {
    void* vftable;
};

void __fastcall CTCTavern_dtor(CTCTavern* tavern);
void operator delete(void*);

void* __fastcall CTCTavern_scalar_deleting_destructor(CTCTavern* tavern, int edx_unused, unsigned int flags)
{
    CTCTavern_dtor(tavern);
    if (flags & 1)
        operator delete(tavern);
    return tavern;
}