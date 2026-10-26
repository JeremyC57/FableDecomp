class CCreatureAction_BuildDrainLifeSpell { public:
    void* vftable;
};

void __fastcall CCreatureAction_BuildDrainLifeSpell_dtor(CCreatureAction_BuildDrainLifeSpell* action);
void operator delete(void*);

void* __fastcall CCreatureAction_BuildDrainLifeSpell_scalar_deleting_destructor(CCreatureAction_BuildDrainLifeSpell* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_BuildDrainLifeSpell_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}