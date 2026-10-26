class CCreatureAction_CastWillSummonSpell { public:
    void* vftable;
};

void __fastcall CCreatureAction_CastWillSummonSpell_dtor(CCreatureAction_CastWillSummonSpell* action);
void operator delete(void*);

void* __fastcall CCreatureAction_CastWillSummonSpell_scalar_deleting_destructor(CCreatureAction_CastWillSummonSpell* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_CastWillSummonSpell_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}