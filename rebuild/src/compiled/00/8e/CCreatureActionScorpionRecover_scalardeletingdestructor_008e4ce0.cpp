struct CCreatureAction_ScorpionRecover {
    void* vftable;
};

void __fastcall CCreatureAction_ScorpionRecover_dtor(CCreatureAction_ScorpionRecover* action);
void operator delete(void*);

void* __fastcall CCreatureAction_ScorpionRecover_scalar_deleting_destructor(CCreatureAction_ScorpionRecover* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_ScorpionRecover_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}