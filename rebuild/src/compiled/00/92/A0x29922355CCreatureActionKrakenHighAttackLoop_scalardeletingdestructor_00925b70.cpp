class CCombatActionBase {
public:
    virtual ~CCombatActionBase();   // 0x0062E900
};
struct CCreatureAction_KrakenHighAttackLoop {
    void* vftable;
};

void operator delete(void*);

void* __fastcall CCreatureAction_KrakenHighAttackLoop_scalar_deleting_destructor(CCreatureAction_KrakenHighAttackLoop* action, int edx_unused, unsigned int flags)
{
    ((CCombatActionBase*)(action))->CCombatActionBase::~CCombatActionBase();
    if (flags & 1)
        operator delete(action);
    return action;
}