class CCreatureAction_FinishGhostSwordSpell { public:
    void* vftable;
};

void __fastcall CCreatureAction_FinishGhostSwordSpell_dtor(CCreatureAction_FinishGhostSwordSpell* action);
void operator delete(void*);

void* __fastcall CCreatureAction_FinishGhostSwordSpell_scalar_deleting_destructor(CCreatureAction_FinishGhostSwordSpell* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_FinishGhostSwordSpell_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}