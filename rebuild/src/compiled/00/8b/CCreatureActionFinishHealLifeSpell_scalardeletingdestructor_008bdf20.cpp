class CCreatureAction_FinishHealLifeSpell { public:
    void* vftable;
};

void __fastcall CCreatureAction_FinishHealLifeSpell_dtor(CCreatureAction_FinishHealLifeSpell* action);
void operator delete(void*);

void* __fastcall CCreatureAction_FinishHealLifeSpell_scalar_deleting_destructor(CCreatureAction_FinishHealLifeSpell* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_FinishHealLifeSpell_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}