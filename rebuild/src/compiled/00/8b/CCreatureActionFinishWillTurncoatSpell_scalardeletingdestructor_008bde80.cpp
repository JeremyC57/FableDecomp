class CCreatureAction_FinishWillTurncoatSpell { public:
    void* vftable;
};

void __fastcall CCreatureAction_FinishWillTurncoatSpell_dtor(CCreatureAction_FinishWillTurncoatSpell* action);
void operator delete(void*);

void* __fastcall CCreatureAction_FinishWillTurncoatSpell_scalar_deleting_destructor(CCreatureAction_FinishWillTurncoatSpell* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_FinishWillTurncoatSpell_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}