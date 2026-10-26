class CCreatureAction_CastWillEnflameSpell { public:
    void* vftable;
};

void __fastcall CCreatureAction_CastWillEnflameSpell_dtor(CCreatureAction_CastWillEnflameSpell* action);
void operator delete(void*);

void* __fastcall CCreatureAction_CastWillEnflameSpell_scalar_deleting_destructor(CCreatureAction_CastWillEnflameSpell* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_CastWillEnflameSpell_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}