class CCreatureAction_CastWillRaiseDeadSpell { public:
    void* vftable;
};

void __fastcall CCreatureAction_CastWillRaiseDeadSpell_dtor(CCreatureAction_CastWillRaiseDeadSpell* action);
void operator delete(void*);

void* __fastcall CCreatureAction_CastWillRaiseDeadSpell_scalar_deleting_destructor(CCreatureAction_CastWillRaiseDeadSpell* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_CastWillRaiseDeadSpell_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}