class CCombatAction_GenericBreakOffDrain { public:
    void* vftable;
};

void __fastcall CCombatAction_GenericBreakOffDrain_dtor(CCombatAction_GenericBreakOffDrain* action);
void operator delete(void*);

void* __fastcall CCombatAction_GenericBreakOffDrain_scalar_deleting_destructor(CCombatAction_GenericBreakOffDrain* action, int edx_unused, unsigned int flags)
{
    CCombatAction_GenericBreakOffDrain_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}