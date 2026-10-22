struct CCreatureAction_JackShotFar {
    void* vftable;
};

void __fastcall CCreatureAction_JackShotFar_dtor(CCreatureAction_JackShotFar* action);
void operator delete(void*);

void* __fastcall CCreatureAction_JackShotFar_scalar_deleting_destructor(CCreatureAction_JackShotFar* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_JackShotFar_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}