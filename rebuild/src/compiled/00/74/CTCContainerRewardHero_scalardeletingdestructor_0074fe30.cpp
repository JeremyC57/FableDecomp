struct CTCContainerRewardHero {
    void* vftable;
};

void __fastcall CTCContainerRewardHero_dtor(CTCContainerRewardHero* hero);
void operator delete(void*);

void* __fastcall CTCContainerRewardHero_scalar_deleting_destructor(CTCContainerRewardHero* hero, int edx_unused, unsigned int flags)
{
    CTCContainerRewardHero_dtor(hero);
    if (flags & 1)
        operator delete(hero);
    return hero;
}