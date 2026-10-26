class CCreatureAction_ReleaseWillEnflameSpell { public:
    void* vftable;
};

void __fastcall CCreatureAction_ReleaseWillEnflameSpell_dtor(CCreatureAction_ReleaseWillEnflameSpell* action);
void operator delete(void*);

void* __fastcall CCreatureAction_ReleaseWillEnflameSpell_scalar_deleting_destructor(CCreatureAction_ReleaseWillEnflameSpell* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_ReleaseWillEnflameSpell_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}