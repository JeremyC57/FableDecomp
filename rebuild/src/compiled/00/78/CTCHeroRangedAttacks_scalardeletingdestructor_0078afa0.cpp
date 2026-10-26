class CTCHeroRangedAttacks { public:
    void* vftable;
};

void __fastcall CTCHeroRangedAttacks_dtor(CTCHeroRangedAttacks* attacks);
void operator delete(void*);

void* __fastcall CTCHeroRangedAttacks_scalar_deleting_destructor(CTCHeroRangedAttacks* attacks, int edx_unused, unsigned int flags)
{
    CTCHeroRangedAttacks_dtor(attacks);
    if (flags & 1)
        operator delete(attacks);
    return attacks;
}