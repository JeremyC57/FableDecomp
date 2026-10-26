class CCreatureAction_SummonerLightningOrbOutOfGroundSmash { public:
    void* vftable;
};

void __fastcall CCreatureAction_SummonerLightningOrbOutOfGroundSmash_dtor(CCreatureAction_SummonerLightningOrbOutOfGroundSmash* action);
void operator delete(void*);

void* __fastcall CCreatureAction_SummonerLightningOrbOutOfGroundSmash_scalar_deleting_destructor(CCreatureAction_SummonerLightningOrbOutOfGroundSmash* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_SummonerLightningOrbOutOfGroundSmash_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}