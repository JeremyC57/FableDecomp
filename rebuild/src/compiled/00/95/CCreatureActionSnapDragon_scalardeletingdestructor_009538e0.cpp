struct CCreatureAction_SnapDragon {
    void* vftable;
};

void __fastcall CCreatureAction_SnapDragon_dtor(CCreatureAction_SnapDragon* action);
void operator delete(void*);

void* __fastcall CCreatureAction_SnapDragon_scalar_deleting_destructor(CCreatureAction_SnapDragon* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_SnapDragon_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}