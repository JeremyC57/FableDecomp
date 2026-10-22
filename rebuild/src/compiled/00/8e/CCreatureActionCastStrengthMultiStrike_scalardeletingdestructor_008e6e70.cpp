struct CCreatureAction_CastStrengthMultiStrike {
    void* vftable;
};

void __fastcall CCreatureAction_CastStrengthMultiStrike_dtor(CCreatureAction_CastStrengthMultiStrike* action);
void operator delete(void*);

void* __fastcall CCreatureAction_CastStrengthMultiStrike_scalar_deleting_destructor(CCreatureAction_CastStrengthMultiStrike* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_CastStrengthMultiStrike_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}