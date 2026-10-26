class CCreatureAction_BuildFireballSpell { public:
    void* vftable;
};

void __fastcall CCreatureAction_BuildFireballSpell_dtor(CCreatureAction_BuildFireballSpell* action);
void operator delete(void*);

void* __fastcall CCreatureAction_BuildFireballSpell_scalar_deleting_destructor(CCreatureAction_BuildFireballSpell* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_BuildFireballSpell_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}