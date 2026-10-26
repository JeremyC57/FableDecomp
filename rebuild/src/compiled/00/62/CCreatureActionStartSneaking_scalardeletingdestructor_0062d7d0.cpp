class CCreatureAction_StartSneaking { public:
    void* vftable;
};

void __fastcall CCreatureAction_StartSneaking_dtor(CCreatureAction_StartSneaking* action);
void operator delete(void*);

void* __fastcall CCreatureAction_StartSneaking_scalar_deleting_destructor(CCreatureAction_StartSneaking* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_StartSneaking_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}