struct CCreatureAction_PlayCombatAnimation {
    void* vftable;
};

void __fastcall CCreatureAction_PlayCombatAnimation_dtor(CCreatureAction_PlayCombatAnimation* action);
void operator delete(void*);

void* __fastcall CCreatureAction_PlayCombatAnimation_scalar_deleting_destructor(CCreatureAction_PlayCombatAnimation* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_PlayCombatAnimation_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}