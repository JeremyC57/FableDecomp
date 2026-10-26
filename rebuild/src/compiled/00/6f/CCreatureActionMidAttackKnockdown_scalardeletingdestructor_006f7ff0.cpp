class CCreatureAction_MidAttackKnockdown { public:
    void* vftable;
};

void __fastcall CCreatureAction_MidAttackKnockdown_dtor(CCreatureAction_MidAttackKnockdown* action);
void operator delete(void*);

void* __fastcall CCreatureAction_MidAttackKnockdown_scalar_deleting_destructor(CCreatureAction_MidAttackKnockdown* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_MidAttackKnockdown_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}