class CCreatureAction_CastBattleCharge { public:
    void* vftable;
};

void __fastcall CCreatureAction_CastBattleCharge_dtor(CCreatureAction_CastBattleCharge* action);
void operator delete(void*);

void* __fastcall CCreatureAction_CastBattleCharge_scalar_deleting_destructor(CCreatureAction_CastBattleCharge* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_CastBattleCharge_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}