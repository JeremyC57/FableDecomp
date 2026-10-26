class CCreatureAction_CastPhysicalShieldSpell { public:
    void* vftable;
};

void __fastcall CCreatureAction_CastPhysicalShieldSpell_dtor(CCreatureAction_CastPhysicalShieldSpell* action);
void operator delete(void*);

void* __fastcall CCreatureAction_CastPhysicalShieldSpell_scalar_deleting_destructor(CCreatureAction_CastPhysicalShieldSpell* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_CastPhysicalShieldSpell_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}