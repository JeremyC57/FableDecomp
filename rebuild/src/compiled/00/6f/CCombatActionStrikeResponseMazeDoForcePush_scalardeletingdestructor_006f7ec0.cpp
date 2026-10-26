class CCombatAction_StrikeResponseMazeDoForcePush { public:
    void* vftable;
};

void __fastcall CCombatAction_StrikeResponseMazeDoForcePush_dtor(CCombatAction_StrikeResponseMazeDoForcePush* action);
void operator delete(void*);

void* __fastcall CCombatAction_StrikeResponseMazeDoForcePush_scalar_deleting_destructor(CCombatAction_StrikeResponseMazeDoForcePush* action, int edx_unused, unsigned int flags)
{
    CCombatAction_StrikeResponseMazeDoForcePush_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}