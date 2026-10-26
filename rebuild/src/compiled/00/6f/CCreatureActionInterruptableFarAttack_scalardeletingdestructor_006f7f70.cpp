class CCreatureAction_InterruptableFarAttack { public:
    void* vftable;
};

void __fastcall CCreatureAction_InterruptableFarAttack_dtor(CCreatureAction_InterruptableFarAttack* action);
void operator delete(void*);

void* __fastcall CCreatureAction_InterruptableFarAttack_scalar_deleting_destructor(CCreatureAction_InterruptableFarAttack* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_InterruptableFarAttack_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}