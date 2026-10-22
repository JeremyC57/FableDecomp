struct CCreatureAction_CastWillHealLifeSpell {
    void* vftable;
};

void __fastcall CCreatureAction_CastWillHealLifeSpell_dtor(CCreatureAction_CastWillHealLifeSpell* action);
void operator delete(void*);

void* __fastcall CCreatureAction_CastWillHealLifeSpell_scalar_deleting_destructor(CCreatureAction_CastWillHealLifeSpell* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_CastWillHealLifeSpell_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}