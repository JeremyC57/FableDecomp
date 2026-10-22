struct CCreatureAction_NymphIdleAfterShot {
    void* vftable;
};

void __fastcall CCreatureAction_NymphIdleAfterShot_dtor(CCreatureAction_NymphIdleAfterShot* action);
void operator delete(void*);

void* __fastcall CCreatureAction_NymphIdleAfterShot_scalar_deleting_destructor(CCreatureAction_NymphIdleAfterShot* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_NymphIdleAfterShot_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}