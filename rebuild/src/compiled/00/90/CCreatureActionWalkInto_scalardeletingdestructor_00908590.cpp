class CCreatureAction_WalkInto { public:
    void* vftable;
};

void __fastcall CCreatureAction_WalkInto_dtor(CCreatureAction_WalkInto* action);
void operator delete(void*);

void* __fastcall CCreatureAction_WalkInto_scalar_deleting_destructor(CCreatureAction_WalkInto* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_WalkInto_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}