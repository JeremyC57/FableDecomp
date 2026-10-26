class CCreatureAction_VillagerAttack { public:
    void* vftable;
};

void __fastcall CCreatureAction_VillagerAttack_dtor(CCreatureAction_VillagerAttack* action);
void operator delete(void*);

void* __fastcall CCreatureAction_VillagerAttack_scalar_deleting_destructor(CCreatureAction_VillagerAttack* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_VillagerAttack_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}