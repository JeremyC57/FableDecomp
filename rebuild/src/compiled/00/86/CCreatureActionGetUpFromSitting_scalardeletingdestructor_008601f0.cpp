struct CCreatureAction_GetUpFromSitting {
    void* vftable;
};

void __fastcall CCreatureAction_GetUpFromSitting_dtor(CCreatureAction_GetUpFromSitting* action);
void operator delete(void*);

void* __fastcall CCreatureAction_GetUpFromSitting_scalar_deleting_destructor(CCreatureAction_GetUpFromSitting* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_GetUpFromSitting_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}