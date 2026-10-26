class CCreatureAction_CastWillDrainLifeSpell { public:
    void* vftable;
};

void __fastcall CCreatureAction_CastWillDrainLifeSpell_dtor(CCreatureAction_CastWillDrainLifeSpell* action);
void operator delete(void*);

void* __fastcall CCreatureAction_CastWillDrainLifeSpell_scalar_deleting_destructor(CCreatureAction_CastWillDrainLifeSpell* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_CastWillDrainLifeSpell_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}