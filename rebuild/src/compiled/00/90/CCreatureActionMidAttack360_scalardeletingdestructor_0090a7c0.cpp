class CCreatureAction_MidAttack360 { public:
    void* vftable;
};

void __fastcall CCreatureAction_MidAttack360_dtor(CCreatureAction_MidAttack360* action);
void operator delete(void*);

void* __fastcall CCreatureAction_MidAttack360_scalar_deleting_destructor(CCreatureAction_MidAttack360* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_MidAttack360_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}