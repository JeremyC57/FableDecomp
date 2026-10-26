class CCombatActionBase {
public:
    virtual ~CCombatActionBase();   // 0x0062E900
};
class CCombatAction_WillHealRelease { public:
    void* vftable;
};

void operator delete(void*);

void* __fastcall CCombatAction_WillHealRelease_scalar_deleting_destructor(CCombatAction_WillHealRelease* action, int edx_unused, unsigned int flags)
{
    ((CCombatActionBase*)(action))->CCombatActionBase::~CCombatActionBase();
    if (flags & 1)
        operator delete(action);
    return action;
}