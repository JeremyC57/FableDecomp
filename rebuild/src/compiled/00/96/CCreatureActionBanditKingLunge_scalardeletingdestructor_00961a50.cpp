struct CCreatureAction_BanditKingLunge {
    void* vftable;
};

void __fastcall CCreatureAction_BanditKingLunge_dtor(CCreatureAction_BanditKingLunge* action);
void operator delete(void*);

void* __fastcall CCreatureAction_BanditKingLunge_scalar_deleting_destructor(CCreatureAction_BanditKingLunge* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_BanditKingLunge_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}