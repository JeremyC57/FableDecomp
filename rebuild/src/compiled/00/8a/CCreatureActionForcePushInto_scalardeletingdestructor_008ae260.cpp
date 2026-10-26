class CCreatureAction_ForcePushInto { public:
    void* vftable;
};

void __fastcall CCreatureAction_ForcePushInto_dtor(CCreatureAction_ForcePushInto* action);
void operator delete(void*);

void* __fastcall CCreatureAction_ForcePushInto_scalar_deleting_destructor(CCreatureAction_ForcePushInto* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_ForcePushInto_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}