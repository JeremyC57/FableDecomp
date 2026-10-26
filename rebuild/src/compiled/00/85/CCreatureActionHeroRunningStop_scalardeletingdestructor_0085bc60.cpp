class CCombatActionBase {
public:
    virtual ~CCombatActionBase();   // 0x0062E900
};
class CCreatureActionHeroRunningStop { public:
    void* vftable;
};

void operator delete(void*);

void* __fastcall CCreatureActionHeroRunningStop_scalar_deleting_destructor(CCreatureActionHeroRunningStop* action, int edx_unused, unsigned int flags)
{
    ((CCombatActionBase*)(action))->CCombatActionBase::~CCombatActionBase();
    if (flags & 1)
        operator delete(action);
    return action;
}