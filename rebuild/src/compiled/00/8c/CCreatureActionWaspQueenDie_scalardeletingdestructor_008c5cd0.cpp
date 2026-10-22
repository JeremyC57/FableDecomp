struct CCreatureAction_WaspQueenDie {
    void* vftable;
};

void __fastcall CCreatureAction_WaspQueenDie_dtor(CCreatureAction_WaspQueenDie* action);
void operator delete(void*);

void* __fastcall CCreatureAction_WaspQueenDie_scalar_deleting_destructor(CCreatureAction_WaspQueenDie* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_WaspQueenDie_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}