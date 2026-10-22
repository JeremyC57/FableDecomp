struct CCreatureAction_SummonerSummonOutOf {
    void* vftable;
};

void __fastcall CCreatureAction_SummonerSummonOutOf_dtor(CCreatureAction_SummonerSummonOutOf* action);
void operator delete(void*);

void* __fastcall CCreatureAction_SummonerSummonOutOf_scalar_deleting_destructor(CCreatureAction_SummonerSummonOutOf* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_SummonerSummonOutOf_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}