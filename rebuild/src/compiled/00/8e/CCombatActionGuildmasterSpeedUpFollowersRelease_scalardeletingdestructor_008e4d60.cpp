class CCombatActionBase {
public:
    virtual ~CCombatActionBase();   // 0x0062E900
};
class CCombatAction_Guildmaster_SpeedUpFollowersRelease { public:
    void* vftable;
};

void operator delete(void*);

void* __fastcall CCombatAction_Guildmaster_SpeedUpFollowersRelease_scalar_deleting_destructor(CCombatAction_Guildmaster_SpeedUpFollowersRelease* action, int edx_unused, unsigned int flags)
{
    ((CCombatActionBase*)(action))->CCombatActionBase::~CCombatActionBase();
    if (flags & 1)
        operator delete(action);
    return action;
}