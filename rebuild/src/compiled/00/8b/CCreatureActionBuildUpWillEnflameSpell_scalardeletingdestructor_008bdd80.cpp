struct CCreatureAction_BuildUpWillEnflameSpell {
    void* vftable;
};

void __fastcall CCreatureAction_BuildUpWillEnflameSpell_dtor(CCreatureAction_BuildUpWillEnflameSpell* action);
void operator delete(void*);

void* __fastcall CCreatureAction_BuildUpWillEnflameSpell_scalar_deleting_destructor(CCreatureAction_BuildUpWillEnflameSpell* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_BuildUpWillEnflameSpell_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}