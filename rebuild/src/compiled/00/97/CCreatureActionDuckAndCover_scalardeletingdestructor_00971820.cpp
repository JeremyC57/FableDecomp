struct CCreatureAction_DuckAndCover {
    void* vftable;
};

void __fastcall CCreatureAction_DuckAndCover_dtor(CCreatureAction_DuckAndCover* action);
void operator delete(void*);

void* __fastcall CCreatureAction_DuckAndCover_scalar_deleting_destructor(CCreatureAction_DuckAndCover* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_DuckAndCover_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}