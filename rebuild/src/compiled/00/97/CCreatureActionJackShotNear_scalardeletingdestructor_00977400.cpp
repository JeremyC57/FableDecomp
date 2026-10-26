class CCreatureAction_JackShotNear { public:
    void* vftable;
};

void __fastcall CCreatureAction_JackShotNear_dtor(CCreatureAction_JackShotNear* action);
void operator delete(void*);

void* __fastcall CCreatureAction_JackShotNear_scalar_deleting_destructor(CCreatureAction_JackShotNear* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_JackShotNear_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}