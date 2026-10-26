class CCreatureAction_BuildUpBattleCharge { public:
    void* vftable;
};

void __fastcall CCreatureAction_BuildUpBattleCharge_dtor(CCreatureAction_BuildUpBattleCharge* action);
void operator delete(void*);

void* __fastcall CCreatureAction_BuildUpBattleCharge_scalar_deleting_destructor(CCreatureAction_BuildUpBattleCharge* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_BuildUpBattleCharge_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}