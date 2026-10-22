struct CCreatureAction_BuildUpWillSummonSpell {
    void* vftable;
};

void __fastcall CCreatureAction_BuildUpWillSummonSpell_dtor(CCreatureAction_BuildUpWillSummonSpell* action);
void operator delete(void*);

void* __fastcall CCreatureAction_BuildUpWillSummonSpell_scalar_deleting_destructor(CCreatureAction_BuildUpWillSummonSpell* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_BuildUpWillSummonSpell_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}