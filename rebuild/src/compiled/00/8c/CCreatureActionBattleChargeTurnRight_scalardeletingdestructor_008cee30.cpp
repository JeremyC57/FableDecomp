struct CCreatureAction_BattleChargeTurnRight {
    void* vftable;
};

void __fastcall CCreatureAction_BattleChargeTurnRight_dtor(CCreatureAction_BattleChargeTurnRight* action);
void operator delete(void*);

void* __fastcall CCreatureAction_BattleChargeTurnRight_scalar_deleting_destructor(CCreatureAction_BattleChargeTurnRight* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_BattleChargeTurnRight_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}