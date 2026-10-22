struct CCreatureAction_OrderSecuritySweep {
    void* vftable;
};

void __fastcall CCreatureAction_OrderSecuritySweep_dtor(CCreatureAction_OrderSecuritySweep* action);
void operator delete(void*);

void* __fastcall CCreatureAction_OrderSecuritySweep_scalar_deleting_destructor(CCreatureAction_OrderSecuritySweep* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_OrderSecuritySweep_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}