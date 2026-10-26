class CCreatureAction_TentacleStrikeGetHit { public:
    void* vftable;
};

void __fastcall CCreatureAction_TentacleStrikeGetHit_dtor(CCreatureAction_TentacleStrikeGetHit* action);
void operator delete(void*);

void* __fastcall CCreatureAction_TentacleStrikeGetHit_scalar_deleting_destructor(CCreatureAction_TentacleStrikeGetHit* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_TentacleStrikeGetHit_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}