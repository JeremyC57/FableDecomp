struct CCombatAction_StabThing {
    void* vftable;
};

void __fastcall CCombatAction_StabThing_dtor(CCombatAction_StabThing* action);
void operator delete(void*);

void* __fastcall CCombatAction_StabThing_scalar_deleting_destructor(CCombatAction_StabThing* action, int edx_unused, unsigned int flags)
{
    CCombatAction_StabThing_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}