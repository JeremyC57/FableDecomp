class CCombatAction_Guildmaster_ShieldFollowersRelease { public:
    void* vftable;
};

void __fastcall CCombatAction_Guildmaster_ShieldFollowersRelease_dtor(CCombatAction_Guildmaster_ShieldFollowersRelease* action);
void operator delete(void*);

void* __fastcall CCombatAction_Guildmaster_ShieldFollowersRelease_scalar_deleting_destructor(CCombatAction_Guildmaster_ShieldFollowersRelease* action, int edx_unused, unsigned int flags)
{
    CCombatAction_Guildmaster_ShieldFollowersRelease_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}