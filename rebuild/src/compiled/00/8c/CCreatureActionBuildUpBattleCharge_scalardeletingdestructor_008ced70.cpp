class CCombatActionBase {
public:
    virtual ~CCombatActionBase();   // 0x0062E900
};
class CCreatureAction_BuildUpBattleCharge { public:
    void* vftable;
};

void operator delete(void*);

void* __fastcall CCreatureAction_BuildUpBattleCharge_scalar_deleting_destructor(CCreatureAction_BuildUpBattleCharge* action, int edx_unused, unsigned int flags)
{
    ((CCombatActionBase*)(action))->CCombatActionBase::~CCombatActionBase();
    if (flags & 1)
        operator delete(action);
    return action;
}