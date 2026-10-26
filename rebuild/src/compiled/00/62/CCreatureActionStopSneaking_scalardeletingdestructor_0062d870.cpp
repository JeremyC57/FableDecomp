class CCreatureAction_StopSneaking { public:
    void* vftable;
};

void __fastcall CCreatureAction_StopSneaking_dtor(CCreatureAction_StopSneaking* action);
void operator delete(void*);

void* __fastcall CCreatureAction_StopSneaking_scalar_deleting_destructor(CCreatureAction_StopSneaking* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_StopSneaking_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}