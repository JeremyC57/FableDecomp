struct CCreatureAction_TurnStrikeLeft {
    void* vftable;
};

void __fastcall CCreatureAction_TurnStrikeLeft_dtor(CCreatureAction_TurnStrikeLeft* action);
void operator delete(void*);

void* __fastcall CCreatureAction_TurnStrikeLeft_scalar_deleting_destructor(CCreatureAction_TurnStrikeLeft* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_TurnStrikeLeft_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}