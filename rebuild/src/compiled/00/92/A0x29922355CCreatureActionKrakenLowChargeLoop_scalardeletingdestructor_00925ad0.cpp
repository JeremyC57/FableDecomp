struct CCreatureAction_KrakenLowChargeLoop {
    void* vftable;
};

void __fastcall CCreatureAction_KrakenLowChargeLoop_dtor(CCreatureAction_KrakenLowChargeLoop* action);
void operator delete(void*);

void* __fastcall CCreatureAction_KrakenLowChargeLoop_scalar_deleting_destructor(CCreatureAction_KrakenLowChargeLoop* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_KrakenLowChargeLoop_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}