struct CCreatureAction_JackDeathInto {
    void* vftable;
};

void __fastcall CCreatureAction_JackDeathInto_dtor(CCreatureAction_JackDeathInto* action);
void operator delete(void*);

void* __fastcall CCreatureAction_JackDeathInto_scalar_deleting_destructor(CCreatureAction_JackDeathInto* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_JackDeathInto_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}