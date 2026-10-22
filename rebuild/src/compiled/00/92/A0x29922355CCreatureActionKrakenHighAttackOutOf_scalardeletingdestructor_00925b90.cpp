struct CCreatureAction_KrakenHighAttackOutOf {
    void* vftable;
};

void __fastcall CCreatureAction_KrakenHighAttackOutOf_dtor(CCreatureAction_KrakenHighAttackOutOf* action);
void operator delete(void*);

void* __fastcall CCreatureAction_KrakenHighAttackOutOf_scalar_deleting_destructor(CCreatureAction_KrakenHighAttackOutOf* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_KrakenHighAttackOutOf_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}