class CCombatAction_GenericStrikeResponse { public:
    void* vftable;
};

void __fastcall CCombatAction_GenericStrikeResponse_dtor(CCombatAction_GenericStrikeResponse* action);
void operator delete(void*);

void* __fastcall CCombatAction_GenericStrikeResponse_scalar_deleting_destructor(CCombatAction_GenericStrikeResponse* action, int edx_unused, unsigned int flags)
{
    CCombatAction_GenericStrikeResponse_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}