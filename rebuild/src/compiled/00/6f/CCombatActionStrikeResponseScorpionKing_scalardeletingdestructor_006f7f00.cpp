class CCombatAction_StrikeResponseScorpionKing { public:
    void* vftable;
};

void __fastcall CCombatAction_StrikeResponseScorpionKing_dtor(CCombatAction_StrikeResponseScorpionKing* action);
void operator delete(void*);

void* __fastcall CCombatAction_StrikeResponseScorpionKing_scalar_deleting_destructor(CCombatAction_StrikeResponseScorpionKing* action, int edx_unused, unsigned int flags)
{
    CCombatAction_StrikeResponseScorpionKing_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}