struct CCreatureAction_TrollTunnel {
    void* vftable;
};

void __fastcall CCreatureAction_TrollTunnel_dtor(CCreatureAction_TrollTunnel* action);
void operator delete(void*);

void* __fastcall CCreatureAction_TrollTunnel_scalar_deleting_destructor(CCreatureAction_TrollTunnel* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_TrollTunnel_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}