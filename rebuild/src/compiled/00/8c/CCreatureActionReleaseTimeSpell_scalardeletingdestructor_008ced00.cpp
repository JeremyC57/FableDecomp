class CCreatureAction_ReleaseTimeSpell { public:
    void* vftable;
};

void __fastcall CCreatureAction_ReleaseTimeSpell_dtor(CCreatureAction_ReleaseTimeSpell* action);
void operator delete(void*);

void* __fastcall CCreatureAction_ReleaseTimeSpell_scalar_deleting_destructor(CCreatureAction_ReleaseTimeSpell* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_ReleaseTimeSpell_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}