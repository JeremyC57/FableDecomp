class CCreatureAction_JackFlash { public:
    void* vftable;
};

void __fastcall CCreatureAction_JackFlash_dtor(CCreatureAction_JackFlash* action);
void operator delete(void*);

void* __fastcall CCreatureAction_JackFlash_scalar_deleting_destructor(CCreatureAction_JackFlash* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_JackFlash_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}