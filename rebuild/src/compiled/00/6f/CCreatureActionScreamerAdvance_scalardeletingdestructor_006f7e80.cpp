struct CCreatureAction_ScreamerAdvance {
    void* vftable;
};

void __fastcall CCreatureAction_ScreamerAdvance_dtor(CCreatureAction_ScreamerAdvance* action);
void operator delete(void*);

void* __fastcall CCreatureAction_ScreamerAdvance_scalar_deleting_destructor(CCreatureAction_ScreamerAdvance* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_ScreamerAdvance_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}