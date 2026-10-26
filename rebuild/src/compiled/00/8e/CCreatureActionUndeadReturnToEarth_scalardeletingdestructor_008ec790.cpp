class CCreatureAction_UndeadReturnToEarth { public:
    void* vftable;
};

void __fastcall CCreatureAction_UndeadReturnToEarth_dtor(CCreatureAction_UndeadReturnToEarth* action);
void operator delete(void*);

void* __fastcall CCreatureAction_UndeadReturnToEarth_scalar_deleting_destructor(CCreatureAction_UndeadReturnToEarth* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_UndeadReturnToEarth_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}