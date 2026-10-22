struct CCreatureAction_CastStrengthMultiStrikeProper {
    void* vftable;
};

void __fastcall CCreatureAction_CastStrengthMultiStrikeProper_dtor(CCreatureAction_CastStrengthMultiStrikeProper* action);
void operator delete(void*);

void* __fastcall CCreatureAction_CastStrengthMultiStrikeProper_scalar_deleting_destructor(CCreatureAction_CastStrengthMultiStrikeProper* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_CastStrengthMultiStrikeProper_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}