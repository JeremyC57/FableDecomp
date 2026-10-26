class CCreatureAction_RockTrollShakeAndExplode { public:
    void* vftable;
};

void __fastcall CCreatureAction_RockTrollShakeAndExplode_dtor(CCreatureAction_RockTrollShakeAndExplode* action);
void operator delete(void*);

void* __fastcall CCreatureAction_RockTrollShakeAndExplode_scalar_deleting_destructor(CCreatureAction_RockTrollShakeAndExplode* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_RockTrollShakeAndExplode_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}