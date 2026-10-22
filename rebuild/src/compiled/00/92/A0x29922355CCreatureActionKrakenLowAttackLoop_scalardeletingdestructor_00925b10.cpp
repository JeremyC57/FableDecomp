struct CCreatureAction_KrakenLowAttackLoop {
    void* vftable;
};

void __fastcall CCreatureAction_KrakenLowAttackLoop_dtor(CCreatureAction_KrakenLowAttackLoop* action);
void operator delete(void*);

void* __fastcall CCreatureAction_KrakenLowAttackLoop_scalar_deleting_destructor(CCreatureAction_KrakenLowAttackLoop* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_KrakenLowAttackLoop_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}