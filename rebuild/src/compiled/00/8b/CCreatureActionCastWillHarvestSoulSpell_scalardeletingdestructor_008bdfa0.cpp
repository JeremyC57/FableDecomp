struct CCreatureAction_CastWillHarvestSoulSpell {
    void* vftable;
};

void __fastcall CCreatureAction_CastWillHarvestSoulSpell_dtor(CCreatureAction_CastWillHarvestSoulSpell* action);
void operator delete(void*);

void* __fastcall CCreatureAction_CastWillHarvestSoulSpell_scalar_deleting_destructor(CCreatureAction_CastWillHarvestSoulSpell* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_CastWillHarvestSoulSpell_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}