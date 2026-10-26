class CCreatureAction_CastWillFireballSpell { public:
    void* vftable;
};

void __fastcall CCreatureAction_CastWillFireballSpell_dtor(CCreatureAction_CastWillFireballSpell* action);
void operator delete(void*);

void* __fastcall CCreatureAction_CastWillFireballSpell_scalar_deleting_destructor(CCreatureAction_CastWillFireballSpell* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_CastWillFireballSpell_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}