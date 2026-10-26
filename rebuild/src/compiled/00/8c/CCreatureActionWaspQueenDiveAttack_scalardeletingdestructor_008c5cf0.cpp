class CCreatureAction_WaspQueenDiveAttack { public:
    void* vftable;
};

void __fastcall CCreatureAction_WaspQueenDiveAttack_dtor(CCreatureAction_WaspQueenDiveAttack* action);
void operator delete(void*);

void* __fastcall CCreatureAction_WaspQueenDiveAttack_scalar_deleting_destructor(CCreatureAction_WaspQueenDiveAttack* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_WaspQueenDiveAttack_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}