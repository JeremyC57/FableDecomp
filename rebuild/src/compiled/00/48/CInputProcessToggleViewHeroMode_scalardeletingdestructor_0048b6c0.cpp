// CInputProcessToggleViewHeroMode::`vector_deleting_destructor'
// Retail 0x0048b6c0, __fastcall, returns this pointer.

// operator delete @ 0xbfe9bc
extern "C" void __cdecl eng_operator_delete(void* p);

struct CInputProcessToggleViewHeroMode
{
    int dummy;
    void lbp_scalar_dtor();                          // @ 0xa0d3d0, this in ecx
    void* scalar_deleting_destructor(unsigned int flags);
};

void* CInputProcessToggleViewHeroMode::scalar_deleting_destructor(unsigned int flags)
{
    this->lbp_scalar_dtor();
    if (flags & 1)
        eng_operator_delete(this);
    return this;
}