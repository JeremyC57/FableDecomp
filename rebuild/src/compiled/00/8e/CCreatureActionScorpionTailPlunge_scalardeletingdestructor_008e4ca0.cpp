struct CCreatureAction_ScorpionTailPlunge {
    void* vftable;
};

void __fastcall CCreatureAction_ScorpionTailPlunge_dtor(CCreatureAction_ScorpionTailPlunge* action);
void operator delete(void*);

void* __fastcall CCreatureAction_ScorpionTailPlunge_scalar_deleting_destructor(CCreatureAction_ScorpionTailPlunge* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_ScorpionTailPlunge_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}