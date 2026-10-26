class CCreatureAction_BalverineLungeAttack { public:
    void* vftable;
};

void __fastcall CCreatureAction_BalverineLungeAttack_dtor(CCreatureAction_BalverineLungeAttack* action);
void operator delete(void*);

void* __fastcall CCreatureAction_BalverineLungeAttack_scalar_deleting_destructor(CCreatureAction_BalverineLungeAttack* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_BalverineLungeAttack_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}