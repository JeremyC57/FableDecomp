struct CCreatureAction_TentacleStrikeInto {
    void* vftable;
};

void __fastcall CCreatureAction_TentacleStrikeInto_dtor(CCreatureAction_TentacleStrikeInto* action);
void operator delete(void*);

void* __fastcall CCreatureAction_TentacleStrikeInto_scalar_deleting_destructor(CCreatureAction_TentacleStrikeInto* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_TentacleStrikeInto_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}