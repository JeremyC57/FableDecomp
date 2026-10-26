class CCreatureAction_BuildUpWillLightningSpell { public:
    void* vftable;
};

void __fastcall CCreatureAction_BuildUpWillLightningSpell_dtor(CCreatureAction_BuildUpWillLightningSpell* action);
void operator delete(void*);

void* __fastcall CCreatureAction_BuildUpWillLightningSpell_scalar_deleting_destructor(CCreatureAction_BuildUpWillLightningSpell* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_BuildUpWillLightningSpell_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}