struct CCreatureAction_ScorpionKingDeath {
    void* vftable;
};

void __fastcall CCreatureAction_ScorpionKingDeath_dtor(CCreatureAction_ScorpionKingDeath* action);
void operator delete(void*);

void* __fastcall CCreatureAction_ScorpionKingDeath_scalar_deleting_destructor(CCreatureAction_ScorpionKingDeath* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_ScorpionKingDeath_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}