struct CCreatureAction_StrafeForward {
    void* vftable;
};

void __fastcall CCreatureAction_StrafeForward_dtor(CCreatureAction_StrafeForward* action);
void operator delete(void*);

void* __fastcall CCreatureAction_StrafeForward_scalar_deleting_destructor(CCreatureAction_StrafeForward* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_StrafeForward_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}