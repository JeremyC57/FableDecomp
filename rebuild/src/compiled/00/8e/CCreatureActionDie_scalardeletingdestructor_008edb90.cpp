struct CCreatureAction_Die {
    void* vftable;
};

void __fastcall CCreatureAction_Die_dtor(CCreatureAction_Die* action);
void operator delete(void*);

void* __fastcall CCreatureAction_Die_scalar_deleting_destructor(CCreatureAction_Die* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_Die_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}