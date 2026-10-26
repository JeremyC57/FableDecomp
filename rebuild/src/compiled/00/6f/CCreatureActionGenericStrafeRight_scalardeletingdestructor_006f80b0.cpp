class CCreatureAction_GenericStrafeRight { public:
    void* vftable;
};

void __fastcall CCreatureAction_GenericStrafeRight_dtor(CCreatureAction_GenericStrafeRight* action);
void operator delete(void*);

void* __fastcall CCreatureAction_GenericStrafeRight_scalar_deleting_destructor(CCreatureAction_GenericStrafeRight* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_GenericStrafeRight_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}