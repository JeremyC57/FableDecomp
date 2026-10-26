class CCombatActionBase {
public:
    virtual ~CCombatActionBase();   // 0x0062E900
};
class CCombatAction_Guildmaster_SpeedUpFollowersInto { public:
    void* vftable;
};

void operator delete(void*);

void* __fastcall CCombatAction_Guildmaster_SpeedUpFollowersInto_scalar_deleting_destructor(CCombatAction_Guildmaster_SpeedUpFollowersInto* action, int edx_unused, unsigned int flags)
{
    ((CCombatActionBase*)(action))->CCombatActionBase::~CCombatActionBase();
    if (flags & 1)
        operator delete(action);
    return action;
}