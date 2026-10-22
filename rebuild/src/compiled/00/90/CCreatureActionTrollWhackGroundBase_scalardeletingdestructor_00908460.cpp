struct CCreatureAction_TrollWhackGroundBase {
    void* vftable;
};

void __fastcall CCreatureAction_TrollWhackGroundBase_dtor(CCreatureAction_TrollWhackGroundBase* action);
void operator delete(void*);

void* __fastcall CCreatureAction_TrollWhackGroundBase_scalar_deleting_destructor(CCreatureAction_TrollWhackGroundBase* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_TrollWhackGroundBase_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}