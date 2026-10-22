struct CCreatureAction_VillagerAdvance {
    void* vftable;
};

void __fastcall CCreatureAction_VillagerAdvance_dtor(CCreatureAction_VillagerAdvance* action);
void operator delete(void*);

void* __fastcall CCreatureAction_VillagerAdvance_scalar_deleting_destructor(CCreatureAction_VillagerAdvance* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_VillagerAdvance_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}