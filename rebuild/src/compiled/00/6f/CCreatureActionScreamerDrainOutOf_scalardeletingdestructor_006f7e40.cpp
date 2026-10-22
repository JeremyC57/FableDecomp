struct CCreatureAction_ScreamerDrainOutOf {
    void* vftable;
};

void __fastcall CCreatureAction_ScreamerDrainOutOf_dtor(CCreatureAction_ScreamerDrainOutOf* action);
void operator delete(void*);

void* __fastcall CCreatureAction_ScreamerDrainOutOf_scalar_deleting_destructor(CCreatureAction_ScreamerDrainOutOf* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_ScreamerDrainOutOf_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}