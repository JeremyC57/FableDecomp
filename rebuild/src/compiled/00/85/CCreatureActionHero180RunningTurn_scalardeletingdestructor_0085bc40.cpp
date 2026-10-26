class CCombatActionBase {
public:
    virtual ~CCombatActionBase();   // 0x0062E900
};
class CCreatureActionHero180RunningTurn { public:
    void* vftable;
};

void operator delete(void*);

void* __fastcall CCreatureActionHero180RunningTurn_scalar_deleting_destructor(CCreatureActionHero180RunningTurn* action, int edx_unused, unsigned int flags)
{
    ((CCombatActionBase*)(action))->CCombatActionBase::~CCombatActionBase();
    if (flags & 1)
        operator delete(action);
    return action;
}