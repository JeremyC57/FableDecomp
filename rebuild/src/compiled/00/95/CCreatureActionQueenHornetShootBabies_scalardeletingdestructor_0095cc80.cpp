class CCreatureAction_QueenHornetShootBabies { public:
    void* vftable;
};

void __fastcall CCreatureAction_QueenHornetShootBabies_dtor(CCreatureAction_QueenHornetShootBabies* action);
void operator delete(void*);

void* __fastcall CCreatureAction_QueenHornetShootBabies_scalar_deleting_destructor(CCreatureAction_QueenHornetShootBabies* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_QueenHornetShootBabies_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}