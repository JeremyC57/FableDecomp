struct CCombatAction_FallStunned {
    void* vftable;
};

void __fastcall CCombatAction_FallStunned_dtor(CCombatAction_FallStunned* action);
void operator delete(void*);

void* __fastcall CCombatAction_FallStunned_scalar_deleting_destructor(CCombatAction_FallStunned* action, int edx_unused, unsigned int flags)
{
    CCombatAction_FallStunned_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}