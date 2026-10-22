struct CCreatureAction_JackGetUp {
    void* vftable;
};

void __fastcall CCreatureAction_JackGetUp_dtor(CCreatureAction_JackGetUp* action);
void operator delete(void*);

void* __fastcall CCreatureAction_JackGetUp_scalar_deleting_destructor(CCreatureAction_JackGetUp* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_JackGetUp_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}