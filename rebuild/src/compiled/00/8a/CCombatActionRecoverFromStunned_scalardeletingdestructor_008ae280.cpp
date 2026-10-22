struct CCombatAction_RecoverFromStunned {
    void* vftable;
};

void __fastcall CCombatAction_RecoverFromStunned_dtor(CCombatAction_RecoverFromStunned* action);
void operator delete(void*);

void* __fastcall CCombatAction_RecoverFromStunned_scalar_deleting_destructor(CCombatAction_RecoverFromStunned* action, int edx_unused, unsigned int flags)
{
    CCombatAction_RecoverFromStunned_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}