struct CCreatureAction_BanditKingAdvance {
    void* vftable;
};

void __fastcall CCreatureAction_BanditKingAdvance_dtor(CCreatureAction_BanditKingAdvance* action);
void operator delete(void*);

void* __fastcall CCreatureAction_BanditKingAdvance_scalar_deleting_destructor(CCreatureAction_BanditKingAdvance* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_BanditKingAdvance_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}