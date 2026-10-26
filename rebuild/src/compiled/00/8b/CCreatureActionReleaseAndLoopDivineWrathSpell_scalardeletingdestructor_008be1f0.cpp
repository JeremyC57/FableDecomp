class CCreatureAction_ReleaseAndLoopDivineWrathSpell { public:
    void* vftable;
};

void __fastcall CCreatureAction_ReleaseAndLoopDivineWrathSpell_dtor(CCreatureAction_ReleaseAndLoopDivineWrathSpell* action);
void operator delete(void*);

void* __fastcall CCreatureAction_ReleaseAndLoopDivineWrathSpell_scalar_deleting_destructor(CCreatureAction_ReleaseAndLoopDivineWrathSpell* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_ReleaseAndLoopDivineWrathSpell_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}