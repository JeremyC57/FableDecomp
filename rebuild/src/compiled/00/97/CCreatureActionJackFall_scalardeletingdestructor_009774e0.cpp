struct CCreatureAction_JackFall {
    void* vftable;
};

void __fastcall CCreatureAction_JackFall_dtor(CCreatureAction_JackFall* action);
void operator delete(void*);

void* __fastcall CCreatureAction_JackFall_scalar_deleting_destructor(CCreatureAction_JackFall* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_JackFall_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}