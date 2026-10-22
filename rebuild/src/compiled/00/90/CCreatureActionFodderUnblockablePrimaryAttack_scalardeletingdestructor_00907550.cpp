struct CCreatureAction_FodderUnblockablePrimaryAttack {
    void* vftable;
};

void __fastcall CCreatureAction_FodderUnblockablePrimaryAttack_dtor(CCreatureAction_FodderUnblockablePrimaryAttack* action);
void operator delete(void*);

void* __fastcall CCreatureAction_FodderUnblockablePrimaryAttack_scalar_deleting_destructor(CCreatureAction_FodderUnblockablePrimaryAttack* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_FodderUnblockablePrimaryAttack_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}