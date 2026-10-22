struct CCreatureAction_CallSpouseHome {
    void* vftable;
};

void __fastcall CCreatureAction_CallSpouseHome_dtor(CCreatureAction_CallSpouseHome* action);
void operator delete(void*);

void* __fastcall CCreatureAction_CallSpouseHome_scalar_deleting_destructor(CCreatureAction_CallSpouseHome* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_CallSpouseHome_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}