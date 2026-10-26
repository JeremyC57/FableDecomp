class CCreatureAction_ScreamerDrainAttack { public:
    void* vftable;
};

void __fastcall CCreatureAction_ScreamerDrainAttack_dtor(CCreatureAction_ScreamerDrainAttack* action);
void operator delete(void*);

void* __fastcall CCreatureAction_ScreamerDrainAttack_scalar_deleting_destructor(CCreatureAction_ScreamerDrainAttack* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_ScreamerDrainAttack_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}