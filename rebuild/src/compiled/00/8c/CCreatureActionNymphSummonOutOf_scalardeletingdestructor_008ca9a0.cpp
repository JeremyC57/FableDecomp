class CCreatureAction_NymphSummonOutOf { public:
    void* vftable;
};

void __fastcall CCreatureAction_NymphSummonOutOf_dtor(CCreatureAction_NymphSummonOutOf* action);
void operator delete(void*);

void* __fastcall CCreatureAction_NymphSummonOutOf_scalar_deleting_destructor(CCreatureAction_NymphSummonOutOf* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_NymphSummonOutOf_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}