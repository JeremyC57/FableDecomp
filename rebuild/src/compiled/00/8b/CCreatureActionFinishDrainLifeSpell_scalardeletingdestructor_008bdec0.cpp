class CCreatureAction_FinishDrainLifeSpell { public:
    void* vftable;
};

void __fastcall CCreatureAction_FinishDrainLifeSpell_dtor(CCreatureAction_FinishDrainLifeSpell* action);
void operator delete(void*);

void* __fastcall CCreatureAction_FinishDrainLifeSpell_scalar_deleting_destructor(CCreatureAction_FinishDrainLifeSpell* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_FinishDrainLifeSpell_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}