class CCombatActionBase {
public:
    virtual ~CCombatActionBase();   // 0x0062E900
};
class CCombatAction_WillHealInto { public:
    void* vftable;
};

void operator delete(void*);

void* __fastcall CCombatAction_WillHealInto_scalar_deleting_destructor(CCombatAction_WillHealInto* action, int edx_unused, unsigned int flags)
{
    ((CCombatActionBase*)(action))->CCombatActionBase::~CCombatActionBase();
    if (flags & 1)
        operator delete(action);
    return action;
}