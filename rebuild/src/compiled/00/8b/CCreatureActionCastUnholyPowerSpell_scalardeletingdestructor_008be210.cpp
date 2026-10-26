class CCreatureAction_CastUnholyPowerSpell { public:
    void* vftable;
};

void __fastcall CCreatureAction_CastUnholyPowerSpell_dtor(CCreatureAction_CastUnholyPowerSpell* action);
void operator delete(void*);

void* __fastcall CCreatureAction_CastUnholyPowerSpell_scalar_deleting_destructor(CCreatureAction_CastUnholyPowerSpell* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_CastUnholyPowerSpell_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}