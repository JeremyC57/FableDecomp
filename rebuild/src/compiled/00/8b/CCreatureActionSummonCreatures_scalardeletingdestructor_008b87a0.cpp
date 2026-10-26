class CCreatureAction_SummonCreatures { public:
    void* vftable;
};

void __fastcall CCreatureAction_SummonCreatures_dtor(CCreatureAction_SummonCreatures* action);
void operator delete(void*);

void* __fastcall CCreatureAction_SummonCreatures_scalar_deleting_destructor(CCreatureAction_SummonCreatures* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_SummonCreatures_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}