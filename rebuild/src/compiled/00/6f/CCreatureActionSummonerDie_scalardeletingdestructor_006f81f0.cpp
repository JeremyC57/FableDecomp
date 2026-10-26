class CCombatActionBase {
public:
    virtual ~CCombatActionBase();   // 0x0062E900
};
class CCreatureAction_SummonerDie { public:
    void* vftable;
};

void operator delete(void*);

void* __fastcall CCreatureAction_SummonerDie_scalar_deleting_destructor(CCreatureAction_SummonerDie* action, int edx_unused, unsigned int flags)
{
    ((CCombatActionBase*)(action))->CCombatActionBase::~CCombatActionBase();
    if (flags & 1)
        operator delete(action);
    return action;
}