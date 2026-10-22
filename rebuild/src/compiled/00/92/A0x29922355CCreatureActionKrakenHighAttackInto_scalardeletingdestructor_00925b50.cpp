struct CCreatureAction_KrakenHighAttackInto {
    void* vftable;
};

void __fastcall CCreatureAction_KrakenHighAttackInto_dtor(CCreatureAction_KrakenHighAttackInto* action);
void operator delete(void*);

void* __fastcall CCreatureAction_KrakenHighAttackInto_scalar_deleting_destructor(CCreatureAction_KrakenHighAttackInto* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_KrakenHighAttackInto_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}