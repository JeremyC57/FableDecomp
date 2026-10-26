class CTCCreatureOpinionOfHero { public:
    void* vftable;
};

void __fastcall CTCCreatureOpinionOfHero_dtor(CTCCreatureOpinionOfHero* hero);
void operator delete(void*);

void* __fastcall CTCCreatureOpinionOfHero_scalar_deleting_destructor(CTCCreatureOpinionOfHero* hero, int edx_unused, unsigned int flags)
{
    CTCCreatureOpinionOfHero_dtor(hero);
    if (flags & 1)
        operator delete(hero);
    return hero;
}