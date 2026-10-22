struct CCreatureAction_KrakenRoarLoop {
    void* vftable;
};

void __fastcall CCreatureAction_KrakenRoarLoop_dtor(CCreatureAction_KrakenRoarLoop* action);
void operator delete(void*);

void* __fastcall CCreatureAction_KrakenRoarLoop_scalar_deleting_destructor(CCreatureAction_KrakenRoarLoop* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_KrakenRoarLoop_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}