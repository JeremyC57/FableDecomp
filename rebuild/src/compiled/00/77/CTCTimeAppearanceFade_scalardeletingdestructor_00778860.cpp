struct CTCTimeAppearanceFade {
    void* vftable;
};

void __fastcall CTCTimeAppearanceFade_dtor(CTCTimeAppearanceFade* fade);
void operator delete(void*);

void* __fastcall CTCTimeAppearanceFade_scalar_deleting_destructor(CTCTimeAppearanceFade* fade, int edx_unused, unsigned int flags)
{
    CTCTimeAppearanceFade_dtor(fade);
    if (flags & 1)
        operator delete(fade);
    return fade;
}