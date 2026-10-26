class CCreatureAction_TrollVulnerableInto { public:
    void* vftable;
};

void __fastcall CCreatureAction_TrollVulnerableInto_dtor(CCreatureAction_TrollVulnerableInto* action);
void operator delete(void*);

void* __fastcall CCreatureAction_TrollVulnerableInto_scalar_deleting_destructor(CCreatureAction_TrollVulnerableInto* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_TrollVulnerableInto_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}