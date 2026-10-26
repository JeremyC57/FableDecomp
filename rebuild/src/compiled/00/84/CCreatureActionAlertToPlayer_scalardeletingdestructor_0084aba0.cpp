class CCreatureAction_AlertToPlayer { public:
    void* vftable;
};

void __fastcall CCreatureAction_AlertToPlayer_dtor(CCreatureAction_AlertToPlayer* action);
void operator delete(void*);

void* __fastcall CCreatureAction_AlertToPlayer_scalar_deleting_destructor(CCreatureAction_AlertToPlayer* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_AlertToPlayer_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}