class CCreatureAction_BeDazed { public:
    void* vftable;
};

void __fastcall CCreatureAction_BeDazed_dtor(CCreatureAction_BeDazed* action);
void operator delete(void*);

void* __fastcall CCreatureAction_BeDazed_scalar_deleting_destructor(CCreatureAction_BeDazed* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_BeDazed_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}