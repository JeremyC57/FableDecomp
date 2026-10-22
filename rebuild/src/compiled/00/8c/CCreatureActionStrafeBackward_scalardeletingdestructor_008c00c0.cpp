struct CCreatureAction_StrafeBackward {
    void* vftable;
};

void __fastcall CCreatureAction_StrafeBackward_dtor(CCreatureAction_StrafeBackward* action);
void operator delete(void*);

void* __fastcall CCreatureAction_StrafeBackward_scalar_deleting_destructor(CCreatureAction_StrafeBackward* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_StrafeBackward_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}