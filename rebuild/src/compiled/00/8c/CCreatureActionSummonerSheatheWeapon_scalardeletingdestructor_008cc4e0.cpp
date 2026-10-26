class CCreatureAction_SummonerSheatheWeapon { public:
    void* vftable;
};

void __fastcall CCreatureAction_SummonerSheatheWeapon_dtor(CCreatureAction_SummonerSheatheWeapon* action);
void operator delete(void*);

void* __fastcall CCreatureAction_SummonerSheatheWeapon_scalar_deleting_destructor(CCreatureAction_SummonerSheatheWeapon* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_SummonerSheatheWeapon_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}