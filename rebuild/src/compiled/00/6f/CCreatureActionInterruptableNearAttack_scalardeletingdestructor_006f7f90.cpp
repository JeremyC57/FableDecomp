struct CCreatureAction_InterruptableNearAttack {
    void* vftable;
};

void __fastcall CCreatureAction_InterruptableNearAttack_dtor(CCreatureAction_InterruptableNearAttack* action);
void operator delete(void*);

void* __fastcall CCreatureAction_InterruptableNearAttack_scalar_deleting_destructor(CCreatureAction_InterruptableNearAttack* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_InterruptableNearAttack_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}