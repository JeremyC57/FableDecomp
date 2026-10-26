class CCreatureAction_BalverineBreakOffFromCombatLong { public:
    void* vftable;
};

void __fastcall CCreatureAction_BalverineBreakOffFromCombatLong_dtor(CCreatureAction_BalverineBreakOffFromCombatLong* action);
void operator delete(void*);

void* __fastcall CCreatureAction_BalverineBreakOffFromCombatLong_scalar_deleting_destructor(CCreatureAction_BalverineBreakOffFromCombatLong* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_BalverineBreakOffFromCombatLong_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}