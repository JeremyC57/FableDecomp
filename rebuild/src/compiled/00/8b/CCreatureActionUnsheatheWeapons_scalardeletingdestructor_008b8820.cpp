struct CCreatureAction_UnsheatheWeapons {
    void* vftable;
};

void __fastcall CCreatureAction_UnsheatheWeapons_dtor(CCreatureAction_UnsheatheWeapons* action);
void operator delete(void*);

void* __fastcall CCreatureAction_UnsheatheWeapons_scalar_deleting_destructor(CCreatureAction_UnsheatheWeapons* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_UnsheatheWeapons_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}