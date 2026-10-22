struct CCreatureAction_TentacleStrikeOutOf {
    void* vftable;
};

void __fastcall CCreatureAction_TentacleStrikeOutOf_dtor(CCreatureAction_TentacleStrikeOutOf* action);
void operator delete(void*);

void* __fastcall CCreatureAction_TentacleStrikeOutOf_scalar_deleting_destructor(CCreatureAction_TentacleStrikeOutOf* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_TentacleStrikeOutOf_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}