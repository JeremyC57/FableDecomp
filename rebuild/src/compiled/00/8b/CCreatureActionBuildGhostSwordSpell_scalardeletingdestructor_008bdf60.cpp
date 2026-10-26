class CCreatureAction_BuildGhostSwordSpell { public:
    void* vftable;
};

void __fastcall CCreatureAction_BuildGhostSwordSpell_dtor(CCreatureAction_BuildGhostSwordSpell* action);
void operator delete(void*);

void* __fastcall CCreatureAction_BuildGhostSwordSpell_scalar_deleting_destructor(CCreatureAction_BuildGhostSwordSpell* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_BuildGhostSwordSpell_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}