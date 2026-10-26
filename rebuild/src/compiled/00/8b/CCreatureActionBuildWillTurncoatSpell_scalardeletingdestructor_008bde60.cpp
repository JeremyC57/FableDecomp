class CCreatureAction_BuildWillTurncoatSpell { public:
    void* vftable;
};

void __fastcall CCreatureAction_BuildWillTurncoatSpell_dtor(CCreatureAction_BuildWillTurncoatSpell* action);
void operator delete(void*);

void* __fastcall CCreatureAction_BuildWillTurncoatSpell_scalar_deleting_destructor(CCreatureAction_BuildWillTurncoatSpell* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_BuildWillTurncoatSpell_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}