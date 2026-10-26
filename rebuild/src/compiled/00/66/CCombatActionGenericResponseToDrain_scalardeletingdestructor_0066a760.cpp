class CCombatAction_GenericResponseToDrain { public:
    void* vftable;
};

void __fastcall CCombatAction_GenericResponseToDrain_dtor(CCombatAction_GenericResponseToDrain* action);
void operator delete(void*);

void* __fastcall CCombatAction_GenericResponseToDrain_scalar_deleting_destructor(CCombatAction_GenericResponseToDrain* action, int edx_unused, unsigned int flags)
{
    CCombatAction_GenericResponseToDrain_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}