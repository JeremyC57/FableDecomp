struct CCreatureAction_ScorpionSummon {
    void* vftable;
};

void __fastcall CCreatureAction_ScorpionSummon_dtor(CCreatureAction_ScorpionSummon* action);
void operator delete(void*);

void* __fastcall CCreatureAction_ScorpionSummon_scalar_deleting_destructor(CCreatureAction_ScorpionSummon* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_ScorpionSummon_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}