class CTCHero { public:
    void* vftable;
};

void __fastcall CTCHero_dtor(CTCHero* hero);
void operator delete(void*);

void* __fastcall CTCHero_scalar_deleting_destructor(CTCHero* hero, int edx_unused, unsigned int flags)
{
    CTCHero_dtor(hero);
    if (flags & 1)
        operator delete(hero);
    return hero;
}