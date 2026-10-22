struct CCreatureAction_JackShotFloat {
    void* vftable;
};

void __fastcall CCreatureAction_JackShotFloat_dtor(CCreatureAction_JackShotFloat* action);
void operator delete(void*);

void* __fastcall CCreatureAction_JackShotFloat_scalar_deleting_destructor(CCreatureAction_JackShotFloat* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_JackShotFloat_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}