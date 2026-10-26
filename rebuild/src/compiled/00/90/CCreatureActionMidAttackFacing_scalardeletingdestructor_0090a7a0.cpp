class CCreatureAction_MidAttackFacing { public:
    void* vftable;
};

void __fastcall CCreatureAction_MidAttackFacing_dtor(CCreatureAction_MidAttackFacing* action);
void operator delete(void*);

void* __fastcall CCreatureAction_MidAttackFacing_scalar_deleting_destructor(CCreatureAction_MidAttackFacing* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_MidAttackFacing_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}