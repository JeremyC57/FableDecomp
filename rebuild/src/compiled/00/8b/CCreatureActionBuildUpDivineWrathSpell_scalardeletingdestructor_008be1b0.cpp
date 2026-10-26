class CCreatureAction_BuildUpDivineWrathSpell { public:
    void* vftable;
};

void __fastcall CCreatureAction_BuildUpDivineWrathSpell_dtor(CCreatureAction_BuildUpDivineWrathSpell* action);
void operator delete(void*);

void* __fastcall CCreatureAction_BuildUpDivineWrathSpell_scalar_deleting_destructor(CCreatureAction_BuildUpDivineWrathSpell* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_BuildUpDivineWrathSpell_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}