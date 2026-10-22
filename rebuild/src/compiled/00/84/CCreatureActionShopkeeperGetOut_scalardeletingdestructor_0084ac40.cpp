struct CCreatureAction_ShopkeeperGetOut {
    void* vftable;
};

void __fastcall CCreatureAction_ShopkeeperGetOut_dtor(CCreatureAction_ShopkeeperGetOut* action);
void operator delete(void*);

void* __fastcall CCreatureAction_ShopkeeperGetOut_scalar_deleting_destructor(CCreatureAction_ShopkeeperGetOut* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_ShopkeeperGetOut_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}