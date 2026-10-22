struct CCombatAction_Guildmaster_SpeedUpFollowersInto {
    void* vftable;
};

void __fastcall CCombatAction_Guildmaster_SpeedUpFollowersInto_dtor(CCombatAction_Guildmaster_SpeedUpFollowersInto* action);
void operator delete(void*);

void* __fastcall CCombatAction_Guildmaster_SpeedUpFollowersInto_scalar_deleting_destructor(CCombatAction_Guildmaster_SpeedUpFollowersInto* action, int edx_unused, unsigned int flags)
{
    CCombatAction_Guildmaster_SpeedUpFollowersInto_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}