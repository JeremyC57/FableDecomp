struct CCombatAction_WillHealRelease {
    void* vftable;
};

void __fastcall CCombatAction_WillHealRelease_dtor(CCombatAction_WillHealRelease* action);
void operator delete(void*);

void* __fastcall CCombatAction_WillHealRelease_scalar_deleting_destructor(CCombatAction_WillHealRelease* action, int edx_unused, unsigned int flags)
{
    CCombatAction_WillHealRelease_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}