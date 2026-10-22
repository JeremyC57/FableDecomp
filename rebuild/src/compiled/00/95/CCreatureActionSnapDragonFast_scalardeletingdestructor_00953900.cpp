struct CCreatureAction_SnapDragonFast {
    void* vftable;
};

void __fastcall CCreatureAction_SnapDragonFast_dtor(CCreatureAction_SnapDragonFast* action);
void operator delete(void*);

void* __fastcall CCreatureAction_SnapDragonFast_scalar_deleting_destructor(CCreatureAction_SnapDragonFast* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_SnapDragonFast_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}