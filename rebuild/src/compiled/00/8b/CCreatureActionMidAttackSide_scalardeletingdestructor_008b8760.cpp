class CCreatureAction_MidAttackSide { public:
    void* vftable;
};

void __fastcall CCreatureAction_MidAttackSide_dtor(CCreatureAction_MidAttackSide* action);
void operator delete(void*);

void* __fastcall CCreatureAction_MidAttackSide_scalar_deleting_destructor(CCreatureAction_MidAttackSide* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_MidAttackSide_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}