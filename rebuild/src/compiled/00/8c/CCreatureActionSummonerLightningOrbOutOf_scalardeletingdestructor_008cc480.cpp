class CCreatureAction_SummonerLightningOrbOutOf { public:
    void* vftable;
};

void __fastcall CCreatureAction_SummonerLightningOrbOutOf_dtor(CCreatureAction_SummonerLightningOrbOutOf* action);
void operator delete(void*);

void* __fastcall CCreatureAction_SummonerLightningOrbOutOf_scalar_deleting_destructor(CCreatureAction_SummonerLightningOrbOutOf* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_SummonerLightningOrbOutOf_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}