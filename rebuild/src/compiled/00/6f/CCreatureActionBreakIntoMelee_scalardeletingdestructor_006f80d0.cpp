struct CCreatureAction_BreakIntoMelee {
    void* vftable;
};

void __fastcall CCreatureAction_BreakIntoMelee_dtor(CCreatureAction_BreakIntoMelee* action);
void operator delete(void*);

void* __fastcall CCreatureAction_BreakIntoMelee_scalar_deleting_destructor(CCreatureAction_BreakIntoMelee* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_BreakIntoMelee_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}