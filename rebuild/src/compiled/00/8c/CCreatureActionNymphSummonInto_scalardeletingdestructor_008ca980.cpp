struct CCreatureAction_NymphSummonInto {
    void* vftable;
};

void __fastcall CCreatureAction_NymphSummonInto_dtor(CCreatureAction_NymphSummonInto* action);
void operator delete(void*);

void* __fastcall CCreatureAction_NymphSummonInto_scalar_deleting_destructor(CCreatureAction_NymphSummonInto* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_NymphSummonInto_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}