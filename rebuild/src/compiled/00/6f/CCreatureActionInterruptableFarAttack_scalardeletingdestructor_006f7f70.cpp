class CCombatActionBase {
public:
    virtual ~CCombatActionBase();   // 0x0062E900
};
class CCreatureAction_InterruptableFarAttack { public:
    void* vftable;
};

void operator delete(void*);

void* __fastcall CCreatureAction_InterruptableFarAttack_scalar_deleting_destructor(CCreatureAction_InterruptableFarAttack* action, int edx_unused, unsigned int flags)
{
    ((CCombatActionBase*)(action))->CCombatActionBase::~CCombatActionBase();
    if (flags & 1)
        operator delete(action);
    return action;
}