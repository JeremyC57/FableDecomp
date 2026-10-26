class CCreatureAction_BuildWillForceSpell { public:
    void* vftable;
};

void __fastcall CCreatureAction_BuildWillForceSpell_dtor(CCreatureAction_BuildWillForceSpell* action);
void operator delete(void*);

void* __fastcall CCreatureAction_BuildWillForceSpell_scalar_deleting_destructor(CCreatureAction_BuildWillForceSpell* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_BuildWillForceSpell_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}