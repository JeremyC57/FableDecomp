struct CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath {
    void* vftable;
};

void __fastcall CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath_dtor(CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath* action);
void operator delete(void*);

void* __fastcall CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath_scalar_deleting_destructor(CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath* action, int edx_unused, unsigned int flags)
{
    CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}