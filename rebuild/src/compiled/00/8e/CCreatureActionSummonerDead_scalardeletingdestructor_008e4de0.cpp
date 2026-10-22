struct CCreatureAction_SummonerDead {
    void* vftable;
};

void __fastcall CCreatureAction_SummonerDead_dtor(CCreatureAction_SummonerDead* action);
void operator delete(void*);

void* __fastcall CCreatureAction_SummonerDead_scalar_deleting_destructor(CCreatureAction_SummonerDead* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_SummonerDead_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}