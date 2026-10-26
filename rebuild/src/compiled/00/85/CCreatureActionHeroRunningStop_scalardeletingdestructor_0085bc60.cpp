class CCreatureActionHeroRunningStop { public:
    void* vftable;
};

void __fastcall CCreatureActionHeroRunningStop_dtor(CCreatureActionHeroRunningStop* action);
void operator delete(void*);

void* __fastcall CCreatureActionHeroRunningStop_scalar_deleting_destructor(CCreatureActionHeroRunningStop* action, int edx_unused, unsigned int flags)
{
    CCreatureActionHeroRunningStop_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}