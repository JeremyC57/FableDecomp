class CCreatureAction_NymphTeleportOutOfLimbo { public:
    void* vftable;
};

void __fastcall CCreatureAction_NymphTeleportOutOfLimbo_dtor(CCreatureAction_NymphTeleportOutOfLimbo* action);
void operator delete(void*);

void* __fastcall CCreatureAction_NymphTeleportOutOfLimbo_scalar_deleting_destructor(CCreatureAction_NymphTeleportOutOfLimbo* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_NymphTeleportOutOfLimbo_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}