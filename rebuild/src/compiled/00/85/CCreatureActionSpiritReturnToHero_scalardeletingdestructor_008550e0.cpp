struct CCreatureAction_SpiritReturnToHero {
    void* vftable;
};

void __fastcall CCreatureAction_SpiritReturnToHero_dtor(CCreatureAction_SpiritReturnToHero* action);
void operator delete(void*);

void* __fastcall CCreatureAction_SpiritReturnToHero_scalar_deleting_destructor(CCreatureAction_SpiritReturnToHero* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_SpiritReturnToHero_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}