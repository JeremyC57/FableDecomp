class CCombatActionBase {
public:
    virtual ~CCombatActionBase();   // 0x0062E900
};
class CCreatureAction_TentacleIdle { public:
    void* vftable;
};

void operator delete(void*);

void* __fastcall CCreatureAction_TentacleIdle_scalar_deleting_destructor(CCreatureAction_TentacleIdle* action, int edx_unused, unsigned int flags)
{
    ((CCombatActionBase*)(action))->CCombatActionBase::~CCombatActionBase();
    if (flags & 1)
        operator delete(action);
    return action;
}