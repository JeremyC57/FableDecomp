class CCombatAction_WillShieldInto { public:
    void* vftable;
};

void __fastcall CCombatAction_WillShieldInto_dtor(CCombatAction_WillShieldInto* action);
void operator delete(void*);

void* __fastcall CCombatAction_WillShieldInto_scalar_deleting_destructor(CCombatAction_WillShieldInto* action, int edx_unused, unsigned int flags)
{
    CCombatAction_WillShieldInto_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}