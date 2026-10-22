struct CCreatureAction_CastStrengthBerserk {
    void* vftable;
};

void __fastcall CCreatureAction_CastStrengthBerserk_dtor(CCreatureAction_CastStrengthBerserk* action);
void operator delete(void*);

void* __fastcall CCreatureAction_CastStrengthBerserk_scalar_deleting_destructor(CCreatureAction_CastStrengthBerserk* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_CastStrengthBerserk_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}