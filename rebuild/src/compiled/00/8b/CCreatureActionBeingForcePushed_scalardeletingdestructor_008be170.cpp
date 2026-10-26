class CCreatureAction_BeingForcePushed { public:
    void* vftable;
};

void __fastcall CCreatureAction_BeingForcePushed_dtor(CCreatureAction_BeingForcePushed* action);
void operator delete(void*);

void* __fastcall CCreatureAction_BeingForcePushed_scalar_deleting_destructor(CCreatureAction_BeingForcePushed* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_BeingForcePushed_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}