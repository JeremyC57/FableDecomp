class CCreatureAction_FinishFireballSpell { public:
    void* vftable;
};

void __fastcall CCreatureAction_FinishFireballSpell_dtor(CCreatureAction_FinishFireballSpell* action);
void operator delete(void*);

void* __fastcall CCreatureAction_FinishFireballSpell_scalar_deleting_destructor(CCreatureAction_FinishFireballSpell* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_FinishFireballSpell_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}