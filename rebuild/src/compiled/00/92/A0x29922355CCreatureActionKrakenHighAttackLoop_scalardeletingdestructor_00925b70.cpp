struct CCreatureAction_KrakenHighAttackLoop {
    void* vftable;
};

void __fastcall CCreatureAction_KrakenHighAttackLoop_dtor(CCreatureAction_KrakenHighAttackLoop* action);
void operator delete(void*);

void* __fastcall CCreatureAction_KrakenHighAttackLoop_scalar_deleting_destructor(CCreatureAction_KrakenHighAttackLoop* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_KrakenHighAttackLoop_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}