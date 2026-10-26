class CCreatureAction_RemovePhysicalShieldSpell { public:
    void* vftable;
};

void __fastcall CCreatureAction_RemovePhysicalShieldSpell_dtor(CCreatureAction_RemovePhysicalShieldSpell* action);
void operator delete(void*);

void* __fastcall CCreatureAction_RemovePhysicalShieldSpell_scalar_deleting_destructor(CCreatureAction_RemovePhysicalShieldSpell* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_RemovePhysicalShieldSpell_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}