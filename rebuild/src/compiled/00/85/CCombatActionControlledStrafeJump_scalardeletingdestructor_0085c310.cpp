struct CCombatAction_ControlledStrafeJump {
    void* vftable;
};

void __fastcall CCombatAction_ControlledStrafeJump_dtor(CCombatAction_ControlledStrafeJump* action);
void operator delete(void*);

void* __fastcall CCombatAction_ControlledStrafeJump_scalar_deleting_destructor(CCombatAction_ControlledStrafeJump* action, int edx_unused, unsigned int flags)
{
    CCombatAction_ControlledStrafeJump_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}