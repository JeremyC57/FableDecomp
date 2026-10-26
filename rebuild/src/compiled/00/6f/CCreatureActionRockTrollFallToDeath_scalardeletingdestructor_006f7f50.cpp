class CCreatureAction_RockTrollFallToDeath { public:
    void* vftable;
};

void __fastcall CCreatureAction_RockTrollFallToDeath_dtor(CCreatureAction_RockTrollFallToDeath* action);
void operator delete(void*);

void* __fastcall CCreatureAction_RockTrollFallToDeath_scalar_deleting_destructor(CCreatureAction_RockTrollFallToDeath* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_RockTrollFallToDeath_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}