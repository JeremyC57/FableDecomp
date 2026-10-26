class CCombatActionBase {
public:
    virtual ~CCombatActionBase();   // 0x0062E900
};
class CCreatureAction_BeetleTakeOff { public:
    void* vftable;
};

void operator delete(void*);

void* __fastcall CCreatureAction_BeetleTakeOff_scalar_deleting_destructor(CCreatureAction_BeetleTakeOff* action, int edx_unused, unsigned int flags)
{
    ((CCombatActionBase*)(action))->CCombatActionBase::~CCombatActionBase();
    if (flags & 1)
        operator delete(action);
    return action;
}