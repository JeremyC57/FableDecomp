class CCreatureAction_CastDivineWrathSpell { public:
    void* vftable;
};

void __fastcall CCreatureAction_CastDivineWrathSpell_dtor(CCreatureAction_CastDivineWrathSpell* action);
void operator delete(void*);

void* __fastcall CCreatureAction_CastDivineWrathSpell_scalar_deleting_destructor(CCreatureAction_CastDivineWrathSpell* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_CastDivineWrathSpell_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}