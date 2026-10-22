struct CCreatureAction_BuildUpUnholyPowerSpell {
    void* vftable;
};

void __fastcall CCreatureAction_BuildUpUnholyPowerSpell_dtor(CCreatureAction_BuildUpUnholyPowerSpell* action);
void operator delete(void*);

void* __fastcall CCreatureAction_BuildUpUnholyPowerSpell_scalar_deleting_destructor(CCreatureAction_BuildUpUnholyPowerSpell* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_BuildUpUnholyPowerSpell_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}