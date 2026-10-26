class CCreatureAction_RiseUp { public:
    void* vftable;
};

void __fastcall CCreatureAction_RiseUp_dtor(CCreatureAction_RiseUp* action);
void operator delete(void*);

void* __fastcall CCreatureAction_RiseUp_scalar_deleting_destructor(CCreatureAction_RiseUp* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_RiseUp_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}