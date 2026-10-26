class CCreatureAction_BuildUpTimeSpell { public:
    void* vftable;
};

void __fastcall CCreatureAction_BuildUpTimeSpell_dtor(CCreatureAction_BuildUpTimeSpell* action);
void operator delete(void*);

void* __fastcall CCreatureAction_BuildUpTimeSpell_scalar_deleting_destructor(CCreatureAction_BuildUpTimeSpell* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_BuildUpTimeSpell_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}