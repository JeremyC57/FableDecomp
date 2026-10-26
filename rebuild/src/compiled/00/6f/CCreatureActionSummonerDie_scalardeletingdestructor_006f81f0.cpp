class CCreatureAction_SummonerDie { public:
    void* vftable;
};

void __fastcall CCreatureAction_SummonerDie_dtor(CCreatureAction_SummonerDie* action);
void operator delete(void*);

void* __fastcall CCreatureAction_SummonerDie_scalar_deleting_destructor(CCreatureAction_SummonerDie* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_SummonerDie_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}