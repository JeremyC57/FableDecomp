struct CCreatureAction_TrollPursue {
    void* vftable;
};

void __fastcall CCreatureAction_TrollPursue_dtor(CCreatureAction_TrollPursue* action);
void operator delete(void*);

void* __fastcall CCreatureAction_TrollPursue_scalar_deleting_destructor(CCreatureAction_TrollPursue* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_TrollPursue_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}