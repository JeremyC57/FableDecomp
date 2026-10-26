class CCreatureAction_TentacleStrikeLoop { public:
    void* vftable;
};

void __fastcall CCreatureAction_TentacleStrikeLoop_dtor(CCreatureAction_TentacleStrikeLoop* action);
void operator delete(void*);

void* __fastcall CCreatureAction_TentacleStrikeLoop_scalar_deleting_destructor(CCreatureAction_TentacleStrikeLoop* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_TentacleStrikeLoop_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}