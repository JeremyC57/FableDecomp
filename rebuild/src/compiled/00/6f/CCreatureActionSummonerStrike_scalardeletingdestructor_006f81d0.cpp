class CCreatureAction_SummonerStrike { public:
    void* vftable;
};

void __fastcall CCreatureAction_SummonerStrike_dtor(CCreatureAction_SummonerStrike* action);
void operator delete(void*);

void* __fastcall CCreatureAction_SummonerStrike_scalar_deleting_destructor(CCreatureAction_SummonerStrike* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_SummonerStrike_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}