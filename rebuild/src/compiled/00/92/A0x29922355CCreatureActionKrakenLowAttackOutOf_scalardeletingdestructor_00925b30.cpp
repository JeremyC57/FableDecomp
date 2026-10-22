struct CCreatureAction_KrakenLowAttackOutOf {
    void* vftable;
};

void __fastcall CCreatureAction_KrakenLowAttackOutOf_dtor(CCreatureAction_KrakenLowAttackOutOf* action);
void operator delete(void*);

void* __fastcall CCreatureAction_KrakenLowAttackOutOf_scalar_deleting_destructor(CCreatureAction_KrakenLowAttackOutOf* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_KrakenLowAttackOutOf_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}