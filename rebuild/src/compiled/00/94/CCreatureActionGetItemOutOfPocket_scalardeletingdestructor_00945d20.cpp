struct CCreatureAction_GetItemOutOfPocket {
    void* vftable;
};

void __fastcall CCreatureAction_GetItemOutOfPocket_dtor(CCreatureAction_GetItemOutOfPocket* action);
void operator delete(void*);

void* __fastcall CCreatureAction_GetItemOutOfPocket_scalar_deleting_destructor(CCreatureAction_GetItemOutOfPocket* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_GetItemOutOfPocket_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}