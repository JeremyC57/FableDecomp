class CCreatureAction_BanditKingChangeWeapons { public:
    void* vftable;
};

void __fastcall CCreatureAction_BanditKingChangeWeapons_dtor(CCreatureAction_BanditKingChangeWeapons* action);
void operator delete(void*);

void* __fastcall CCreatureAction_BanditKingChangeWeapons_scalar_deleting_destructor(CCreatureAction_BanditKingChangeWeapons* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_BanditKingChangeWeapons_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}