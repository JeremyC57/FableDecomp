struct CCreatureAction_ReleaseWillSummonSpell {
    void* vftable;
};

void __fastcall CCreatureAction_ReleaseWillSummonSpell_dtor(CCreatureAction_ReleaseWillSummonSpell* action);
void operator delete(void*);

void* __fastcall CCreatureAction_ReleaseWillSummonSpell_scalar_deleting_destructor(CCreatureAction_ReleaseWillSummonSpell* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_ReleaseWillSummonSpell_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}