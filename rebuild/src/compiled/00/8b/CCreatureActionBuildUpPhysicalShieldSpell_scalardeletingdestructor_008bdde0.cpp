struct CCreatureAction_BuildUpPhysicalShieldSpell {
    void* vftable;
};

void __fastcall CCreatureAction_BuildUpPhysicalShieldSpell_dtor(CCreatureAction_BuildUpPhysicalShieldSpell* action);
void operator delete(void*);

void* __fastcall CCreatureAction_BuildUpPhysicalShieldSpell_scalar_deleting_destructor(CCreatureAction_BuildUpPhysicalShieldSpell* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_BuildUpPhysicalShieldSpell_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}