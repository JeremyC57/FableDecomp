class CCombatAction_Guildmaster_HealFollowersRelease { public:
    void* vftable;
};

void __fastcall CCombatAction_Guildmaster_HealFollowersRelease_dtor(CCombatAction_Guildmaster_HealFollowersRelease* action);
void operator delete(void*);

void* __fastcall CCombatAction_Guildmaster_HealFollowersRelease_scalar_deleting_destructor(CCombatAction_Guildmaster_HealFollowersRelease* action, int edx_unused, unsigned int flags)
{
    CCombatAction_Guildmaster_HealFollowersRelease_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}