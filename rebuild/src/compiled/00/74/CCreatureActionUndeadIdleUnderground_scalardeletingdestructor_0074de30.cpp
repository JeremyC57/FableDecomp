struct CCreatureAction_UndeadIdleUnderground {
    void* vftable;
};

void __fastcall CCreatureAction_UndeadIdleUnderground_dtor(CCreatureAction_UndeadIdleUnderground* action);
void operator delete(void*);

void* __fastcall CCreatureAction_UndeadIdleUnderground_scalar_deleting_destructor(CCreatureAction_UndeadIdleUnderground* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_UndeadIdleUnderground_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}