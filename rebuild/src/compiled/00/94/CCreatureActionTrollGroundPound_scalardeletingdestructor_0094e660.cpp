struct CCreatureAction_TrollGroundPound {
    void* vftable;
};

void __fastcall CCreatureAction_TrollGroundPound_dtor(CCreatureAction_TrollGroundPound* action);
void operator delete(void*);

void* __fastcall CCreatureAction_TrollGroundPound_scalar_deleting_destructor(CCreatureAction_TrollGroundPound* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_TrollGroundPound_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}