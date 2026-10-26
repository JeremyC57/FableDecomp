class CCreatureAction_CombatBoast { public:
    void* vftable;
};

void __fastcall CCreatureAction_CombatBoast_dtor(CCreatureAction_CombatBoast* action);
void operator delete(void*);

void* __fastcall CCreatureAction_CombatBoast_scalar_deleting_destructor(CCreatureAction_CombatBoast* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_CombatBoast_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}