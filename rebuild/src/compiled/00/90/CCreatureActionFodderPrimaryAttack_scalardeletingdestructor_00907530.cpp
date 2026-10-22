struct CCreatureAction_FodderPrimaryAttack {
    void* vftable;
};

void __fastcall CCreatureAction_FodderPrimaryAttack_dtor(CCreatureAction_FodderPrimaryAttack* action);
void operator delete(void*);

void* __fastcall CCreatureAction_FodderPrimaryAttack_scalar_deleting_destructor(CCreatureAction_FodderPrimaryAttack* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_FodderPrimaryAttack_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}