class CCreatureAction_WalkOutOf { public:
    void* vftable;
};

void __fastcall CCreatureAction_WalkOutOf_dtor(CCreatureAction_WalkOutOf* action);
void operator delete(void*);

void* __fastcall CCreatureAction_WalkOutOf_scalar_deleting_destructor(CCreatureAction_WalkOutOf* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_WalkOutOf_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}