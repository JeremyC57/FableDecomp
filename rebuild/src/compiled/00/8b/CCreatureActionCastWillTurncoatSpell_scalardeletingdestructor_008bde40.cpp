class CCreatureAction_CastWillTurncoatSpell { public:
    void* vftable;
};

void __fastcall CCreatureAction_CastWillTurncoatSpell_dtor(CCreatureAction_CastWillTurncoatSpell* action);
void operator delete(void*);

void* __fastcall CCreatureAction_CastWillTurncoatSpell_scalar_deleting_destructor(CCreatureAction_CastWillTurncoatSpell* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_CastWillTurncoatSpell_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}