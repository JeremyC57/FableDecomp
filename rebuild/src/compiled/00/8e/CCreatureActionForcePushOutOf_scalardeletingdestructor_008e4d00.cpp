class CCreatureAction_ForcePushOutOf { public:
    void* vftable;
};

void __fastcall CCreatureAction_ForcePushOutOf_dtor(CCreatureAction_ForcePushOutOf* action);
void operator delete(void*);

void* __fastcall CCreatureAction_ForcePushOutOf_scalar_deleting_destructor(CCreatureAction_ForcePushOutOf* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_ForcePushOutOf_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}