struct CCreatureAction_ReleaseStrengthBerserk {
    void* vftable;
};

void __fastcall CCreatureAction_ReleaseStrengthBerserk_dtor(CCreatureAction_ReleaseStrengthBerserk* action);
void operator delete(void*);

void* __fastcall CCreatureAction_ReleaseStrengthBerserk_scalar_deleting_destructor(CCreatureAction_ReleaseStrengthBerserk* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_ReleaseStrengthBerserk_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}