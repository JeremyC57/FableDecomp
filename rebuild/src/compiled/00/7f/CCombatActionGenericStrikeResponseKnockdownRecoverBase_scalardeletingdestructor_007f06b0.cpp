struct CCombatAction_GenericStrikeResponseKnockdownRecoverBase {
    void* vftable;
};

void __fastcall CCombatAction_GenericStrikeResponseKnockdownRecoverBase_dtor(CCombatAction_GenericStrikeResponseKnockdownRecoverBase* action);
void operator delete(void*);

void* __fastcall CCombatAction_GenericStrikeResponseKnockdownRecoverBase_scalar_deleting_destructor(CCombatAction_GenericStrikeResponseKnockdownRecoverBase* action, int edx_unused, unsigned int flags)
{
    CCombatAction_GenericStrikeResponseKnockdownRecoverBase_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}