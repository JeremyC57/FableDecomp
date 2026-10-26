class CCreatureAction_SummonerUnsheatheStrike { public:
    void* vftable;
};

void __fastcall CCreatureAction_SummonerUnsheatheStrike_dtor(CCreatureAction_SummonerUnsheatheStrike* action);
void operator delete(void*);

void* __fastcall CCreatureAction_SummonerUnsheatheStrike_scalar_deleting_destructor(CCreatureAction_SummonerUnsheatheStrike* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_SummonerUnsheatheStrike_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}