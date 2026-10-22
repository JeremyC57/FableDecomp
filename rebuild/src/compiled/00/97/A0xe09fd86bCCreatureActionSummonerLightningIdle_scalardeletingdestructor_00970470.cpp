struct CCreatureAction_SummonerLightningIdle {
    void* vftable;
};

void __fastcall CCreatureAction_SummonerLightningIdle_dtor(CCreatureAction_SummonerLightningIdle* action);
void operator delete(void*);

void* __fastcall CCreatureAction_SummonerLightningIdle_scalar_deleting_destructor(CCreatureAction_SummonerLightningIdle* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_SummonerLightningIdle_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}