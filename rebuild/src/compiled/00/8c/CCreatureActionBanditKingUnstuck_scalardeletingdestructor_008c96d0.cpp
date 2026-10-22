struct CCreatureAction_BanditKingUnstuck {
    void* vftable;
};

void __fastcall CCreatureAction_BanditKingUnstuck_dtor(CCreatureAction_BanditKingUnstuck* action);
void operator delete(void*);

void* __fastcall CCreatureAction_BanditKingUnstuck_scalar_deleting_destructor(CCreatureAction_BanditKingUnstuck* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_BanditKingUnstuck_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}