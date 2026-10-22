struct CCreatureAction_TentacleIdle {
    void* vftable;
};

void __fastcall CCreatureAction_TentacleIdle_dtor(CCreatureAction_TentacleIdle* action);
void operator delete(void*);

void* __fastcall CCreatureAction_TentacleIdle_scalar_deleting_destructor(CCreatureAction_TentacleIdle* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_TentacleIdle_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}