struct CCreatureAction_ScreamerIntoAggressive {
    void* vftable;
};

void __fastcall CCreatureAction_ScreamerIntoAggressive_dtor(CCreatureAction_ScreamerIntoAggressive* action);
void operator delete(void*);

void* __fastcall CCreatureAction_ScreamerIntoAggressive_scalar_deleting_destructor(CCreatureAction_ScreamerIntoAggressive* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_ScreamerIntoAggressive_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}