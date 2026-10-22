struct CCreatureAction_JackOfBladesUnblockableMidAttackNoBlend {
    void* vftable;
};

void __fastcall CCreatureAction_JackOfBladesUnblockableMidAttackNoBlend_dtor(CCreatureAction_JackOfBladesUnblockableMidAttackNoBlend* action);
void operator delete(void*);

void* __fastcall CCreatureAction_JackOfBladesUnblockableMidAttackNoBlend_scalar_deleting_destructor(CCreatureAction_JackOfBladesUnblockableMidAttackNoBlend* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_JackOfBladesUnblockableMidAttackNoBlend_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}