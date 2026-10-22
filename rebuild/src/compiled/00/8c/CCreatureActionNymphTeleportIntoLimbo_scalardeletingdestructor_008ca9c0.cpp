struct CCreatureAction_NymphTeleportIntoLimbo {
    void* vftable;
};

void __fastcall CCreatureAction_NymphTeleportIntoLimbo_dtor(CCreatureAction_NymphTeleportIntoLimbo* action);
void operator delete(void*);

void* __fastcall CCreatureAction_NymphTeleportIntoLimbo_scalar_deleting_destructor(CCreatureAction_NymphTeleportIntoLimbo* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_NymphTeleportIntoLimbo_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}