struct CCreatureAction_PlayHopscotch {
    void* vftable;
};

void __fastcall CCreatureAction_PlayHopscotch_dtor(CCreatureAction_PlayHopscotch* action);
void operator delete(void*);

void* __fastcall CCreatureAction_PlayHopscotch_scalar_deleting_destructor(CCreatureAction_PlayHopscotch* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_PlayHopscotch_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}