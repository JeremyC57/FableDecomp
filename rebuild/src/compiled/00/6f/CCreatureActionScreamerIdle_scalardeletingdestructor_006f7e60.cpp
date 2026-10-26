class CCreatureAction_ScreamerIdle { public:
    void* vftable;
};

void __fastcall CCreatureAction_ScreamerIdle_dtor(CCreatureAction_ScreamerIdle* action);
void operator delete(void*);

void* __fastcall CCreatureAction_ScreamerIdle_scalar_deleting_destructor(CCreatureAction_ScreamerIdle* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_ScreamerIdle_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}