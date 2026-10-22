struct CCreatureAction_BanditKingAttack {
    void* vftable;
};

void __fastcall CCreatureAction_BanditKingAttack_dtor(CCreatureAction_BanditKingAttack* action);
void operator delete(void*);

void* __fastcall CCreatureAction_BanditKingAttack_scalar_deleting_destructor(CCreatureAction_BanditKingAttack* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_BanditKingAttack_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}