class CCreatureAction_BeetleTakeOff { public:
    void* vftable;
};

void __fastcall CCreatureAction_BeetleTakeOff_dtor(CCreatureAction_BeetleTakeOff* action);
void operator delete(void*);

void* __fastcall CCreatureAction_BeetleTakeOff_scalar_deleting_destructor(CCreatureAction_BeetleTakeOff* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_BeetleTakeOff_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}