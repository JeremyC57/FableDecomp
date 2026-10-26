class CCreatureAction_ScorpionPrepareTailPlunge { public:
    void* vftable;
};

void __fastcall CCreatureAction_ScorpionPrepareTailPlunge_dtor(CCreatureAction_ScorpionPrepareTailPlunge* action);
void operator delete(void*);

void* __fastcall CCreatureAction_ScorpionPrepareTailPlunge_scalar_deleting_destructor(CCreatureAction_ScorpionPrepareTailPlunge* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_ScorpionPrepareTailPlunge_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}