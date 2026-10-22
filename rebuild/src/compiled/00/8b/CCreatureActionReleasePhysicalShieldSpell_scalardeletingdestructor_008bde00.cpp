struct CCreatureAction_ReleasePhysicalShieldSpell {
    void* vftable;
};

void __fastcall CCreatureAction_ReleasePhysicalShieldSpell_dtor(CCreatureAction_ReleasePhysicalShieldSpell* action);
void operator delete(void*);

void* __fastcall CCreatureAction_ReleasePhysicalShieldSpell_scalar_deleting_destructor(CCreatureAction_ReleasePhysicalShieldSpell* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_ReleasePhysicalShieldSpell_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}