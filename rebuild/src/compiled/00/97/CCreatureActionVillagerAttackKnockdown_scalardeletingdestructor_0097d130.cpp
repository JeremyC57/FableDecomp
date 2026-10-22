struct CCreatureAction_VillagerAttackKnockdown {
    void* vftable;
};

void __fastcall CCreatureAction_VillagerAttackKnockdown_dtor(CCreatureAction_VillagerAttackKnockdown* action);
void operator delete(void*);

void* __fastcall CCreatureAction_VillagerAttackKnockdown_scalar_deleting_destructor(CCreatureAction_VillagerAttackKnockdown* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_VillagerAttackKnockdown_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}