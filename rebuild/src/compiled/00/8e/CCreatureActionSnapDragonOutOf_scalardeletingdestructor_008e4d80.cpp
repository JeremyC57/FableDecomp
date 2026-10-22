struct CCreatureAction_SnapDragonOutOf {
    void* vftable;
};

void __fastcall CCreatureAction_SnapDragonOutOf_dtor(CCreatureAction_SnapDragonOutOf* action);
void operator delete(void*);

void* __fastcall CCreatureAction_SnapDragonOutOf_scalar_deleting_destructor(CCreatureAction_SnapDragonOutOf* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_SnapDragonOutOf_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}