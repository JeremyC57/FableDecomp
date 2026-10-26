class CCombatAction_GenericResponseToDecapitation { public:
    void* vftable;
};

void __fastcall CCombatAction_GenericResponseToDecapitation_dtor(CCombatAction_GenericResponseToDecapitation* action);
void operator delete(void*);

void* __fastcall CCombatAction_GenericResponseToDecapitation_scalar_deleting_destructor(CCombatAction_GenericResponseToDecapitation* action, int edx_unused, unsigned int flags)
{
    CCombatAction_GenericResponseToDecapitation_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}