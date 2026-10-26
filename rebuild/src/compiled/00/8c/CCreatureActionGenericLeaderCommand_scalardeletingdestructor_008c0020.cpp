class CCreatureAction_GenericLeaderCommand { public:
    void* vftable;
};

void __fastcall CCreatureAction_GenericLeaderCommand_dtor(CCreatureAction_GenericLeaderCommand* action);
void operator delete(void*);

void* __fastcall CCreatureAction_GenericLeaderCommand_scalar_deleting_destructor(CCreatureAction_GenericLeaderCommand* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_GenericLeaderCommand_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}