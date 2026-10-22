struct CCombatAction_RecoverFromDazed {
    void* vftable;
};

void __fastcall CCombatAction_RecoverFromDazed_dtor(CCombatAction_RecoverFromDazed* action);
void operator delete(void*);

void* __fastcall CCombatAction_RecoverFromDazed_scalar_deleting_destructor(CCombatAction_RecoverFromDazed* action, int edx_unused, unsigned int flags)
{
    CCombatAction_RecoverFromDazed_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}