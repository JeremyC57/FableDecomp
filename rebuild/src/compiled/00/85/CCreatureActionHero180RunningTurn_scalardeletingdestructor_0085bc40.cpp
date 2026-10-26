class CCreatureActionHero180RunningTurn { public:
    void* vftable;
};

void __fastcall CCreatureActionHero180RunningTurn_dtor(CCreatureActionHero180RunningTurn* action);
void operator delete(void*);

void* __fastcall CCreatureActionHero180RunningTurn_scalar_deleting_destructor(CCreatureActionHero180RunningTurn* action, int edx_unused, unsigned int flags)
{
    CCreatureActionHero180RunningTurn_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}