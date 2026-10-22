struct CCreatureAction_KrakenLowAttackInto {
    void* vftable;
};

void __fastcall CCreatureAction_KrakenLowAttackInto_dtor(CCreatureAction_KrakenLowAttackInto* action);
void operator delete(void*);

void* __fastcall CCreatureAction_KrakenLowAttackInto_scalar_deleting_destructor(CCreatureAction_KrakenLowAttackInto* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_KrakenLowAttackInto_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}