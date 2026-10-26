class CCreatureAction_SheatheWeapons { public:
    void* vftable;
};

void __fastcall CCreatureAction_SheatheWeapons_dtor(CCreatureAction_SheatheWeapons* action);
void operator delete(void*);

void* __fastcall CCreatureAction_SheatheWeapons_scalar_deleting_destructor(CCreatureAction_SheatheWeapons* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_SheatheWeapons_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}