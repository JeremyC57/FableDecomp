class CCreatureAction_OnGenerate { public:
    void* vftable;
};

void __fastcall CCreatureAction_OnGenerate_dtor(CCreatureAction_OnGenerate* action);
void operator delete(void*);

void* __fastcall CCreatureAction_OnGenerate_scalar_deleting_destructor(CCreatureAction_OnGenerate* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_OnGenerate_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}