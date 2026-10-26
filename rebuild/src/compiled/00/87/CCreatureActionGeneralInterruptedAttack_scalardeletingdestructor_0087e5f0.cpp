class CCreatureAction_GeneralInterruptedAttack { public:
    void* vftable;
};

void __fastcall CCreatureAction_GeneralInterruptedAttack_dtor(CCreatureAction_GeneralInterruptedAttack* action);
void operator delete(void*);

void* __fastcall CCreatureAction_GeneralInterruptedAttack_scalar_deleting_destructor(CCreatureAction_GeneralInterruptedAttack* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_GeneralInterruptedAttack_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}