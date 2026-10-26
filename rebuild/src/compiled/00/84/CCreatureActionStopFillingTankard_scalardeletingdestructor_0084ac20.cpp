class CCreatureAction_StopFillingTankard { public:
    void* vftable;
};

void __fastcall CCreatureAction_StopFillingTankard_dtor(CCreatureAction_StopFillingTankard* action);
void operator delete(void*);

void* __fastcall CCreatureAction_StopFillingTankard_scalar_deleting_destructor(CCreatureAction_StopFillingTankard* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_StopFillingTankard_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}