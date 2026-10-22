struct CCreatureAction_TeleportIn {
    void* vftable;
};

void __fastcall CCreatureAction_TeleportIn_dtor(CCreatureAction_TeleportIn* action);
void operator delete(void*);

void* __fastcall CCreatureAction_TeleportIn_scalar_deleting_destructor(CCreatureAction_TeleportIn* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_TeleportIn_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}