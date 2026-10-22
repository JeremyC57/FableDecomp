struct CCreatureAction_StartBlocking {
    void* vftable;
};

void __fastcall CCreatureAction_StartBlocking_dtor(CCreatureAction_StartBlocking* action);
void operator delete(void*);

void* __fastcall CCreatureAction_StartBlocking_scalar_deleting_destructor(CCreatureAction_StartBlocking* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_StartBlocking_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}