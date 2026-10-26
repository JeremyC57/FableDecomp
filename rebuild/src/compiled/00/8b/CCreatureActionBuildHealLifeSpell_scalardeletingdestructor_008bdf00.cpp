class CCreatureAction_BuildHealLifeSpell { public:
    void* vftable;
};

void __fastcall CCreatureAction_BuildHealLifeSpell_dtor(CCreatureAction_BuildHealLifeSpell* action);
void operator delete(void*);

void* __fastcall CCreatureAction_BuildHealLifeSpell_scalar_deleting_destructor(CCreatureAction_BuildHealLifeSpell* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_BuildHealLifeSpell_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}