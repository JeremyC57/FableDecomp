class CCreatureAction_CastWillLightningSpell { public:
    void* vftable;
};

void __fastcall CCreatureAction_CastWillLightningSpell_dtor(CCreatureAction_CastWillLightningSpell* action);
void operator delete(void*);

void* __fastcall CCreatureAction_CastWillLightningSpell_scalar_deleting_destructor(CCreatureAction_CastWillLightningSpell* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_CastWillLightningSpell_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}