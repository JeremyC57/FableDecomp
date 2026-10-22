struct CCreatureAction_Charge {
    void* vftable;
};

void __fastcall CCreatureAction_Charge_dtor(CCreatureAction_Charge* action);
void operator delete(void*);

void* __fastcall CCreatureAction_Charge_scalar_deleting_destructor(CCreatureAction_Charge* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_Charge_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}