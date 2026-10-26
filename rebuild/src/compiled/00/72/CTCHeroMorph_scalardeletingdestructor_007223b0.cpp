class CTCHeroMorph { public:
    void* vftable;
};

void __fastcall CTCHeroMorph_dtor(CTCHeroMorph* morph);
void operator delete(void*);

void* __fastcall CTCHeroMorph_scalar_deleting_destructor(CTCHeroMorph* morph, int edx_unused, unsigned int flags)
{
    CTCHeroMorph_dtor(morph);
    if (flags & 1)
        operator delete(morph);
    return morph;
}