struct CCreatureAction_KrakenRoarOutOf {
    void* vftable;
};

void __fastcall CCreatureAction_KrakenRoarOutOf_dtor(CCreatureAction_KrakenRoarOutOf* action);
void operator delete(void*);

void* __fastcall CCreatureAction_KrakenRoarOutOf_scalar_deleting_destructor(CCreatureAction_KrakenRoarOutOf* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_KrakenRoarOutOf_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}