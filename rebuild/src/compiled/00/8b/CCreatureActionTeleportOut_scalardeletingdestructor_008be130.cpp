struct CCreatureAction_TeleportOut {
    void* vftable;
};

void __fastcall CCreatureAction_TeleportOut_dtor(CCreatureAction_TeleportOut* action);
void operator delete(void*);

void* __fastcall CCreatureAction_TeleportOut_scalar_deleting_destructor(CCreatureAction_TeleportOut* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_TeleportOut_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}