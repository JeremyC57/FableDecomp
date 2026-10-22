struct CCreatureAction_ChargeStrike {
    void* vftable;
};

void __fastcall CCreatureAction_ChargeStrike_dtor(CCreatureAction_ChargeStrike* action);
void operator delete(void*);

void* __fastcall CCreatureAction_ChargeStrike_scalar_deleting_destructor(CCreatureAction_ChargeStrike* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_ChargeStrike_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}