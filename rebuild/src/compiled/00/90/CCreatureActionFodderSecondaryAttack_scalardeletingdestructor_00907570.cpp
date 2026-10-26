class CCreatureAction_FodderSecondaryAttack { public:
    void* vftable;
};

void __fastcall CCreatureAction_FodderSecondaryAttack_dtor(CCreatureAction_FodderSecondaryAttack* action);
void operator delete(void*);

void* __fastcall CCreatureAction_FodderSecondaryAttack_scalar_deleting_destructor(CCreatureAction_FodderSecondaryAttack* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_FodderSecondaryAttack_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}