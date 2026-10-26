class CCreatureAction_UnblockableMidAttack { public:
    void* vftable;
};

void __fastcall CCreatureAction_UnblockableMidAttack_dtor(CCreatureAction_UnblockableMidAttack* action);
void operator delete(void*);

void* __fastcall CCreatureAction_UnblockableMidAttack_scalar_deleting_destructor(CCreatureAction_UnblockableMidAttack* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_UnblockableMidAttack_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}