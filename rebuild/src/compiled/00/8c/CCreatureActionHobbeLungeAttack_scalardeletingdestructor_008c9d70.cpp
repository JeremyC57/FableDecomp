struct CCreatureAction_HobbeLungeAttack {
    void* vftable;
};

void __fastcall CCreatureAction_HobbeLungeAttack_dtor(CCreatureAction_HobbeLungeAttack* action);
void operator delete(void*);

void* __fastcall CCreatureAction_HobbeLungeAttack_scalar_deleting_destructor(CCreatureAction_HobbeLungeAttack* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_HobbeLungeAttack_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}