class CCreatureAction_Ambush { public:
    void* vftable;
};

void __fastcall CCreatureAction_Ambush_dtor(CCreatureAction_Ambush* action);
void operator delete(void*);

void* __fastcall CCreatureAction_Ambush_scalar_deleting_destructor(CCreatureAction_Ambush* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_Ambush_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}