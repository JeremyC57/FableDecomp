struct CCreatureAction_BalverineCorneredJumpIntoSky {
    void* vftable;
};

void __fastcall CCreatureAction_BalverineCorneredJumpIntoSky_dtor(CCreatureAction_BalverineCorneredJumpIntoSky* action);
void operator delete(void*);

void* __fastcall CCreatureAction_BalverineCorneredJumpIntoSky_scalar_deleting_destructor(CCreatureAction_BalverineCorneredJumpIntoSky* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_BalverineCorneredJumpIntoSky_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}