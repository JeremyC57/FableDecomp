struct CCombatAction_WillForcePushInto {
    void* vftable;
};

void __fastcall CCombatAction_WillForcePushInto_dtor(CCombatAction_WillForcePushInto* action);
void operator delete(void*);

void* __fastcall CCombatAction_WillForcePushInto_scalar_deleting_destructor(CCombatAction_WillForcePushInto* action, int edx_unused, unsigned int flags)
{
    CCombatAction_WillForcePushInto_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}