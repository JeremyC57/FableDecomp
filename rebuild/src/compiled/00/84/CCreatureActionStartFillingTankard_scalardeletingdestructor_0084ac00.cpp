class CCreatureAction_StartFillingTankard { public:
    void* vftable;
};

void __fastcall CCreatureAction_StartFillingTankard_dtor(CCreatureAction_StartFillingTankard* action);
void operator delete(void*);

void* __fastcall CCreatureAction_StartFillingTankard_scalar_deleting_destructor(CCreatureAction_StartFillingTankard* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_StartFillingTankard_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}