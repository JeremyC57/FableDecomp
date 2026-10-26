class CCreatureAction_GoUnderground { public:
    void* vftable;
};

void __fastcall CCreatureAction_GoUnderground_dtor(CCreatureAction_GoUnderground* action);
void operator delete(void*);

void* __fastcall CCreatureAction_GoUnderground_scalar_deleting_destructor(CCreatureAction_GoUnderground* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_GoUnderground_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}