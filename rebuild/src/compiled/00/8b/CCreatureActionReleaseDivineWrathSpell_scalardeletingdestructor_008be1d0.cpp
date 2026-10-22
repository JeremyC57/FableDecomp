struct CCreatureAction_ReleaseDivineWrathSpell {
    void* vftable;
};

void __fastcall CCreatureAction_ReleaseDivineWrathSpell_dtor(CCreatureAction_ReleaseDivineWrathSpell* action);
void operator delete(void*);

void* __fastcall CCreatureAction_ReleaseDivineWrathSpell_scalar_deleting_destructor(CCreatureAction_ReleaseDivineWrathSpell* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_ReleaseDivineWrathSpell_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}