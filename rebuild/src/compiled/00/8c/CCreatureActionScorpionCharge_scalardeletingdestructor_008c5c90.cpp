struct CCreatureAction_ScorpionCharge {
    void* vftable;
};

void __fastcall CCreatureAction_ScorpionCharge_dtor(CCreatureAction_ScorpionCharge* action);
void operator delete(void*);

void* __fastcall CCreatureAction_ScorpionCharge_scalar_deleting_destructor(CCreatureAction_ScorpionCharge* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_ScorpionCharge_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}