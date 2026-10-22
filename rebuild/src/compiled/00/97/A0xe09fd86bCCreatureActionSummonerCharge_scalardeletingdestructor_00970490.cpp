struct CCreatureAction_SummonerCharge {
    void* vftable;
};

void __fastcall CCreatureAction_SummonerCharge_dtor(CCreatureAction_SummonerCharge* action);
void operator delete(void*);

void* __fastcall CCreatureAction_SummonerCharge_scalar_deleting_destructor(CCreatureAction_SummonerCharge* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_SummonerCharge_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}