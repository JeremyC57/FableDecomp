struct CCreatureAction_VillagerBackOff {
    void* vftable;
};

void __fastcall CCreatureAction_VillagerBackOff_dtor(CCreatureAction_VillagerBackOff* action);
void operator delete(void*);

void* __fastcall CCreatureAction_VillagerBackOff_scalar_deleting_destructor(CCreatureAction_VillagerBackOff* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_VillagerBackOff_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}