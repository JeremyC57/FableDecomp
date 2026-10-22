struct CCombatAction_Guildmaster_HealFollowersInto {
    void* vftable;
};

void __fastcall CCombatAction_Guildmaster_HealFollowersInto_dtor(CCombatAction_Guildmaster_HealFollowersInto* action);
void operator delete(void*);

void* __fastcall CCombatAction_Guildmaster_HealFollowersInto_scalar_deleting_destructor(CCombatAction_Guildmaster_HealFollowersInto* action, int edx_unused, unsigned int flags)
{
    CCombatAction_Guildmaster_HealFollowersInto_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}