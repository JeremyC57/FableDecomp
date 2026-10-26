class CCreatureAction_WaspQueenLashOutAttack { public:
    void* vftable;
};

void __fastcall CCreatureAction_WaspQueenLashOutAttack_dtor(CCreatureAction_WaspQueenLashOutAttack* action);
void operator delete(void*);

void* __fastcall CCreatureAction_WaspQueenLashOutAttack_scalar_deleting_destructor(CCreatureAction_WaspQueenLashOutAttack* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_WaspQueenLashOutAttack_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}