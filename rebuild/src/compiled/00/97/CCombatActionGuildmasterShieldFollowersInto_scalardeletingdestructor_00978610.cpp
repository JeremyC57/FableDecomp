struct CCombatAction_Guildmaster_ShieldFollowersInto {
    void* vftable;
};

void __fastcall CCombatAction_Guildmaster_ShieldFollowersInto_dtor(CCombatAction_Guildmaster_ShieldFollowersInto* action);
void operator delete(void*);

void* __fastcall CCombatAction_Guildmaster_ShieldFollowersInto_scalar_deleting_destructor(CCombatAction_Guildmaster_ShieldFollowersInto* action, int edx_unused, unsigned int flags)
{
    CCombatAction_Guildmaster_ShieldFollowersInto_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}