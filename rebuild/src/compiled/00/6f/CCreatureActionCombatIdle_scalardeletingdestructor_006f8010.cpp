struct CCreatureAction_CombatIdle {
    void* vftable;
};

void __fastcall CCreatureAction_CombatIdle_dtor(CCreatureAction_CombatIdle* action);
void operator delete(void*);

void* __fastcall CCreatureAction_CombatIdle_scalar_deleting_destructor(CCreatureAction_CombatIdle* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_CombatIdle_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}