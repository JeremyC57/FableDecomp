struct CCreatureAction_FinishWillForceSpell {
    void* vftable;
};

void __fastcall CCreatureAction_FinishWillForceSpell_dtor(CCreatureAction_FinishWillForceSpell* action);
void operator delete(void*);

void* __fastcall CCreatureAction_FinishWillForceSpell_scalar_deleting_destructor(CCreatureAction_FinishWillForceSpell* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_FinishWillForceSpell_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}