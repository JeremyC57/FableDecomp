class CCombatActionBase {
public:
    virtual ~CCombatActionBase();   // 0x0062E900
};
class CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath { public:
    void* vftable;
};

void operator delete(void*);

void* __fastcall CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath_scalar_deleting_destructor(CCombatAction_GenericStrikeResponseKnockdownRecoverFromDeath* action, int edx_unused, unsigned int flags)
{
    ((CCombatActionBase*)(action))->CCombatActionBase::~CCombatActionBase();
    if (flags & 1)
        operator delete(action);
    return action;
}