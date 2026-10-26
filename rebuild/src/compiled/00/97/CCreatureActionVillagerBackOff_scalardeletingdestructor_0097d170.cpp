class CCombatActionBase {
public:
    virtual ~CCombatActionBase();   // 0x0062E900
};
class CCreatureAction_VillagerBackOff { public:
    void* vftable;
};

void operator delete(void*);

void* __fastcall CCreatureAction_VillagerBackOff_scalar_deleting_destructor(CCreatureAction_VillagerBackOff* action, int edx_unused, unsigned int flags)
{
    ((CCombatActionBase*)(action))->CCombatActionBase::~CCombatActionBase();
    if (flags & 1)
        operator delete(action);
    return action;
}