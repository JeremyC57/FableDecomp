struct CCreatureAction_ScorpionTailWithdraw {
    void* vftable;
};

void __fastcall CCreatureAction_ScorpionTailWithdraw_dtor(CCreatureAction_ScorpionTailWithdraw* action);
void operator delete(void*);

void* __fastcall CCreatureAction_ScorpionTailWithdraw_scalar_deleting_destructor(CCreatureAction_ScorpionTailWithdraw* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_ScorpionTailWithdraw_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}