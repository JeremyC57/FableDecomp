class CCreatureAction_PlayAnimation { public:
    void* vftable;
};

void __fastcall CCreatureAction_PlayAnimation_dtor(CCreatureAction_PlayAnimation* action);
void operator delete(void*);

void* __fastcall CCreatureAction_PlayAnimation_scalar_deleting_destructor(CCreatureAction_PlayAnimation* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_PlayAnimation_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}