class CCombatAction_Guildmaster_SpeedUpFollowersRelease { public:
    void* vftable;
};

void __fastcall CCombatAction_Guildmaster_SpeedUpFollowersRelease_dtor(CCombatAction_Guildmaster_SpeedUpFollowersRelease* action);
void operator delete(void*);

void* __fastcall CCombatAction_Guildmaster_SpeedUpFollowersRelease_scalar_deleting_destructor(CCombatAction_Guildmaster_SpeedUpFollowersRelease* action, int edx_unused, unsigned int flags)
{
    CCombatAction_Guildmaster_SpeedUpFollowersRelease_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}