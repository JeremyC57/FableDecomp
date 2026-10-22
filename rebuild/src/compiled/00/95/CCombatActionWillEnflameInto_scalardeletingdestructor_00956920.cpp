struct CCombatAction_WillEnflameInto {
    void* vftable;
};

void __fastcall CCombatAction_WillEnflameInto_dtor(CCombatAction_WillEnflameInto* action);
void operator delete(void*);

void* __fastcall CCombatAction_WillEnflameInto_scalar_deleting_destructor(CCombatAction_WillEnflameInto* action, int edx_unused, unsigned int flags)
{
    CCombatAction_WillEnflameInto_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}