class CCombatActionBase {
public:
    virtual ~CCombatActionBase();   // 0x0062E900
};
class CCreatureAction_UndeadRiseUp { public:
    void* vftable;
};

void operator delete(void*);

void* __fastcall CCreatureAction_UndeadRiseUp_scalar_deleting_destructor(CCreatureAction_UndeadRiseUp* action, int edx_unused, unsigned int flags)
{
    ((CCombatActionBase*)(action))->CCombatActionBase::~CCombatActionBase();
    if (flags & 1)
        operator delete(action);
    return action;
}