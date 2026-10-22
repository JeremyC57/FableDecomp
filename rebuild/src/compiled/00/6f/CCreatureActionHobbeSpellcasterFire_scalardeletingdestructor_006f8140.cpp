struct CCreatureAction_HobbeSpellcasterFire {
    void* vftable;
};

void __fastcall CCreatureAction_HobbeSpellcasterFire_dtor(CCreatureAction_HobbeSpellcasterFire* action);
void operator delete(void*);

void* __fastcall CCreatureAction_HobbeSpellcasterFire_scalar_deleting_destructor(CCreatureAction_HobbeSpellcasterFire* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_HobbeSpellcasterFire_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}