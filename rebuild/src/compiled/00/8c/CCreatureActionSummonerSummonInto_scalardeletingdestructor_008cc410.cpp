struct CCreatureAction_SummonerSummonInto {
    void* vftable;
};

void __fastcall CCreatureAction_SummonerSummonInto_dtor(CCreatureAction_SummonerSummonInto* action);
void operator delete(void*);

void* __fastcall CCreatureAction_SummonerSummonInto_scalar_deleting_destructor(CCreatureAction_SummonerSummonInto* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_SummonerSummonInto_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}