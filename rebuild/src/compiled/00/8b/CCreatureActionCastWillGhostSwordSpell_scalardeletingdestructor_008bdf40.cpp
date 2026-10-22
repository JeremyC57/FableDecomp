struct CCreatureAction_CastWillGhostSwordSpell {
    void* vftable;
};

void __fastcall CCreatureAction_CastWillGhostSwordSpell_dtor(CCreatureAction_CastWillGhostSwordSpell* action);
void operator delete(void*);

void* __fastcall CCreatureAction_CastWillGhostSwordSpell_scalar_deleting_destructor(CCreatureAction_CastWillGhostSwordSpell* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_CastWillGhostSwordSpell_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}