class CCreatureAction_TurnStrikeRight { public:
    void* vftable;
};

void __fastcall CCreatureAction_TurnStrikeRight_dtor(CCreatureAction_TurnStrikeRight* action);
void operator delete(void*);

void* __fastcall CCreatureAction_TurnStrikeRight_scalar_deleting_destructor(CCreatureAction_TurnStrikeRight* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_TurnStrikeRight_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}