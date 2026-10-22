struct CCreatureAction_KrakenReturnToWater {
    void* vftable;
};

void __fastcall CCreatureAction_KrakenReturnToWater_dtor(CCreatureAction_KrakenReturnToWater* action);
void operator delete(void*);

void* __fastcall CCreatureAction_KrakenReturnToWater_scalar_deleting_destructor(CCreatureAction_KrakenReturnToWater* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_KrakenReturnToWater_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}