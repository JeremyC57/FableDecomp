struct CCreatureAction_ActivateZTarget {
    void* vftable;
};

void __fastcall CCreatureAction_ActivateZTarget_dtor(CCreatureAction_ActivateZTarget* action);
void operator delete(void*);

void* __fastcall CCreatureAction_ActivateZTarget_scalar_deleting_destructor(CCreatureAction_ActivateZTarget* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_ActivateZTarget_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}