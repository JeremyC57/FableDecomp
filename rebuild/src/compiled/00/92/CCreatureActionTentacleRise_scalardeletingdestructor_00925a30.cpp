struct CCreatureAction_TentacleRise {
    void* vftable;
};

void __fastcall CCreatureAction_TentacleRise_dtor(CCreatureAction_TentacleRise* action);
void operator delete(void*);

void* __fastcall CCreatureAction_TentacleRise_scalar_deleting_destructor(CCreatureAction_TentacleRise* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_TentacleRise_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}