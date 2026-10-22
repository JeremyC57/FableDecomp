struct CCreatureAction_JackFloatUp {
    void* vftable;
};

void __fastcall CCreatureAction_JackFloatUp_dtor(CCreatureAction_JackFloatUp* action);
void operator delete(void*);

void* __fastcall CCreatureAction_JackFloatUp_scalar_deleting_destructor(CCreatureAction_JackFloatUp* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_JackFloatUp_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}