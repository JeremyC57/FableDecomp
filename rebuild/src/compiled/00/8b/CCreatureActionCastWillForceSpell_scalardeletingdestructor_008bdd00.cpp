struct CCreatureAction_CastWillForceSpell {
    void* vftable;
};

void __fastcall CCreatureAction_CastWillForceSpell_dtor(CCreatureAction_CastWillForceSpell* action);
void operator delete(void*);

void* __fastcall CCreatureAction_CastWillForceSpell_scalar_deleting_destructor(CCreatureAction_CastWillForceSpell* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_CastWillForceSpell_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}