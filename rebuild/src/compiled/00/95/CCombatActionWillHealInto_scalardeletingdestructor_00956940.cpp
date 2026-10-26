class CCombatAction_WillHealInto { public:
    void* vftable;
};

void __fastcall CCombatAction_WillHealInto_dtor(CCombatAction_WillHealInto* action);
void operator delete(void*);

void* __fastcall CCombatAction_WillHealInto_scalar_deleting_destructor(CCombatAction_WillHealInto* action, int edx_unused, unsigned int flags)
{
    CCombatAction_WillHealInto_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}