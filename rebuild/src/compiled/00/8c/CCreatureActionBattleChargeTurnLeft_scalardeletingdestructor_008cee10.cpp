struct CCreatureAction_BattleChargeTurnLeft {
    void* vftable;
};

void __fastcall CCreatureAction_BattleChargeTurnLeft_dtor(CCreatureAction_BattleChargeTurnLeft* action);
void operator delete(void*);

void* __fastcall CCreatureAction_BattleChargeTurnLeft_scalar_deleting_destructor(CCreatureAction_BattleChargeTurnLeft* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_BattleChargeTurnLeft_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}