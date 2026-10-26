class CCombatAction_StrikeResponseJackOfBladesIntoRush { public:
    void* vftable;
};

void __fastcall CCombatAction_StrikeResponseJackOfBladesIntoRush_dtor(CCombatAction_StrikeResponseJackOfBladesIntoRush* action);
void operator delete(void*);

void* __fastcall CCombatAction_StrikeResponseJackOfBladesIntoRush_scalar_deleting_destructor(CCombatAction_StrikeResponseJackOfBladesIntoRush* action, int edx_unused, unsigned int flags)
{
    CCombatAction_StrikeResponseJackOfBladesIntoRush_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}