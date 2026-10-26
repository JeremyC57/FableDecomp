class CCombatActionBase {
public:
    virtual ~CCombatActionBase();   // 0x0062E900
};
class CCombatAction_ControlledStrafeJump { public:
    void* vftable;
};

void operator delete(void*);

void* __fastcall CCombatAction_ControlledStrafeJump_scalar_deleting_destructor(CCombatAction_ControlledStrafeJump* action, int edx_unused, unsigned int flags)
{
    ((CCombatActionBase*)(action))->CCombatActionBase::~CCombatActionBase();
    if (flags & 1)
        operator delete(action);
    return action;
}