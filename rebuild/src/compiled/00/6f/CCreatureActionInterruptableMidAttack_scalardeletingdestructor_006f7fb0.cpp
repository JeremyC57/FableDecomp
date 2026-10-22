struct CCreatureAction_InterruptableMidAttack {
    void* vftable;
};

void __fastcall CCreatureAction_InterruptableMidAttack_dtor(CCreatureAction_InterruptableMidAttack* action);
void operator delete(void*);

void* __fastcall CCreatureAction_InterruptableMidAttack_scalar_deleting_destructor(CCreatureAction_InterruptableMidAttack* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_InterruptableMidAttack_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}