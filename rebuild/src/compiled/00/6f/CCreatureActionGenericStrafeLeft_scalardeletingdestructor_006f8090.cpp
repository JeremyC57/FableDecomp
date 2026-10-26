class CCreatureAction_GenericStrafeLeft { public:
    void* vftable;
};

void __fastcall CCreatureAction_GenericStrafeLeft_dtor(CCreatureAction_GenericStrafeLeft* action);
void operator delete(void*);

void* __fastcall CCreatureAction_GenericStrafeLeft_scalar_deleting_destructor(CCreatureAction_GenericStrafeLeft* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_GenericStrafeLeft_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}