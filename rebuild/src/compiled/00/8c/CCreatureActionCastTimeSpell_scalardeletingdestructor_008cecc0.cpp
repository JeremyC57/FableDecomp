struct CCreatureAction_CastTimeSpell {
    void* vftable;
};

void __fastcall CCreatureAction_CastTimeSpell_dtor(CCreatureAction_CastTimeSpell* action);
void operator delete(void*);

void* __fastcall CCreatureAction_CastTimeSpell_scalar_deleting_destructor(CCreatureAction_CastTimeSpell* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_CastTimeSpell_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}