struct CCreatureAction_KrakenRoarInto {
    void* vftable;
};

void __fastcall CCreatureAction_KrakenRoarInto_dtor(CCreatureAction_KrakenRoarInto* action);
void operator delete(void*);

void* __fastcall CCreatureAction_KrakenRoarInto_scalar_deleting_destructor(CCreatureAction_KrakenRoarInto* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_KrakenRoarInto_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}