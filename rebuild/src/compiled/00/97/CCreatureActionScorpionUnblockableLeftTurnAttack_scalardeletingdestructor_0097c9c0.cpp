class CCreatureAction_ScorpionUnblockableLeftTurnAttack { public:
    void* vftable;
};

void __fastcall CCreatureAction_ScorpionUnblockableLeftTurnAttack_dtor(CCreatureAction_ScorpionUnblockableLeftTurnAttack* action);
void operator delete(void*);

void* __fastcall CCreatureAction_ScorpionUnblockableLeftTurnAttack_scalar_deleting_destructor(CCreatureAction_ScorpionUnblockableLeftTurnAttack* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_ScorpionUnblockableLeftTurnAttack_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}