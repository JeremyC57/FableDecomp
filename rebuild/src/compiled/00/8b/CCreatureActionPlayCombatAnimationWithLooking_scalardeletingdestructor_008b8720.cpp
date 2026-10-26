class CCreatureAction_PlayCombatAnimationWithLooking { public:
    void* vftable;
};

void __fastcall CCreatureAction_PlayCombatAnimationWithLooking_dtor(CCreatureAction_PlayCombatAnimationWithLooking* action);
void operator delete(void*);

void* __fastcall CCreatureAction_PlayCombatAnimationWithLooking_scalar_deleting_destructor(CCreatureAction_PlayCombatAnimationWithLooking* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_PlayCombatAnimationWithLooking_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}