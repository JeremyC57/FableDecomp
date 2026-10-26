class CCreatureAction_PlayAnimationFromIndex { public:
    void* vftable;
};

void __fastcall CCreatureAction_PlayAnimationFromIndex_dtor(CCreatureAction_PlayAnimationFromIndex* action);
void operator delete(void*);

void* __fastcall CCreatureAction_PlayAnimationFromIndex_scalar_deleting_destructor(CCreatureAction_PlayAnimationFromIndex* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_PlayAnimationFromIndex_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}