struct CCreatureAction_JumpOutFromCover {
    void* vftable;
};

void __fastcall CCreatureAction_JumpOutFromCover_dtor(CCreatureAction_JumpOutFromCover* action);
void operator delete(void*);

void* __fastcall CCreatureAction_JumpOutFromCover_scalar_deleting_destructor(CCreatureAction_JumpOutFromCover* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_JumpOutFromCover_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}