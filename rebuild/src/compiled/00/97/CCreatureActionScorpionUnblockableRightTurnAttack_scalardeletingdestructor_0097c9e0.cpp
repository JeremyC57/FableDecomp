class CCreatureAction_ScorpionUnblockableRightTurnAttack { public:
    void* vftable;
};

void __fastcall CCreatureAction_ScorpionUnblockableRightTurnAttack_dtor(CCreatureAction_ScorpionUnblockableRightTurnAttack* action);
void operator delete(void*);

void* __fastcall CCreatureAction_ScorpionUnblockableRightTurnAttack_scalar_deleting_destructor(CCreatureAction_ScorpionUnblockableRightTurnAttack* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_ScorpionUnblockableRightTurnAttack_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}