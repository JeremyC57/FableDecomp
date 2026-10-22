struct CCreatureAction_ScorpionChargeStrike {
    void* vftable;
};

void __fastcall CCreatureAction_ScorpionChargeStrike_dtor(CCreatureAction_ScorpionChargeStrike* action);
void operator delete(void*);

void* __fastcall CCreatureAction_ScorpionChargeStrike_scalar_deleting_destructor(CCreatureAction_ScorpionChargeStrike* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_ScorpionChargeStrike_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}