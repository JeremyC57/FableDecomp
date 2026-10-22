struct CCreatureAction_TrollAngryGroundPound {
    void* vftable;
};

void __fastcall CCreatureAction_TrollAngryGroundPound_dtor(CCreatureAction_TrollAngryGroundPound* action);
void operator delete(void*);

void* __fastcall CCreatureAction_TrollAngryGroundPound_scalar_deleting_destructor(CCreatureAction_TrollAngryGroundPound* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_TrollAngryGroundPound_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}