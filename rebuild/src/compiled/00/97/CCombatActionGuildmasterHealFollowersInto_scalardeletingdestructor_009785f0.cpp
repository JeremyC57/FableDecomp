class CCombatActionBase {
public:
    virtual ~CCombatActionBase();   // 0x0062E900
};
class CCombatAction_Guildmaster_HealFollowersInto { public:
    void* vftable;
};

void operator delete(void*);

void* __fastcall CCombatAction_Guildmaster_HealFollowersInto_scalar_deleting_destructor(CCombatAction_Guildmaster_HealFollowersInto* action, int edx_unused, unsigned int flags)
{
    ((CCombatActionBase*)(action))->CCombatActionBase::~CCombatActionBase();
    if (flags & 1)
        operator delete(action);
    return action;
}