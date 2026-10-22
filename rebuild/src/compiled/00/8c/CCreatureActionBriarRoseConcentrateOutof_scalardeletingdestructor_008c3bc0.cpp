struct CCreatureAction_BriarRoseConcentrateOutof {
    void* vftable;
};

void __fastcall CCreatureAction_BriarRoseConcentrateOutof_dtor(CCreatureAction_BriarRoseConcentrateOutof* action);
void operator delete(void*);

void* __fastcall CCreatureAction_BriarRoseConcentrateOutof_scalar_deleting_destructor(CCreatureAction_BriarRoseConcentrateOutof* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_BriarRoseConcentrateOutof_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}