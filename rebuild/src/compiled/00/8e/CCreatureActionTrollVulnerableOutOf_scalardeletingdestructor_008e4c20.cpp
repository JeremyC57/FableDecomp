struct CCreatureAction_TrollVulnerableOutOf {
    void* vftable;
};

void __fastcall CCreatureAction_TrollVulnerableOutOf_dtor(CCreatureAction_TrollVulnerableOutOf* action);
void operator delete(void*);

void* __fastcall CCreatureAction_TrollVulnerableOutOf_scalar_deleting_destructor(CCreatureAction_TrollVulnerableOutOf* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_TrollVulnerableOutOf_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}