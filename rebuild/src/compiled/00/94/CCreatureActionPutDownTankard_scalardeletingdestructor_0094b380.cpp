struct CCreatureAction_PutDownTankard {
    void* vftable;
};

void __fastcall CCreatureAction_PutDownTankard_dtor(CCreatureAction_PutDownTankard* action);
void operator delete(void*);

void* __fastcall CCreatureAction_PutDownTankard_scalar_deleting_destructor(CCreatureAction_PutDownTankard* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_PutDownTankard_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}