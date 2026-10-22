struct CCreatureAction_WaspQueenPrimaryAttack {
    void* vftable;
};

void __fastcall CCreatureAction_WaspQueenPrimaryAttack_dtor(CCreatureAction_WaspQueenPrimaryAttack* action);
void operator delete(void*);

void* __fastcall CCreatureAction_WaspQueenPrimaryAttack_scalar_deleting_destructor(CCreatureAction_WaspQueenPrimaryAttack* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_WaspQueenPrimaryAttack_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}