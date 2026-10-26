class CCreatureAction_ReleaseUnholyPowerSpell { public:
    void* vftable;
};

void __fastcall CCreatureAction_ReleaseUnholyPowerSpell_dtor(CCreatureAction_ReleaseUnholyPowerSpell* action);
void operator delete(void*);

void* __fastcall CCreatureAction_ReleaseUnholyPowerSpell_scalar_deleting_destructor(CCreatureAction_ReleaseUnholyPowerSpell* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_ReleaseUnholyPowerSpell_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}