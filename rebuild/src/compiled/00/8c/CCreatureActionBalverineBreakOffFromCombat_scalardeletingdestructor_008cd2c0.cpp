class CCreatureAction_BalverineBreakOffFromCombat { public:
    void* vftable;
};

void __fastcall CCreatureAction_BalverineBreakOffFromCombat_dtor(CCreatureAction_BalverineBreakOffFromCombat* action);
void operator delete(void*);

void* __fastcall CCreatureAction_BalverineBreakOffFromCombat_scalar_deleting_destructor(CCreatureAction_BalverineBreakOffFromCombat* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_BalverineBreakOffFromCombat_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}