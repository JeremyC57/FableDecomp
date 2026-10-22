struct CCreatureAction_BeStunned {
    void* vftable;
};

void __fastcall CCreatureAction_BeStunned_dtor(CCreatureAction_BeStunned* action);
void operator delete(void*);

void* __fastcall CCreatureAction_BeStunned_scalar_deleting_destructor(CCreatureAction_BeStunned* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_BeStunned_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}